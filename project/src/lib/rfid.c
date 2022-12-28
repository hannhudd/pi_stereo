#include "spi.h"
#include "rfid.h"
#include "timer.h"
#include "gpio.h"

static Uid uid;

void PCD_Init(void)
{
    spi_init(SPI_CE0, CLK_DIV);
    gpio_set_input(RST);
    if (gpio_read(RST) == 0){
	gpio_set_output(RST);
	gpio_write(RST, 0);
	timer_delay_us(2);
	gpio_write(RST, 1);
	timer_delay_ms(50);
    }
    else {
	PCD_Reset();
    }
    //configure timer is something goes wrong
    PCD_WriteRegisterOne(TModeReg, 0x8D); //timer starts automatically at end of transmission
    PCD_WriteRegisterOne(TPrescalerReg, 0x3E); 
    PCD_WriteRegisterOne(TReloadRegH, 0); 
    PCD_WriteRegisterOne(TReloadRegL, 30);
    PCD_WriteRegisterOne(TxASKReg, 0x40);
    PCD_WriteRegisterOne(ModeReg, 0x3D); //set preset value for CRC coprocessor
    PCD_AntennaOn();
}

void PCD_WriteRegisterOne(PCD_Register reg, unsigned char value)
{
    unsigned char rx[2];
    unsigned char tx[2];
    tx[0] = reg;
    tx[1] = value;
    spi_transfer(tx, rx, 2);
}

unsigned char PCD_ReadRegisterOne(PCD_Register reg)
{
    unsigned char addr = (0x80 | reg); //most significant bit is 1 in read mode
    unsigned char rx[2];
    unsigned char tx[2];
    tx[0] = addr;
    tx[1] = 0;
    spi_transfer(tx, rx, 2);
    return rx[1];
}

void PCD_WriteRegisterMany(PCD_Register reg, unsigned int count, unsigned char *values)
{
    unsigned char rx[count + 1];
    unsigned char tx[count + 1];
    tx[0] = reg;
    for (int i = 0; i < count; i++) tx[i + 1] = values[i];
    spi_transfer(tx, rx, count + 1);
}

void PCD_ReadRegisterMany(PCD_Register reg, unsigned int count, unsigned char *returnValues)
{
    unsigned char addr = (0x80 | reg); //most significant bit is 1 in read mode
    unsigned char rx[count + 1];
    unsigned char tx[count + 1];    
    for (int i = 0; i < count; i++) tx[i] = addr;
    tx[count] = 0;
    spi_transfer(tx, rx, count + 1);
    for (int j = 0; j < count; j++) returnValues[j] = rx[j + 1];
}

void SetBitMask(PCD_Register reg, unsigned char mask)
{
    unsigned char temp = PCD_ReadRegisterOne(reg);
    PCD_WriteRegisterOne(reg, temp | mask);
}

void ClearBitMask(PCD_Register reg, unsigned char mask)
{
    unsigned char temp = PCD_ReadRegisterOne(reg);
    PCD_WriteRegisterOne(reg, temp & (~mask));
}

void PCD_AntennaOn(void)
{
    unsigned char value = PCD_ReadRegisterOne(TxControlReg);
    if ((value & 0x03) != 0x03) { //0x03 turns on 13.56 MHz energy carrier
	SetBitMask(TxControlReg, 0x03);
    }
}

void PCD_Reset(void)
{
    PCD_WriteRegisterOne(CommandReg, PCD_SoftReset);
    timer_delay_ms(150); //wait a bit for oscillator to start up
}

unsigned char PCD_Version(void)
{
    return PCD_ReadRegisterOne(VersionReg);
}

//transfer data to the MFRC522 FIFO, excutes a command, waits for completion, and transfers data back from FIFO

int PCD_ToPICC(PCD_Command command, unsigned char* senData, unsigned int senLen, unsigned char* recData, unsigned int recLen)
{
    unsigned char irqEn = 0x00;
    unsigned char waitIRq = 0x00;
    if (command == PCD_MFAuthent) {
	irqEn = 0x12; //idle and error
	waitIRq = 0x10; //end of command
    }
    if (command == PCD_Transceive) {
	irqEn = 0x77;
	waitIRq = 0x30; //end of valid data stream or end of command
    }
    PCD_WriteRegisterOne(CommIEnReg, irqEn | 0x80);
    ClearBitMask(ComIrqReg, 0x80); //clear interrupt bits
    SetBitMask(FIFOLevelReg, 0x80); //FIFO initialization
    PCD_WriteRegisterOne(CommandReg, PCD_Idle); //stop active commands
    PCD_WriteRegisterMany(FIFODataReg, senLen, senData); //write data to the FIFO
    PCD_WriteRegisterOne(CommandReg, command); //execute command
    
    if (command == PCD_Transceive) {
    	SetBitMask(BitFramingReg, 0x80); //to initialize transmission
    }
    int i;
    for (i = 2000; i > 0; i--) {
	unsigned char n = PCD_ReadRegisterOne(ComIrqReg);
	if (n & waitIRq) { // One of the interrupts that signal success has been set.
	    break;
	}
	if (n & 0x01) { // Timer interrupt - nothing received in 25ms
	    return 0;
	}
    }
    ClearBitMask(BitFramingReg, 0x80);
    if (i == 0) { //communication with MFRC522 might be down
	return 0;
    }
    unsigned char errorRegValue = PCD_ReadRegisterOne(ErrorReg);
    if (errorRegValue & 0x13) { // BufferOvfl ParityErr ProtocolErr
	return 0;
    }
    if (recLen > 0) {
	int sizeFIFO = PCD_ReadRegisterOne(FIFOLevelReg); //number of bytes in the FIFO
	if (sizeFIFO > recLen) {
 	    return 0;
	}
        recLen = sizeFIFO;
	PCD_ReadRegisterMany(FIFODataReg, recLen, recData); //get data back from FIFO
    }
    return 1;
}

int PCD_TransceiveData(unsigned char* sendData, unsigned int sendLen, unsigned char* recData, unsigned int recLen)
{
    return PCD_ToPICC(PCD_Transceive, sendData, sendLen, recData, recLen);
}

//wake up PICC from IDLE or HALT to READY state
int PICC_WakeUp(void)
{
    unsigned char command = PICC_REQA;
    PCD_WriteRegisterOne(BitFramingReg, 7);
    return PCD_TransceiveData(&command, 1, 0, 0); 
}

int PCD_CalculateCRC(unsigned char* data, unsigned int length, unsigned char* result)
{
    PCD_WriteRegisterOne(CommandReg, PCD_Idle); //stop any active commands 
    PCD_WriteRegisterOne(DivIrqReg, 0x04); //clear the CRCIRq interrupt request bit
    PCD_WriteRegisterOne(FIFOLevelReg, 0x80); //FIFO initialization 
    PCD_WriteRegisterMany(FIFODataReg, length, data); //write data to the FIFO
    PCD_WriteRegisterOne(CommandReg, PCD_CalcCRC); //start calculation
    for (int i = 5000; i > 0; i--) {
	unsigned char n = PCD_ReadRegisterOne(DivIrqReg);
	if (n & 0x04) { //calculation done
	    PCD_WriteRegisterOne(CommandReg, PCD_Idle);
	    result[0] = PCD_ReadRegisterOne(CRCResultRegL); //transfer result
	    result[1] = PCD_ReadRegisterOne(CRCResultRegH);
	    return 1;
	}
    }
    return 0;
}

//PICC must be in READY mode before calling this function
//put PICC in state ACTIVE and get UID

int PICC_Anticoll(void)
{
    PCD_WriteRegisterOne(BitFramingReg, 0x00);
    int uidComplete = 0;
    int selectDone;
    int cascadeLevel = 1;
    int innerLoop;
    int uidIndex = 0;
    int index;
    int result = 0;
    unsigned char buffer[9];
    unsigned char bufferUsed = 2;
    unsigned char* responseBuffer;
    unsigned char responseLen;

    while (!uidComplete) {
	switch (cascadeLevel) {
	    case 1:
		buffer[0] = PICC_SEL_CL1;
		uidIndex = 0;
		break;
	    case 2:
		buffer[0] = PICC_SEL_CL2;	
		uidIndex = 3;
		break;
	    case 3:
		buffer[0] = PICC_SEL_CL3;
		uidIndex = 6;
		break;
    	}
	innerLoop = 0;
	selectDone = 0;
 	while (!selectDone) {
	    if (innerLoop == 1) {
		buffer[1] = 0x70;
		buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
		result = PCD_CalculateCRC(buffer, 7, &buffer[7]);
		if (result == 0) return 0;
		bufferUsed = 9;
		responseBuffer = &buffer[6];
		responseLen = 3;
		selectDone = 1;
	    }
	    else {
		index = 2;
		buffer[1] = (index << 4);
		bufferUsed = index;
		responseBuffer = &buffer[index];
		responseLen = sizeof(buffer) - index;
		innerLoop = 1;
	    }
	    result = PCD_TransceiveData(buffer, bufferUsed, responseBuffer, responseLen);
	    if (result == 0) {	
		return 0;
	    }
	}
	int index = (buffer[2] == PICC_CT) ? 3 : 2;
	int bytesToCopy = (buffer[2] == PICC_CT) ? 3 : 4;
	for (int count = 0; count < bytesToCopy; count++) {
	    uid.uidByte[uidIndex + count] = buffer[index++];
	}
	if (responseBuffer[0] & 0x04) cascadeLevel++;
	else {
	    uidComplete = 1;
	    uid.sak = responseBuffer[0];
	}
    }
    uid.size = 3 * cascadeLevel + 1;
    return 1;
}

//the SAK is a code used to identify the RFID card type
unsigned char getSAK(void)
{
    return uid.sak;
}

//UID is a 4, 7, or 10 byte code unique to the RFID card
unsigned char* getUID(void)
{
    return uid.uidByte;
}

//UID size is 4, 7, 0 10 bytes
int getUIDSize(void)
{  
    return uid.size;
}

//read 4 pages of data from an NTAG 215 card
int MIFARE_Read(char blockAddr, unsigned char* returnData, int returnSize)
{
    if (returnSize < 18) return 0;
    unsigned char cmdBuffer[4];
    cmdBuffer[0] = PICC_Read;
    cmdBuffer[1] = blockAddr;
    int result = PCD_CalculateCRC(cmdBuffer, 2, &cmdBuffer[2]);
    if (result == 0) return 0;
    result = PCD_TransceiveData(cmdBuffer, 4, returnData, returnSize);
    if (result == 0) return 0;
    return 1;
}

//write 1 page of data to an NTAG 215 card
int MIFARE_ULWrite(int page, unsigned char* buffer, int bufferSize)
{
    if (bufferSize != 4) return 0;
    unsigned char cmdBuffer[8];
    cmdBuffer[0] = PICC_ULWrite;
    cmdBuffer[1] = page;
    for (int i = 0; i < 4; i++) {
	cmdBuffer[i + 2] = buffer[i];
    }
    int result = PCD_CalculateCRC(cmdBuffer, 6, &cmdBuffer[6]);
    if (result == 0) return 0;
    result = PCD_TransceiveData(cmdBuffer, 8, cmdBuffer, 8);
    if (result == 0) return 0;
    if (cmdBuffer[0] != 0xA) { //0xA is the ACK
     	return 0;
    }
    return 1;
}

//write a number to page 6
void wake_select_writenum(unsigned char num)
{
    unsigned char buf[4] = {num, 0, 0, 0};
    while (1) {
    	if (PICC_WakeUp()) {
            if (PICC_Anticoll()) {
                if (MIFARE_ULWrite(6, buf, 4)) break;
            }
        }
    }
}

//read a number from page 6
unsigned int wake_select_readnum(unsigned int *return_value)
{
    unsigned char returnBuf[18];
    if (PICC_WakeUp()) {
        if (PICC_Anticoll()) {
            if (MIFARE_Read(6, returnBuf, 18)) {
                *return_value = (unsigned int)returnBuf[0];
                return 1;
            }
        }
    }
    return 0;
}
