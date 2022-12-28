## RFID Scanner Based MP3 Player with LCD Display
![Image](https://github.com/cs107e/Mokhalil01-hannhudd-project/blob/master/player_image.jpeg)
## Team members
Hannah Huddleston\
Moe Khalil

## Project description
### Extra Components:
DFPlayer | 
20x4 LCD Display | 
RFID RC522 | 
JST-P2.0 Interface Speaker 

### Setup:
#### LCD Display
GND - GND | VCC - 5V | SDA - GPIO2 | SCL - GPIO3
#### DFPlayer
GND - GND | VCC - 5V | SPK1/SPK2 - Speaker | RX - GPIO6
#### RFID RC522
GND - GND | VCC - 3.3V | RST - GPIO20 | MISO - GPIO9 | MOSI - GPIO10 | SCK - GPIO11 | SDA - GPIO8
#### Other
Button - GPIO16 (Next Track) | Button - GPIO23 (Volume Up) | Button - GPIO24 (Volume Down)

### Milestones:
1. Get the DFPlayer to play music - M
2. Communicate with the RFID module using SPI - H
3. Connect buttons to DFPlayer to pause/play/skip tracks - M
4. Configure interrupt-based RFID scanning - H
5. Try to write to RFID cards using module (if possible) - H
6. Connect music player to RFID module - Both
7. Communicate with LCD display using I2C - M
8. Create array with RFID codes - MP3 number - Track name to connect all 3 modules - Both

## Member contribution
**Moe** :\
          * Wrote bit-banging based asynchronous serial library to send data using internal timer.\
          * Wrote library to interact with LCD Display using I2C.\
          * Wrote library to interact with DFPlayer using asynchronous serial.    
          * Wrote main app code that reads cards and plays music and configured pull-up buttons to skip/increase volume/decrease volume.\
**Hannah** :\
          * Set up communication between the RPi and RC522 scanner using SPI.\
          * Adapted RC522 source code to run with the RPi instead of Arduino.\
          * Handled the transfer of data between the RC522 scanner and NTAG 215 cards.\
          * Wrote functions to search, select, and read/write a number to the NTAG 215 cards, which was used to identify the corresponding album.  

## References
**DF Robot Wiki : https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299**  
  * Used to get protocol to communicate with DFPlayer.  
  
**LiquidCrystal_I2C C++ Library : https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.cpp**  
  * Used to get protocol to communicate with LCD Display as the module did not come with a data sheet.  

**SPI/I2C Library: libpiextra from the CS107E repository**

**MFRC522-python Library: https://github.com/mxgxw/MFRC522-python.git**
  * Used as a general reference for interacting with the RC522 scanner.
  * Provided intialization settings for the RC522 timer registers.
  
**MFRC522 Library for Arduino: https://github.com/miguelbalboa/rfid.git**
  * Used as a baseline for RC522 initialization, data transfer to the scanner, and card selection.
  * In particular, the functions PCD_ToPICC, PCD_CalculateCRC, and PICC_Anticoll were based off of this library.

## Self-evaluation
We were able to complete our proposal and ended up with a final product that surpassed what we expected, which was great! The most difficult part of our project was communicating with the different modules using different protocols (I2C/SPI/Asynchronous) as well as dealing with poor documentation for many of the modules we bought.  
One of the hardest issues we faced was understanding the RFID cards and the several steps required to read or write to them. Another issue was that we received a faulty DFPlayer that did not respond to queries, and therefore had to implement an internal timer system and hardcode every song's length, which could have been avoided. Finally, we had to find a way to communicate with the DFPlayer despite the Raspberry Pi's UART TX and RX pins being in use, and so we implemented a bit-banging based communication library.  
We are both incredibly proud of the final product and have been showing it off and playing music on it all day. Just the fact that everything is working is so cool. We both wrote our first line of code ever this January in CS106A, and to be able to build something that includes so many different modules collaboratively is very rewarding. We both think we became much better at learning new technologies independently, and at reading manuals over and over again.  
Overall, we feel great about our final product and the process we went through to get to it.  

## Photos
[Video of Player in Action](https://github.com/cs107e/Mokhalil01-hannhudd-project/blob/master/player_video.mp4)
