#ifndef RFID_H
#define RRID_H

#define CLK_DIV 50 //MFRC522 accepts speeds up to 10 MHz, and this is 5 MHz
#define RST GPIO_PIN20

typedef enum {
    CommandReg		= 0x01 << 1, //starts and stops commands (see PCD_Command)
    CommIEnReg		= 0x02 << 1,
    ComIrqReg		= 0x04 << 1,
    DivIrqReg		= 0x05 << 1,
    ErrorReg		= 0x06 << 1,
    Status2Reg		= 0x08 << 1,
    FIFODataReg		= 0x09 << 1, //data input and output port for FIFO
    FIFOLevelReg	= 0x0A << 1, //number of bytes stored in FIFO
    BitFramingReg	= 0x0D << 1, //adjustments for bit-oriented frames, including StartSend with Transceive command
    CollReg		= 0x0E << 1,
    ModeReg 		= 0x11 << 1,
    TxModeReg		= 0x12 << 1,
    RxModeReg		= 0x13 << 1,
    TxControlReg	= 0x14 << 1,
    TxASKReg		= 0x15 << 1,
    CRCResultRegH	= 0x21 << 1,
    CRCResultRegL	= 0x22 << 1,
    ModWidthReg		= 0x24 << 1,
    TModeReg 		= 0x2A << 1,
    TPrescalerReg 	= 0x2B << 1,
    TReloadRegH		= 0x2C << 1,
    TReloadRegL		= 0x2D << 1,
    VersionReg		= 0x37 << 1
} PCD_Register;

typedef enum {
    PCD_Idle 		= 0x00, //cancel current command
    PCD_CalcCRC		= 0x03,
    PCD_Transmit 	= 0x04, //transmit data from FIFO
    PCD_Receive		= 0x08, //activate receiver circuits
    PCD_Transceive	= 0x0C, //transmit data from FIFO to antenna and activate receiver after transmission
    PCD_MFAuthent	 = 0x0E,   
    PCD_SoftReset	 = 0x0F //reset the PCD
} PCD_Command;

typedef enum {
    PICC_REQA		= 0x26,
    PICC_CT		= 0x88, //cascade tag
    PICC_SEL_CL1	= 0x93, //select cascade level 1
    PICC_SEL_CL2	= 0x95, //select cascade level 2
    PICC_SEL_CL3	= 0x97, //select cascade level 3
    PICC_Read		= 0x30,
    PICC_ULWrite	= 0xA2  
} PICC_Command;

//PCD functions
void PCD_Reset(void);
void PCD_Init(void); 
void PCD_AntennaOn(void);
unsigned char PCD_Version(void);
void PCD_WriteRegisterOne(PCD_Register reg, unsigned char value);
unsigned char PCD_ReadRegisterOne(PCD_Register reg);
void PCD_WriteRegisterMany(PCD_Register reg, unsigned int count, unsigned char *values);
void PCD_ReadRegisterMany(PCD_Register reg, unsigned int count, unsigned char *returnValues);
int PCD_CalculateCRC(unsigned char* data, unsigned int length, unsigned char* result);

//PICC functions
int PCD_CommunicateWithPICC(PCD_Command command, unsigned char* sendData, unsigned int sendLen, unsigned char* recData, unsigned int recLen);
int PCD_TransceiveData(unsigned char* sendData, unsigned int sendLen, unsigned char* recData, unsigned int recLen);
int PICC_WakeUp(void);
int PICC_Anticoll(void);
void SetBitMask(PCD_Register reg, unsigned char mask);
void ClearBitMask(PCD_Register reg, unsigned char mask);
unsigned char getSAK(void);
unsigned char* getUID(void);
int getUIDSize(void);
int MIFARE_ULWrite(int page, unsigned char* buffer, int bufferSize);
int MIFARE_Read(char blockAddr, unsigned char* returnData, int returnSize);
void wake_select_writenum(unsigned char num);
unsigned int wake_select_readnum(unsigned int *return_value);

typedef struct {
    unsigned char uidByte[10];
    unsigned char sak;
    int size;
} Uid;

#endif 
