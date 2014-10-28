ECSE-426-Final-Project
======================
The full system: 
![alt tag](https://raw.githubusercontent.com/mvertescher/ECSE426-FinalProject/master/IMAGES/fullsys.jpg)

A few images of the orientation listener: 
![alt tag](https://raw.githubusercontent.com/mvertescher/ECSE426-FinalProject/master/IMAGES/board1.jpg)

Top view: 
![alt tag](https://raw.githubusercontent.com/mvertescher/ECSE426-FinalProject/master/IMAGES/topview.jpg)

Side view:
![alt tag](https://raw.githubusercontent.com/mvertescher/ECSE426-FinalProject/master/IMAGES/sideview.jpg)


Pin assignments for SPI2 to the EZ430-RF2500T chipset:

SPI                           STM32F407VG       EZ430-RF2500T

SCK (SPI clock)               PB13              Pin 16

MISO (Master in Serial out)   PB14              Pin 15

MOSI (Master out Serial in)   PB15              Pin 18

CS  (Chip select)             PE04              Pin 17

Note that the SPI wires go by different names in different documentation files.
