#include "PCD8544_HOANGSA.h" //You can download it here: http://arduino.vn/tutorial/1345-nokia5110-huong-dan-su-dung-va-chia-se-thu-vien-hoang-sa or https://drive.google.com/file/d/0BzMEcyRK_uUFWWJIcHFxOThpRTg/view

String all_lcd_content = "";
int x_char = 0;
int y_char = 0;

PCD8544 lcd(14,15,16,17,18); //Setup lcd pin: RST, DC, DIN, CLK. (Connect pin in Readme.md)

void printlcd();

void setup() {
  Serial.begin(9600); //Open lcd port
  /*Power the LCD*/
  pinMode(19, OUTPUT);//VCC
  digitalWrite(19, HIGH);//For VCC
  /*Starting the lcd*/
  lcd.ON();//Power on the LCD
  lcd.SET(25,0,0,0,4); //Just don't edit this
  //lcd.Asc_String(10,15,Asc("Hello!"), BLACK);//For testing
  //lcd.Display();
}

void loop() {
  while (Serial.available() > 0) { //If the serial has incomming character, read it
    int incomingByte = Serial.read(); //Read the character ASCII number to the incomingByte varrible
    if (incomingByte < 32 | incomingByte > 127) {
      if (incomingByte == 13) { //If the user press enter
        all_lcd_content = String(all_lcd_content + '\n');
        Serial.println("Next line"); //For debug, in real run please comment this!
        x_char = 0;
        y_char = y_char + 8;
      }
      //Serial.println(F("Not in range!"));
    } else {
      char data = char(incomingByte); //Convert number to ASCII character
      all_lcd_content = String(all_lcd_content + data);
      printlcd(data);
    }
  }
}

void printlcd(char data){
  Serial.println(x_char);//For debug, in real run please comment this!
  Serial.println(y_char);//For debug, in real run please comment this!
  if(x_char < 80 && y_char < 41){
    lcd.Asc_Char(x_char,y_char,data,BLACK);
    lcd.Display();
    x_char = x_char + 5;
  } else {
    if(x_char > 76){
      x_char = 0;
      y_char = y_char + 8;
      lcd.Asc_Char(x_char,y_char,data,BLACK);
      lcd.Display();
    }
    if(y_char > 40){
      lcd.Clear();
      x_char = 0;
      y_char = 0;
      lcd.Asc_Char(x_char,y_char,data,BLACK);
      lcd.Display();
    }
  }
}
