#include "uart.h"
#include "rfid.h"
#include "printf.h"
#include "spi.h"
#include "timer.h"

//static void test_version(void)
//{
//    printf("Starting version test\n");
//    unsigned char result = PCD_Version();
//    printf("Version is %x\n", result);
//}

//static void test_sak(void)
//{   
//    printf("\n\nSearching for cards\n");
//    while (1) { 
//    unsigned char bufferAQTA[2];
//    int status = PICC_WakeUp(bufferAQTA, 2);
//    if (status == 1) {
//    	int select = PICC_Anticoll();
//    	if (select == 1) {
//	    printf("Card detected!\n");
//    	    int size = getUIDSize();
//    	    unsigned char* uid = getUID();
//    	    printf("Uid is:\n");
//    	    for (int i = 0; i < size; i++) {
//    	        printf("%x ", uid[i]);
//    	    }
//	    printf("\n\nScan another card\n");
//    	    PICC_Halt();
//	}
//	else printf("Could not find card\n");
//    }
//    timer_delay(3);
//    }
//}

//static void test_read_write(void)
//{
//    unsigned char buf[4] = {'H', 'i', ' ', 'M'};
//    unsigned char buf2[4] = {'o', 'e', ':', ')'};
//    unsigned char returnBuf[25];
//    printf("Searching for card\n");
//    while (1) {
//	int status = PICC_WakeUp();
//	if (status == 1) {
//	    int select = PICC_Anticoll();
//	    if (select == 1) {
//		int res = MIFARE_ULWrite(0x06, buf, 4);
//		if (res == 1) {
//		    res = MIFARE_ULWrite(0x07, buf2, 4);
//		    if (res == 1) {
//		    	printf("Write success!\n\n");
//		    	break;
//		    }
//		}
//	    }
//	}
//    }
//    timer_delay(2);
//    printf("Now scan again to read\n");
//    while (1) {
//        int stat = PICC_WakeUp();
//        if (stat == 1) {
//            int sel = PICC_Anticoll();
//            if (sel == 1) {
//                int s = MIFARE_Read(0x06, returnBuf, 25);
//                if (s == 1) {
//                    returnBuf[9] = '\0';
//		    printf("Card says: %s\n", returnBuf);
//                    break;
//                }
//            }
//        }
//    }
//}

static void test_read_writenum(void)
{
    wake_select_writenum(5);
    char ch = wake_select_readnum();
    printf("Num is %d\n", (int)ch);
}

void main(void)
{
    uart_init();
    PCD_Init();
//    test_version();
//    test_sak();
//    test_read_write();
    test_read_writenum();
    uart_putchar(EOT);
}
