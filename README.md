# Lcd Serial Print
Print Serial data to LCD Nokia 5110 5V. Support for enter next line, clear the screen when it run out. Run on Arduino!
### If you want to use with 3.3v, you have to modify the code!
### This is a small project for fun (actually for one of my project). Hope you like it!
## Pin
It's very simple, just connect the first 6 pin of the lcd (RST, CE, DC, Din, CLK, VCC) to the Analog A0 -> A5 in Arduino Uno. And the 2 other pin are BL and GND, connect them to GND pin on Arduino. If you like backlight, connect BL to 3.3v (I use this because it doesn't take a lot of space!)
## Tip
If you want to receive more character than the Arduino Serial Buffer, please go to [Arduino Directory](often C:\Program Files\arduino-[ver])\hardware\arduino\avr\cores\arduino\ and change the USBAPI.h at line SERIAL_BUFFER_SIZE 64 change it to 128, 256 or any size you want! Remember it must be smaller than your RAM amount! Good luck!
### Hope you like it! If you have any problem, fell free to ask me!
