

/*

// cập nhật ngày 4/5/2018 (Thái Sơn)
// fix lỗi thiếu kí tự 0 trong hàm Number_Float

PCD8544 _NOKIA5110 LCD library!

Copyright (C) 2016 SON
// some of this code was written by <cstone@pobox.com> originally; it is in the public domain.
 Developed by Phùng Thái Sơn - 1996- Hà Nam
 https://github.com/NickChungVietNam/HOANG_SA_LIBRARY_LCD5110_NOKIA.git
Supported by Nguyễn Mạnh Hoạch, Phùng Thái Hải.
Mât rất nhiều thời gian và công sức cho bộ thư viện này
Mình đã chú thích khá đầy đủ trong quá trình viết code
Nếu bạn đã vào phần này, hãy bỏ chút thời gian tham khảo (có nhiều cái hay)và phát triển bộ thư viện này nhé.

Up loaded to Arduino.vn 25/11/2016
Đã khắc phục lỗi: tọa độ có thể mang giá trị âm 9:12AM-8/28/2016
12:45 am -1/9/2016 Bổ trợ khả năng in số theo các kiểu biến byte int16_t, uint16_t, long, ulong, float
25/10/2016: viết được kí tự tiếng việt
31/10/2016 get_xy_ellipse và sphere
16/12/2016 hỗ trợ thay đổi kích cỡ font chữ
Thư viện nguồn mở, bạn có thể thay đổi hoặc sử dụng cho mục đích khác, đăng tải ở bất cứ đâu
(có ghi chú )nếu nó hợp pháp và không vi phạm các quy tắc trong cộng đồng nguồn mở!

code phôi được tham khảo từ trang:
This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution




*/

/* $Id: ST7656 HOMEPHONE 11/2016 by Thai Son $ */
#include <avr/pgmspace.h>
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifdef __AVR__
  #include <util/delay.h>
#endif

#ifndef _BV
  #define _BV(x) (1 << (x))
#endif

#include <stdlib.h>

#include "PCD8544_HOANGSA.h"

#include <glcdfont.c>
#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif



// BỘ FONT 5X7


// a 5x7 font table

const extern unsigned char vietnamese_vowel_table[] PROGMEM ;
const extern unsigned char style_number[] PROGMEM ;
const extern unsigned char font[] PROGMEM ;
const extern unsigned char casio_number[] PROGMEM ;
// the memory buffer for the LCD
uint8_t pcd8544_buffer[504] = {};// 504=(84*48 điểm)/8 byte=504 byte


// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
//#define enablePartialUpdate

#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif



static void updateBoundingBox(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax) {

 if(xmin<0){xmin=0;}
   if(ymin<0){ymin=0;}
   if(xmax>X_MAX){xmax=X_MAX;}//X_MAX=83
    if(ymax>Y_MAX){ymax=Y_MAX;}//Y_MAX=47
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}
PCD8544::PCD8544(uint8_t RST,uint8_t CS,uint8_t DC,uint8_t DIN,uint8_t SCLK) {
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = CS;
}

PCD8544::PCD8544(uint8_t RST,uint8_t DC,uint8_t DIN,uint8_t SCLK)  {
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = -1;
}

PCD8544::PCD8544( uint8_t RST, uint8_t CS,uint8_t DC) {
  // -1 for din and sclk specify using hardware SPI
  _din = -1;
  _sclk = -1;
  _dc = DC;
  _rst = RST;
  _cs = CS;
}


////////||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void PCD8544::Bitmap(int16_t x, int16_t y, uint16_t w, uint16_t h,const uint8_t *bitmap, bool color) {
  bool non=!color;
 
    for (uint16_t i=0; i<w; i++ ) {

      for (uint16_t j=0; j<h; j++) {


    if(bitRead( pgm_read_byte(bitmap+i+(j/8)*w),j%8)){// font: địa chỉ của mảng font[] là tên mảng đó

  my_drawpixel(x+i, y+j, color);
      }else{

  my_drawpixel(x+i, y+j, non);
      }
    }
  }

  updateBoundingBox(x, y, x+w, y+h);



}
   //||||||||||||||||||||||||||||||||||||||||||||||
   //|||||||||||||||||||||||
void PCD8544::Plus_Bitmap(int16_t x0, int16_t y0, uint16_t w, uint16_t h,const uint8_t *bitmap , int16_t goc, bool mirror, bool color) {
 // xoay ảnh bitmap trong4 góc 0-90-180-270
int16_t x,y;
bool notcolor=!color;
uint16_t w_1,h_1;
w_1=w-1;
h_1=h-1;
 for (uint16_t i=0; i<w; i++ ) {//con trỏ dich byte
  for (uint16_t j=0; j<h; j++) {// con trỏ dịch bit

 switch(goc){
  case 0:

         
  y=y0+j;//0
     if(mirror){
          
                  x=(x0+w_1)-i; // xoay 0 độ  gương
     }else{

                  x=x0+i;   // xoay 0 độ  thường 
     }
  break;
  case 90:

         
     y=(y0+w_1)-i;//90
     if(mirror){
x=(x0+h_1)-j;// xoay 90 độ  gương
     }else{
x=x0+j;// xoay 90 độ  thường
     }
  break;
  case 180:

          y=(y0+h_1)-j;//180
     if(mirror){
                    x=x0+i; // xoay 180 độ gương

     }else{

                    x=(x0+w_1)-i;// xoay 180 độ thường
     }
  break;
  case 270:

        
  y=y0+i;//270
     if(mirror){
               
      x=x0+j;// xoay 270 độ gương

     }else{
x=(x0+h_1)-j;// xoay 270 độ thường

     }
  break;
default:
         return;  //thoát        
break;

     }// switch

      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
       my_drawpixel(x,y,color);
       }else{
       my_drawpixel(x,y,notcolor);
       }
 }
}
w+=x0;
h+=y0;
if((goc==0)||(goc==90)){
  updateBoundingBox(x0,y0,w,h);
} else{

  updateBoundingBox(x0,y0,h,w);
}
}

////|||||||||||||||||||||||||||||||||||||||||||||||

void  PCD8544::Asc_Char(int16_t x1, int16_t y1,unsigned char c PROGMEM, bool color) {
bool non=!color;
const unsigned char* dress =font+c*5;
  for (byte i =0; i<5; i++ ) {
  for (byte j =0; j<8; j++ ) {

    if(bitRead( pgm_read_byte(dress+i),j)){// font: địa chỉ của mảng font[] là tên mảng đó
   my_drawpixel(x1+i, y1+j,color);
  }else{
 my_drawpixel(x1+i, y1+j,non);
  }
}
}
updateBoundingBox(x1,y1,x1+5,y1+8);
}

///||||||||||||||||||||||||||||||||||\\\\\\\\\\\\\\\\\\


void PCD8544::RunStatus(int16_t x, int16_t y,byte a,uint16_t t,const char *c PROGMEM, bool color) { 
  
 bool non=!color;
t=abs(101-t);
byte b,d,m,w;
uint16_t  i;

i=0;
b=a;
while (c[i+1]!=0){ // dịch kí tự bắt đầu
 

//gặp kí tự cuối cùng thì thoát lặp
  i++;
if (c[i+b]==0){// TIÊN ĐOÁN VỊ TRÍ NGẮT
  
a--;//trừ độ dài hiển thị  đi 1
}// ngược lại , thì giữ nguyên độ dài 
b=a; //đặt lại b
 for ( d= 6; d>0 ; d--) { // dịch hoành độ
 m=x;
 for ( w = i; w <( b+i); w++ ) { // viết tất cả a kí tự
 if (m<LCDWIDTH-6) {
   Asc_Char(m+d , y , c[w] , color);

 m+=6;
}

}
Asc_Char(x, y , 16 , color); // vẽ con trỏs
_delay_ms( t);
Display();

FillRect(x+5,y,a*6,8,non); 
/* ẩn đối tượng bằng hình chữ nhật có màu trắng
*/
  }

}
/* đóng hộp
*/

 }


// ví dụ Asc_String( 5,10,PSTR("HELLO"),BLACK);// chú ý đoạn PSTR""
void PCD8544::Asc_String(int16_t x1, int16_t y1,PGM_CHAR s , bool color) {
int16_t x=x1;
unsigned char c;
  while ((c=pgm_read_byte(s)) != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    // Mời bạn tìm hiểu thêm cách lưu biến tĩnh vào bộ nhớ flash trên trang arduino.vn
    Asc_Char(x, y1, c, color);
    //Serial.print16_tln(c[i]);
    s++;
    x += 6; // 6 pixels wide
    if (x + 6 >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=8;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}
/*
void PCD8544::Asc_String(int16_t x1, int16_t y1, const  unsigned char  *c , bool color) {
int16_t x=x1;
  while (c[0] != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    Asc_Char(x, y1, c[0], color);
    //Serial.print16_tln(c[i]);
    c++;
    x += 6; // 6 pixels wide
    if (x + 6 >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=8;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}
*/

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||


void  PCD8544::draw_single_number(int16_t x1, int16_t y1,byte single_number, byte select_font,bool color) {
//SET_SIZE(select_font);// chọn cỡ chữ theo font
bool non=!color;
const unsigned char *ADRESS PROGMEM;
switch (select_font){
case 1:
ADRESS=casio_number;
break;
case 2:
ADRESS=style_number;
break;
default:
ADRESS=font+240;//48*5 byte đầu tiên của kí tự 0 (số 0 ) trang bảng ascii
break;
}
ADRESS+=single_number*Font_width;
uint16_t  enter;
    for (byte i=0; i<Font_width; i++ ) {
       for (byte j=0; j<Font_high; j++) {
      if( j<8){
        enter=0;
      }else if(j<15){
        enter=Font_column_bitmap;


      }
      if (bitRead(pgm_read_byte(ADRESS + i + enter),j%8)) {
  my_drawpixel(x1+i, y1+j, color);
      }else{
        my_drawpixel(x1+i, y1+j, non);
      }
    }
  }

  updateBoundingBox(x1, y1, x1+Font_width, y1+Font_high);
}

  void PCD8544::Number_Long(int16_t x,int16_t y, long a,byte select_font, bool color){

 SET_SIZE(select_font);// chọn cỡ chữ theo font
if(a==0){

draw_single_number(x,y,0,select_font,color);
return;
}

int16_t denta_x=0;
byte font_w=Font_width+1;
byte du;
long b;
if (a<0){
 // nếu a<0 thì đổi dấu
  a=(-1)*a;

Asc_Char(x,y+Font_width/2,8, color); //  VẼ dấu trừ (-) ở đằng trước

x+=6;//dịch con trỏ sang phải  , bắt đàu ghi số
} 
b=a;
while(b>9){
b/=10;
denta_x+=font_w;
}
x+=denta_x;
while (a>0){
du=a%10;// lấy phần dư hàng đơn vị

draw_single_number(x,y,du,select_font,color);
a=((a-du)/10);// lấy chẵn rồi giảm xuống 10 lần
x-=font_w;// dịch con trỏ sang trái  sau mỗi lần ghi
}// kết thúc a=0

  }


void PCD8544::Number_Ulong(int16_t x, int16_t y,unsigned long a,byte select_font, bool color){

 SET_SIZE(select_font);// chọn cỡ chữ theo font

if(a==0){

draw_single_number(x,y,0,select_font,color);
return;
}
int16_t denta_x=0;
byte font_w=Font_width+1;
byte du;
unsigned long b=a;
b=a;
while(b>9){
b/=10;
denta_x+=font_w;
}
x+=denta_x;
while (a>0){
du=a%10;// lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);
a=((a-du)/10);// lấy chẵn rồi giảm xuống 10 lần
x-=font_w;// dịch con trỏ sang trái  sau mỗi lần ghi
}// kết thúc a=0

}

void PCD8544::Number_Float(int16_t x, int16_t y, float a,byte n, byte select_font,bool color){



 SET_SIZE(select_font);// chọn font
 if(a==0){

draw_single_number(x,y,0,select_font,color);
return;
}
// vẽ phần trước dấu phảy

float sau;// 
unsigned long b1,b2; // để lưu trong tính toán
byte du ;// 1 số dư du

if (a<0){
 // nếu a<0 thì đổi dấu
a=(-1)*a;
Asc_Char(x,y+Font_width/2,8, color); //  VẼ dấu trừ (-) ở đằng trước
x+=6;//dịch con trỏ sang phải  , bắt đàu ghi số
} 

b1=b2=(unsigned long)a;// ép về kiểu số nguyên, ta được số trước dấu phẩy
sau=a-float(b1);//tách lấy số sau dấu phẩy

byte denta_x=0;
byte font_w=Font_width+1;
while(b1>9){
  b1/=10;
  denta_x+=font_w;
}
x+=denta_x;// VẼ TỪ PHẢI SANG TRÁI






do{
if( b2>0){
  
du=b2%10; // lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);

b2=((b2-du)/10); // kết thúc b2=0
}else{
  
draw_single_number(x,y,0,select_font,color);
}
x-=font_w;//dịch con trỏ sang trái sau mỗi lần ghi
}while( b2>0);

// //////////////////kết thúc phần trước dấu phảy
x+=denta_x+2+font_w*2;
//vẽ dấu phẩy cách đằng trước và sau 2 pixel
fillrect(x,y+Font_high-2,2,2,color);
x+=4;
// vẽ phần sau dấu phẩy
float d_sau;
d_sau=sau*float(pow(10,n));// nhân =(10 mũ n) n_max=8
b2=(unsigned long)d_sau;

x+=((n-1)*font_w);//dịch con trỏ sang phải 



byte count_stop=(byte)n;
do{
 
 if(b2>0){

du=b2%10; // lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);

b2=((b2-du)/10); // kết thúc b2=0
 }else{
   
draw_single_number(x,y,0,select_font,color);
 }
x-=font_w;//dịch con trỏ sang trái sau mỗi lần ghi
count_stop--;
}while( b2>0 || count_stop >0);


}



byte PCD8544::tim_thu_tu( char16_t  c){
switch(c){
// nguyên âm thấp   và không mũ(thường)
  case u'\x61':return 0;break;// a .0/6 thương =0 DƯ 0
  case u'\xE0':return 1;break;// à .1/6 thương 0 dư 1
  case u'\xE1':return 2;break;// á
  case u'\x1EA3':return 3;break;//ả
  case u'\xE3':return 4;break;//ã
  case u'\x1EA1':return 5;break;//ạ 5/6 thuong =0 dư 5

  case u'\x65':return 6;break;//e
  case u'\xE8':return 7;break;//è
  case u'\xE9':return 8;break;//é
  case u'\x1EBB':return 9;break;//ẻ
  case u'\x1EBD':return 10;break;//ẽ
  case u'\x1EB9':return 11;break;//ẹ

  case u'i':return 12;break;//i
  case u'\xEC':return 13;break;//ì
  case u'\xED':return 14;break;//í
  case u'\x1EC9':return 15;break;//ỉ
  case u'\x129':return 16;break;//ĩ
  case u'\x1ECB':return 17;break;//ị
 
  case u'o':return 18;break;//o
  case u'\xF2':return 19;break;//ò
  case u'\xF3':return 20;break;//ó
  case u'\x1ECF':return 21;break;//ỏ
  case u'\xF5':return 22;break;//õ
  case u'\x1ECD':return 23;break;//ọ

  case u'u':return 24;break;//u
  case u'\xF9':return 25;break;//ù
  case u'\xFA':return 26;break;//ú
  case u'\x1EE7':return 27;break;//ủ
  case u'\x169':return 28;break;//ũ
  case u'\x1EE5':return 29;break;//ụ

  case u'\x1B0':return 30;break;//ư
  case u'\x1EEB':return 31;break;//ừ
  case u'\x1EE9':return 32;break;//ứ 
  case u'\x1EED':return 33;break;//ử
  case u'\x1EEF':return 34;break;//ữ
  case u'\x1EF1':return 35;break;//ự

  case u'y':return 36;break;//y
  case u'\x1EF3':return 37;break;//ỳ
  case u'\xFD':return 38;break;//ý
  case u'\x1EF7':return 39;break;//ỷ
  case u'\x1EF9':return 40;break;//ỹ
  case u'\x1EF5':return 41;break;//ỵ
// nguyên âm thấp và có mũ
  case u'\x103':return 42;break;//ă 6/6=1 dư 0
  case u'\x1EB1':return 43;break;//ằ 7/6=1 dư 1
  case u'\x1EAF':return 44;break;//ắ
  case u'\x1EB3':return 45;break;//ẳ
  case u'\x1EB5':return 46;break;//ẵ
  case u'\x1EB7':return 47;break;//ặ

  case u'\xE2':return 48;break;//â
  case u'\x1EA7':return 49;break;//ầ
  case u'\x1EA5':return 50;break;//ấ
  case u'\x1EA9':return 51;break;//ẩ
  case u'\x1EAB':return 52;break;//ẫ
  case u'\x1EAD':return 53;break;//ậ
 
  case u'\xEA':return 54;break;//ê
  case u'\x1EC1':return 55;break;//ề
  case u'\x1EBF':return 56;break;//ế
  case u'\x1EC3':return 57;break;//ể
  case u'\x1EC5':return 58;break;//ễ
  case u'\x1EC7':return 59;break;//ệ
 
  case u'\xF4':return 60;break;//ô
  case u'\x1ED3':return 61;break;//ồ
  case u'\x1ED1':return 62;break;//ố
  case u'\x1ED5':return 63;break;//ổ
  case u'\x1ED7':return 64;break;//ỗ
  case u'\x1ED9':return 65;break;//ộ
 
  case u'\x1A1':return 66;break;//ơ
  case u'\x1EDD':return 67;break;//ờ
  case u'\x1EDB':return 68;break;//ớ
  case u'\x1EDF':return 69;break;//ở
  case u'\x1EE1':return 70;break;//ỡ
  case u'\x1EE3':return 71;break;//ợ
 // nguyên âm cao (in hoa)
  case u'\x41':return 72;break;//A
  case u'\xC0':return 73;break;//À
  case u'\xC1':return 74;break;//Á
  case u'\x1EA2':return 75;break;//Ả
  case u'\xC3' :return 76;break;//Ã
  case u'\x1EA0':return 77;break;//Ạ
  
  case u'\x102':return 78;break;//Ă
  case u'\x1EB0':return 79;break;//Ằ
  case u'\x1EAE':return 80;break;//Ắ
  case u'\x1EB2':return 81;break;//Ẳ
  case u'\x1EB4':return 82;break;//Ẵ
  case u'\x1EB6':return 83;break;//Ặ
 
  case u'\xC2':return 84;break;//Â
  case u'\x1EA6':return 85;break;//Ầ
  case u'\x1EA4':return 86;break;//Ấ
  case u'\x1EA8':return 87;break;//Ẩ
  case u'\x1EAA':return 88;break;//Ẫ
  case u'\x1EAC':return 89;break;//Ậ
 
  case u'\x45':return 90;break;//E
  case u'\xC8':return 91;break;//È
  case u'\xC9':return 92;break;//É
  case u'\x1EBA':return 93;break;//Ẻ
  case u'\x1EBC':return 94;break;//Ẽ
  case u'\x1EB8':return 95;break;// Ẹ11/6=1 dư 5

  case u'\xCA':return 96;break;//Ê
  case u'\x1EC0':return 97;break;//Ề
  case u'\x1EBE':return 98;break;//Ế
  case u'\x1EC2':return 99;break;//Ể
  case u'\x1EC4':return 100;break;//Ễ
  case u'\x1EC6':return 101;break;//Ệ
 
  case u'I':return 102;break;//I
  case u'\xCC':return 103;break;//Ì
  case u'\xCD':return 104;break;//Í
  case u'\x1EC8':return 105;break;//Ỉ
  case u'\x128':return 106;break;//Ĩ
  case u'\x1ECA':return 107;break;//Ị
 
  case u'O':return 108;break;//O
  case u'\xD2':return 109;break;//Ò
  case u'\xD3':return 110;break;//Ó
  case u'\x1ECE':return 111;break;//Ỏ
  case u'\xD5':return 112;break;//Õ
  case u'\x1ECC':return 113;break;//Ọ

  case u'\xD4':return 114;break;//Ô
  case u'\x1ED2':return 115;break;//Ồ
  case u'\x1ED0':return 116;break;//Ố
  case u'\x1ED4':return 117;break;//Ổ
  case u'\x1ED6':return 118;break;//Ỗ
  case u'\x1ED8':return 119;break;//Ộ
 
  case u'\x1A0':return 120;break;//Ơ
  case u'\x1EDC':return 121;break;//Ờ
  case u'\x1EDA':return 122;break;//Ớ
  case u'\x1EDE':return 123;break;//Ở
  case u'\x1EE0':return 124;break;//Ỡ
  case u'\x1EE2':return 125;break;//Ợ
 
  case u'U':return 126;break;//U
  case u'\xD9':return 127;break;//Ù
  case u'\xDA':return 128;break;//Ú
  case u'\x1EE6':return 129;break;//Ủ
  case u'\x168':return 130;break;//Ũ
  case u'\x1EE4':return 131;break;//Ụ
 
  case u'\x1AF':return 132;break;//Ư
  case u'\x1EEA':return 133;break;//Ừ
  case u'\x1EE8':return 134;break;//Ứ
  case u'\x1EEC':return 135;break;//Ử
  case u'\x1EEE':return 136;break;//Ữ
  case u'\x1EF0':return 137;break;//Ự
 
  case u'Y':return 138;break;//Y
  case u'\x1EF2':return 139;break;//Ỳ
  case u'\xDD':return 140;break;//Ý
  case u'\x1EF6':return 141;break;//Ỷ
  case u'\x1EF8':return 142;break;//Ỹ
  case u'\x1EF4':return 143;break;//Ỵ
 //phụ âm
  case u'q':return 144; break; //q
  case u'r':return 145; break;//r
  case u't':return 146; break;//t
  case u'p':return 147; break;//p
  case u's':return 148; break;//s
  case u'\x64':return 149; break;//d
  case u'\x111':return 150; break;//đ
  case u'g':return 151; break;//g
  case u'h':return 152; break;//h
  case u'k':return 153; break;//k
  case u'l':return 154; break;//l
  case u'\x78':return 155; break;//x
  case u'\x63':return 156; break;//c
  case u'v':return 157; break;//v
  case u'\x62':return 158; break;//b
  case u'n':return 159; break;//n
  case u'm':return 160; break;//m
  case u'Q':return 161; break;//Q
  case u'R':return 162; break;//R
  case u'T':return 163; break;//T
  case u'P':return 164; break;//P
  case u'S':return 165; break;//S
  case u'\x44':return 166; break;//D
  case u'\x110':return 167; break;//Đ
  case u'G':return 168; break;//G
  case u'H':return 169; break;//H
  case u'K':return 170; break;//K
  case u'L':return 171; break;//L
  case u'\x58':return 172; break;//X
  case u'\x43':return 173; break;//C
  case u'V':return 174; break;//V
  case u'\x42':return 175; break;//B
  case u'N':return 176; break;//N
  case u'M':return 177; break;//M
  default : return 178; break;
}

}
void  PCD8544::Uni_Char(int16_t x1, int16_t y1, char16_t c, bool color){
bool non=!color;
byte  thu_tu;
thu_tu=tim_thu_tu(c);

uint16_t thuong;

thuong=thu_tu/6;// thương để xác định vị trí kí tự
thuong*=6;// nhân 6 để cho ra byte đầu tiên của kí tự đó
uint16_t so_du;
so_du=thu_tu%6;// số dư  để xác định vị trí dấu
so_du=so_du*6+348;// tìm byte
byte dich_bit=0;
if(thu_tu<42){
  // nguyên âm thấp và không mũ
  dich_bit=3;
}
else if(thu_tu<72){
  // nguyên âm thấp và có mũ
  dich_bit=1;
} 
// ngược lại là nguyên âm cao, dich_bit=0;
//348 là byte thứ 348 , nơi bắt dầu bộ dấu
bool read_bit_ki_tu=0; // trả về bit 0 hoặc 1
bool read_bit_dau=0; // trả về bit 0 hoặc 1


uint16_t enter;
uint16_t tim_byte_phu_am;
tim_byte_phu_am=(thu_tu-144)*6+144;
// 144 là thứ tự bắt đầu phụ âm
    for (byte i=0; i<6; i++ ) {
       for (byte j=0; j<15; j++) {
      if( j<8){
      enter=i;
      }else if(j<14){
      enter=384+i;
      }// mỗi hàng có 384 byte, chúng ta có 2 hàng

if(thu_tu<144){// vẽ nguyên âm

read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+ enter+thuong),j%8);//đọc bitmap kí tự
read_bit_dau=bitRead( (pgm_read_byte(vietnamese_vowel_table+so_du+ enter)<<dich_bit),j%8);//đọc bit map dấu

}else{// vẽ phụ âm

read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+tim_byte_phu_am+ enter),j%8);//đọc bitmap kí tự phụ âm
}
//vẽ 

 if(read_bit_ki_tu||read_bit_dau){// j%8 đọc 2 byte 
  my_drawpixel(x1+i, y1+j, color);
      }else{
        my_drawpixel(x1+i, y1+j, non);
      }
    }//for
}//for

updateBoundingBox(x1,y1,x1+6, y1+14);
}


/*

read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+thuong_j),j%8);//đọc bitmap kí tự nguyên âm
read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+thuong*6+i + enter),j%8);//đọc bitmap kí tự
read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+denta_j),j%8);//đọc bitmap kí tự phụ âm
read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+(thu_tu-144)*6+144+i + enter),j%8);//đọc bitmap kí tự
read_bit_dau=bitRead( pgm_read_byte(vietnamese_vowel_table+348+so_du*6+i + enter),j%8);//đọc bit map dấu
read_bit_dau=bitRead( (pgm_read_byte(vietnamese_vowel_table+so_du_j)<<dich_bit),j%8);//đọc bit map dấu

*/



void PCD8544::Uni_String(int16_t x1, int16_t y1,PGM_CHAR16T s , bool color) {
int16_t x=x1;
char16_t c;// kiểu char 16 bít(  2 bytes)
  while ((c=pgm_read_word(s)) != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    // Mời bạn tìm hiểu thêm cách lưu biến tĩnh vào bộ nhớ flash trên trang arduino.vn
    Uni_Char(x, y1, c, color);
    //Serial.print16_tln(c[i]);
    s++;
    x += 7; // 7 pixels wide
    if (x + 7 >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=15;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}


int16_t PCD8544::Keep_Angle(int16_t goc){
  // giữ cho góc trong khoảng 0->360
if(goc<0){
  goc=(-1)*goc;// đổi dấu
 goc=(goc%360);// giữ
  return 360-goc;
  } else{
    return goc%360;
  } 

}

  // mình khá tâm đắc về cái này, hehe
void PCD8544::Find_XY_Elip( int16_t x0, int16_t y0,  int16_t a, int16_t b, int16_t  goc_alpha){


goc_alpha=Keep_Angle(goc_alpha);
long L_rad;
float  F_rad, Fx_tich, Fy_tich;


 L_rad= map( goc_alpha, 0, 360, 0,62838);
 F_rad=((float(L_rad))/10000.0);
 // đổi góc sang radian
 Fx_tich=(a*(float(cos(F_rad))));//
 Fy_tich=(b*(float(sin(F_rad)))); // quỹ đạo của y thuộc sin*b (-b->+b)
 
X_ELLIPSE=x0+int16_t(Fx_tich);
Y_ELLIPSE=y0-int16_t(Fy_tich);
}
int16_t PCD8544::X_Elip() {
return X_ELLIPSE;


}

int16_t PCD8544::Y_Elip() {

  return Y_ELLIPSE;
}

void PCD8544::Find_XY_Sphere( int16_t x0, int16_t y0, int16_t a0, int16_t b0, int16_t goc_alpha, int16_t goc_beta){
  
//b1: tìm góc beta dạng số thực
goc_beta=Keep_Angle(goc_beta);
if((goc_beta>90)&&(goc_beta<270)){goc_alpha+=180;}
long L_rad; // long để lưu số  nguyên lớn hơn 32768
float  beta_rad;
 L_rad= map( goc_beta, 0, 360, 0,62838); // map chỉ trả về số nguyên, đổi 360 độ =2*pi=6,2838 rad
 beta_rad=((float(L_rad))/10000.0); // chia cho 10000.0 dạng số thực
// b2: 
float a,b,y;
a=float(a0);
b=float(b0);
y=float(y0);
// hmax=a-b;
float hmax=float(a0-b0);
double ti_so_h=sin(beta_rad);
double ti_so_ab=abs(cos(beta_rad));

int16_t  y_ellipse, a_ellipse, b_ellipse;
y_ellipse=int16_t(y-hmax*ti_so_h);
a_ellipse=int16_t(ti_so_ab*a);
b_ellipse=int16_t(ti_so_ab*b);

Find_XY_Elip(x0, y_ellipse, a_ellipse, b_ellipse, goc_alpha);
X_SPHERE= X_Elip();
Y_SPHERE=Y_Elip();
}
 
 int16_t PCD8544::X_Sphere( ){
return X_SPHERE;
 }

int16_t PCD8544::Y_Sphere(){
  return Y_SPHERE;
}
// nếu thấy khó hiểu thì bạn có thể tham khảo cái hình minh họa 
// ở trong file đính kèm thư viện này nhé..
// hoàn thiện get_xy: 9:44pm 31/10/2016
// đúng  ngày haloween, người ta có gấu đi chơi tối nay,
// mình thì ngồi nhà hoàn thiện cái code 
// (niềm vui bất tận )__SƠN__
















void PCD8544::Analog( int16_t x, int16_t y, int16_t w, int16_t h, int16_t value, bool color){
// mặc dù value mình đặt là uint16_t, nhưng, value chỉ được trong đoạn 0->250
 int16_t Xi,Yi, v ;
bool non=!color;
w+=x;
h+=y;
v= map (value, 0,500, h, y); //value max =500
DrawPixel(w-1, v, color); // vẽ mồi điểm xuất phát tại cột cuối cùng, không thì hàm sẽ không hoạt động


for(Xi=x; Xi<w; Xi++ ){ // Quets hàng ngang
for (Yi=y ; Yi< h; Yi++){ //Quets hàng dọc
if (GetPixel(Xi, Yi)) {// Kiểm tra điểm đó có được vẽ hay không
  DrawPixel(Xi-1, Yi, color);// vẽ sang bên trái 1 điểm tương tự
  DrawPixel(Xi, Yi, non); // Xóa ddiemr cũ bẵng một điểm có màu trắng
break;//thoát quét hàng dọc ngay lập tức

}


} //quét xong hàng dọc



}// quét xong hàng ngang


 }


















void PCD8544::duong_nam_ngang( long x1, long x2, long y0,bool color){
  if(x1>x2){
    swap(x1,x2);
  }

  int16_t hieu=x2-x1;
  for( int16_t x=x1; x<=x2; x++){
    my_drawpixel(x,y0, color);
  }
  updateBoundingBox(x1,y0,x1+hieu,y0);
}

void PCD8544::duong_thang_dung(long x0, long y1, long y2, bool color){
  if(y1>y2){
    swap(y1,y2);
  }
  int16_t hieu=y2-y1;
  for( int16_t y=y1; y<=y2; y++){
my_drawpixel( x0,y,color);

  }
  updateBoundingBox(x0,y1,x0,y1+hieu);
}


void PCD8544::Plot4EllipsePoint16_ts(int16_t CX,int16_t  CY,int16_t X, int16_t Y, bool color, bool fill)
{
int16_t X1,X2,Y0A,Y0B;
X1=CX-X;
X2=CX+X;
Y0A=CY+Y;
Y0B=CY-Y;
  if(fill)
  { // to fill rather than draw a line, plot between the point16_ts
    duong_nam_ngang(X1, X2,Y0A,color);
    //drawline(CX+X, CY+Y, CX-X, CY+Y, color);
    duong_nam_ngang(X1,X2, Y0B, color); 
    //drawline(CX-X, CY-Y, CX+X, CY-Y, color);
  }
  else
  {
    DrawPixel(X2, Y0A, color); //{point16_t in quadrant 1}

    DrawPixel(X1, Y0A, color); //{point16_t in quadrant 2}

    DrawPixel(X1, Y0B, color); //{point16_t in quadrant 3}

    DrawPixel(X2, Y0B, color); //{point16_t in quadrant 4}

  }
}

void PCD8544::DrawEllipse_private(long CX, long  CY, long XRadius,long YRadius, bool color, bool fill)
{
// ported the algorithm by John Kennedy found at
// http://homepage.smc.edu/kennedy_john/belipse.pdf
//
if((XRadius<1)||(YRadius<1)){
  return;//thoát
} 
  long X, Y;
  long XChange, YChange;
  long EllipseError;
  long TwoASquare,TwoBSquare;
  long StoppingX, StoppingY;
  TwoASquare = 2*XRadius*XRadius;
  TwoBSquare = 2*YRadius*YRadius;
  X = XRadius;
  Y = 0;
  XChange = YRadius*YRadius*(1-2*XRadius);
  YChange = XRadius*XRadius;
  EllipseError = 0;
  StoppingX = TwoBSquare*XRadius;
  StoppingY = 0;

  while ( StoppingX >=StoppingY ) 
  { 
    Plot4EllipsePoint16_ts(CX,CY,X,Y,color, fill);

    Y++;
    StoppingY=StoppingY+ TwoASquare;
    EllipseError = EllipseError+ YChange;
    YChange=YChange+TwoASquare;
    if ((2*EllipseError + XChange) > 0 ) {
      X--;
      StoppingX=StoppingX- TwoBSquare;
      EllipseError=EllipseError+ XChange;
      XChange=XChange+TwoBSquare;
    }
  }

  Y = YRadius;
  X = 0;
  YChange = XRadius*XRadius*(1-2*YRadius);
  XChange = YRadius*YRadius;
  EllipseError = 0;
  StoppingY = TwoASquare*YRadius;
  StoppingX = 0;
  while ( StoppingY >=StoppingX )
  {
    Plot4EllipsePoint16_ts(CX,CY,X,Y,color, fill);



    X++;
    StoppingX=StoppingX + TwoBSquare;
    EllipseError=EllipseError+ XChange;
    XChange=XChange+TwoBSquare;
    if ((2*EllipseError + YChange) > 0 ) {
      Y--;
      StoppingY=StoppingY- TwoASquare;
      EllipseError=EllipseError+ YChange;
      YChange=YChange+TwoASquare;
    }
  }
}


void PCD8544::Elip(long xCenter, long  yCenter, long xRadius,long yRadius, bool color)
{
  DrawEllipse_private(xCenter, yCenter, xRadius,yRadius, color, 0);
}

void PCD8544::FillElip(long xCenter, long  yCenter, long xRadius, long yRadius, bool color)
{
  DrawEllipse_private(xCenter, yCenter, xRadius, yRadius, color, 1);
}

void PCD8544::Tri(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3, bool color){
  DrawLine(x3,y3,x1,y1,color);
  DrawLine(x1,y1,x2,y2,color);
  DrawLine(x2,y2,x3,y3,color);


}


















void PCD8544::FillTri(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3, bool color)
{
int16_t sl,sx1,sx2;
double m1,m2,m3;
  if(y2>y3)
  {
    swap(x2,x3);
    swap(y2,y3);
  }
  if(y1>y2)
  {
    swap(x1,x2);
    swap(y1,y2);
  }
  m1=(double)(x1-x2)/(y1-y2);
  m2=(double)(x2-x3)/(y2-y3);
  m3=(double)(x3-x1)/(y3-y1);
  for(sl=y1;sl<=y2;sl++)
  {
    sx1= m1*(sl-y1)+x1;
    sx2= m3*(sl-y1)+x1;
    if(sx1>sx2)
      swap(sx1,sx2);
    duong_nam_ngang(sx1,sx2,sl,color);
    //drawline(sx1, sl, sx2, sl, color);
  }
  for(sl=y2;sl<=y3;sl++)
  {
    sx1= m2*(sl-y3)+x3;
    sx2= m3*(sl-y1)+x1;
    if(sx1>sx2)
      swap(sx1,sx2);
    duong_nam_ngang(sx1, sx2,sl,color);
    //drawline(sx1, sl, sx2, sl, color);
  }
}









// draw a circle outline
void PCD8544::Circle(int16_t x0,int16_t y0,int16_t r,bool color) {

 if(r<1){return;}//thoát

 int16_t f = 1 - r;
 int16_t ddF_x = 1;
 int16_t ddF_y = -2 * r;
 int16_t x = 0;
 int16_t y = r;

  my_drawpixel(x0, y0+r, color);
  my_drawpixel(x0, y0-r, color);
  my_drawpixel(x0+r, y0, color);
  my_drawpixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  int16_t x0x=x0+x,x0y=x0+y, x0_x=x0-x, x0_y=x0-y;
  int16_t y0x=y0+x, y0y=y0+y, y0_x=y0-x, y0_y=y0-y;
// 1/2 góc phần tư thứ 1-trên
    my_drawpixel(x0x, y0_y, color);
//1/2 góc phần tư thứ 1-dưới
    my_drawpixel(x0y, y0_x, color);
// 1/2 góc phần tư thứ 2-trên
    my_drawpixel(x0_x, y0_y, color);
// 1/2 góc phần tư thứ 2-dưới
    my_drawpixel(x0_y, y0_x, color);
// 1/2 góc phần tư thứ 3-trên
    my_drawpixel(x0_y, y0x, color);
// 1/2 góc phần tư thứ 3-dưới 
    my_drawpixel(x0_x, y0y, color);
// 1/2 góc phần tư thứ 4-trên 
    my_drawpixel(x0y, y0x, color); 
/// 1/2 góc phần tư thứ 4-dưới 
    my_drawpixel(x0x, y0y, color);

    
  }


  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);

}






void PCD8544::FillCircle(int16_t x0,int16_t y0,int16_t r, bool color) {
 if(r<1){return;}//thoát
 int16_t f = 1 - r;
 int16_t ddF_x = 1;
 int16_t ddF_y = -2 * r;
 int16_t x = 0;
 int16_t y = r;

  for (int16_t i=y0-r; i<=y0+r; i++) {
    my_drawpixel(x0, i, color);
  }

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  int16_t y0y=y0+y;
  int16_t y0x=y0+x;
    for (int16_t i=y0-y; i<=y0y; i++) {
      my_drawpixel(x0+x, i, color);
      my_drawpixel(x0-x, i, color);
    
    } 
    for (int16_t i=y0-x; i<=y0x; i++) {
      my_drawpixel(x0+y, i, color);
      my_drawpixel(x0-y, i, color);
    }    
  }

  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);
}


void PCD8544::my_drawpixel(int16_t x,int16_t y, bool color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT)||(x<0)||(y<0)){

    return;
  }
/*
  if(x<0){x=0;}
  if(y<0){y=0;}
  if(x>127){x=127;}
  if(y>63){y=63;}*/
  // x is which column

  if (color) {
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);  
  }
  else{
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8); 
  }


}




// hàm trả về giá trị 0 hoặc 1
//Kiểm tra xem một điểm có tọa độ (x,y) trên màn hình có được vẽ hay không.
//giả sử tô trắng toàn màn hình rồi vẽ 1 điểm lcd.drawpixel(20,20,BLACK)
//đặt biến n=lcd.getpixel(20,20);
//thì n=1.
//Mặc định tất cả các điểm có tọa độ âm, hoặc lớn quá kích cỡ màn hình
//thì n sẽ trả về 0.

// bresenham's algorithm - thx wikpedia

void PCD8544::DrawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,bool color) {
  if(x0==x1){
    duong_thang_dung(x0,y0,y1,color);
    return;
  }
  if(y0==y1){
    duong_nam_ngang(x0,x1,y0,color);
    return;
  }

 int16_t steep = abs(y1 - y0) > abs(x1 - x0);

  

  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }


int16_t dx=0, dy=0;
  dx = x1 - x0;
  dy = abs(y1 - y0);
 int16_t err = dx / 2;
 int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) {
    if (steep) {
      DrawPixel(y0, x0, color);

    } else {
      DrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }


}

// filled rectangle
void PCD8544::FillRect(int16_t x,int16_t y,int16_t w,int16_t h, bool color) {

  // stupidest version - just pixels - but fast with int16_ternal buffer!
  w+=x;
  h+=y;

  for (int16_t i=x; i<w; i++) {
    for (int16_t j=y; j<h; j++) {
      my_drawpixel(i, j, color);
    }
  }

  updateBoundingBox(x, y, w, h); 


}

// draw a rectangle
void PCD8544::Rect(int16_t x,int16_t y,int16_t w,int16_t h, bool color) {
  // stupidest version - just pixels - but fast with int16_ternal buffer!
  w+=x;
  h+=y;
  for (int16_t i=x; i<w; i++) {
    my_drawpixel(i, y, color);
    my_drawpixel(i, h-1, color);
  }
  for (int16_t i=y; i<h; i++) {
    my_drawpixel(x, i, color);
    my_drawpixel(w-1, i, color);
  } 

  updateBoundingBox(x, y, w, y);// cạnh trên
  updateBoundingBox(x, h, w, h);// cạnh dưới
  updateBoundingBox( x,y, x, h);//cạnh trái
  updateBoundingBox(w,y, w, h);// cạnh phải
 
}

// draw a rect have circle corner



void PCD8544::Corner(int16_t x,int16_t y,int16_t w,int16_t h,int16_t r, bool color){ 
// Thủ thuật 
//vẽ 4 cạnh hcn + 4 góc phần tư của hình tròn
//

if( (r>h/2)||(r>w/2)){
  return;// thoát
}
int16_t xr=x+r, xw=x+w, xw_r=x+w-r;
int16_t yr=y+r, yh=y+h, yh_r=y+h-r;
//cạnh trên
  duong_nam_ngang(xr, xw_r,y, color);
// cạnh trái
  duong_thang_dung(x, yr, yh_r,color);
//cạnh dưới
  duong_nam_ngang( xr, xw_r, yh,color);
// cạnh phải
  duong_thang_dung(xw, yr, yh_r,color);
  if(r<1){
    
  return;//thoát
  }
// Bắt đầu vẽ góc
 int16_t f = 1 - r;
 int16_t ddF_a = 1;
 int16_t ddF_b = -2 * r;
 int16_t a = 0;
 int16_t b = r;
  while (a<b) { 
if (f >= 0) {
      b--;
      ddF_b += 2;
      f += ddF_b;
    }
    a++;
    ddF_a += 2;
    f += ddF_a;
  
int16_t xw_ra=xw_r +a, xw_rb=xw_r+b;
 int16_t xr_a=xr-a, xr_b=xr-b;
 int16_t yr_a=yr-a, yr_b=yr-b;
 int16_t yh_ra=yh_r+a, yh_rb=yh_r+b;
// bo góc phải- trên
    my_drawpixel( xw_ra, yr_b, color);
    my_drawpixel(xw_rb, yr_a, color);
// bo góc trái-trên
    my_drawpixel(xr_a, yr_b, color);
    my_drawpixel(xr_b, yr_a, color);
// bo góc trái dưới
    my_drawpixel(xr_b, yh_ra, color);
    my_drawpixel(xr_a, yh_rb, color);
// bo góc phải- dưới 
    my_drawpixel(xw_rb, yh_ra, color); 
    my_drawpixel(xw_ra, yh_rb, color);

    
  }

  updateBoundingBox(x,y,x+w, y+h); //update screen faster
}

void PCD8544::FillCorner(int16_t x,int16_t y,int16_t w,int16_t h,int16_t r, bool color){ 
// đành lười biếng kiểu này
// vẽ 4 hình tròn
  int16_t xw_r=x+w-r, yr=y+r, xr=x+r, yh_r=y+h-r;
  int16_t w_2r=w-2*r, h_2r=h-2*r;
FillCircle(xw_r, yr, r, color);
FillCircle(xr, yr, r, color);
FillCircle(xr, yh_r, r,color);
FillCircle(xw_r, yh_r, r,color);
// vẽ hình chữ nhật chèn vào
FillRect(xr, y, w_2r, h+1, color);
FillRect(x, yr, r, h_2r, color);
FillRect(xw_r, yr, r+1,h_2r,color);

  updateBoundingBox(x,y,x+w, y+h); //update screen faster
// xong



}

///////////////////////////////////////////////////////////////////
byte PCD8544::Pullup_4(byte right_pin,  byte up_pin, byte left_pin, byte down_pin){
// nếu nút dược nhấn thì đọc bằng 0, mặc định =1

if ((digitalRead(right_pin)==1)&&(digitalRead(up_pin)==1)&&(digitalRead(left_pin)==1)&&(digitalRead(down_pin)==1)){
  return 0; // 0 nút nào được nhấn
} 
else{//có 1 hoặc 2 nút được nhấn 
    if(digitalRead(right_pin)==0){//  right được nhấn
            if(digitalRead(up_pin)==0){ return 20; }// (1*2)*10---nhấn cả up
            if(digitalRead(left_pin)==0){return 30;}// (1*3)*10---nhán cả left
            if(digitalRead(down_pin)==0){ return 40;}//(1*4)*10---nhấn cả down
        return 1;//chỉ mỗi nút right
    }//nếu chưa thỏa mãn thì duyệt tiếp
    if(digitalRead(up_pin)==0){//nếu up được nhấn 
             if(digitalRead(left_pin)==0){ return 60;}// (2*3)*10---nhấn cả left
             if(digitalRead(down_pin)==0){return 80;}//(2*4)*10---nhấn cả down
         return 2;//chỉ mỗi nút up
     }// nếu chưa thỏa mãn thì duyệt tiếp
    if(digitalRead(left_pin)==0){// left được nhấn
             if (digitalRead(down_pin)==0){return 120;}// (3*4)*100---nhấn cả down
         return 3; //chỉ mỗi left

    }
// nếu chưa thỏa mãn thì đây phải là down
    return 4;// chỉ mỗi nút down
}//đóng else

}// đóng 4button_pullup


// the most basic function, set a single pixel
void PCD8544::DrawPixel(int16_t x,int16_t y, bool color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT)||(x<0)||(y<0)){

    return;
  }
/*
  if(x<0){x=0;}
  if(y<0){y=0;}
  if(x>127){x=127;}
  if(y>63){y=63;}*/
  // x is which column

  if (color) 
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);  
  else
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8); 

  updateBoundingBox(x,y,x,y);

}

// the most basic function, get a single pixel
bool PCD8544::GetPixel(int16_t x, int16_t y) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    {return 0;
}else{
  return (pcd8544_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;  
}
}




void PCD8544::ON(void){

  if (isHardwareSPI()) {
    // Setup hardware SPI.
    SPI.begin();
    SPI.setClockDivider(PCD8544_SPI_CLOCK_DIV);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
  }
  else {
    // Setup software SPI.

    // Set software SPI specific pin outputs.
    pinMode(_din, OUTPUT);
    pinMode(_sclk, OUTPUT);

    // Set software SPI ports and masks.
    clkport     = portOutputRegister(digitalPinToPort(_sclk));
    clkpinmask  = digitalPinToBitMask(_sclk);
    mosiport    = portOutputRegister(digitalPinToPort(_din));
    mosipinmask = digitalPinToBitMask(_din);
  }

  // Set common pin outputs.
  pinMode(_dc, OUTPUT);
  if (_rst > 0)
      pinMode(_rst, OUTPUT);
  if (_cs > 0)
      pinMode(_cs, OUTPUT);

  // toggle RST low to reset
  if (_rst > 0) {
    digitalWrite(_rst, LOW);
    delay(500);
    digitalWrite(_rst, HIGH);
  }


  SET(40,0,0,0,4);
  clear();

}

void PCD8544::SET(uint8_t contrast, bool negative, bool rotation, bool mirror,uint8_t bias) {


  // get into the EXTENDED mode!
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

  // LCD bias select (4 is optimal?)
  command(PCD8544_SETBIAS | bias);// cài ??/



// cài tương phản
  // set VOP
  if (contrast > 0x7f)
    contrast = 0x7f;

  command( PCD8544_SETVOP | contrast); // Experimentally determined

  command(PCD8544_FUNCTIONSET);





  // Set display to Normal
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);




if (negative){ command(CMD_SET_DISP_REVERSE );
} else
{  command(CMD_SET_DISP_NORMAL);
} //âm bản

/*
if (rotation){ 
command(0x9);
} else
{ 
 command(CMD_SET_COM_NORMAL );
 }//quay ngang
 
if (mirror){ 
command(CMD_SET_ADC_REVERSE );
} else
{  command( CMD_SET_ADC_NORMAL );
 }// gương
*/
  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  // Push out pcd8544_buffer to the Display (will show the AFI logo)
  display();
}




































inline void PCD8544::spiWrite(uint8_t d) {
  if (isHardwareSPI()) {
    // Hardware SPI write.
    SPI.transfer(d);
  }
  else {
    // Software SPI write with bit banging.
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      *clkport &= ~clkpinmask;
      if(d & bit) *mosiport |=  mosipinmask;
      else        *mosiport &= ~mosipinmask;
      *clkport |=  clkpinmask;
    }
  }
}

bool PCD8544::isHardwareSPI() {
  return (_din == -1 && _sclk == -1);
}

void PCD8544::command(uint8_t c) {
  digitalWrite(_dc, LOW);
  if (_cs > 0)
    digitalWrite(_cs, LOW);
  spiWrite(c);
  if (_cs > 0)
    digitalWrite(_cs, HIGH);
}

void PCD8544::data(uint8_t c) {
  digitalWrite(_dc, HIGH);
  if (_cs > 0)
    digitalWrite(_cs, LOW);
  spiWrite(c);
  if (_cs > 0)
    digitalWrite(_cs, HIGH);
}



void PCD8544::Display(void) {
  uint8_t col, maxcol, p;
  
  for(p = 0; p < 6; p++) {
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    command(PCD8544_SETYADDR | p);


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

    command(PCD8544_SETXADDR | col);

    digitalWrite(_dc, HIGH);
    if (_cs > 0)
      digitalWrite(_cs, LOW);
    for(; col <= maxcol; col++) {
      spiWrite(pcd8544_buffer[(LCDWIDTH*p)+col]);
    }
    if (_cs > 0)
      digitalWrite(_cs, HIGH);

  }

  command(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif

}

void PCD8544::Clear(int16_t x0, int16_t y0, int16_t width, int16_t high){
  width+=x0;
  high+=y0;
  for(int16_t x=x0;x<width; x++){
    for(int16_t y=y0; y<high; y++){
      my_drawpixel(x,y,WHITE);
    }
  }
  updateBoundingBox(x0,y0,width,high);
}

// clear everything
void PCD8544::Clear(void) {
  memset(pcd8544_buffer, 0, 504);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
}


  void PCD8544::phong_to(int16_t x0, int16_t y0, uint16_t w0, uint16_t h0, uint8_t he_so_phong_to, bool color){
    // hàm này có nhiệm vụ phóng to một vùng ảnh lên gấp 2, gấp 4, gấp 8 ..
// đầu 


// b0:cấp phát một mảng để copy vùng ảnh 
 bool copy_buffer[w0][h0];
//bool *hh=new bool[h0];


    //b1: copy vùng ảnh tọa độ x0+w,y0+w kích thước 1xh0
    for( uint16_t w=0; w<w0; w++){
 for( uint16_t h=0; h< h0; h++){
  copy_buffer[w][h]=getpixel( x0+w, y0+h);// kiểm tra xem điểm có được vẽ hay không

    }
  }


//b1.1: xóa vùng ảnh cũ
  // fix : 5/4/2018 : thái sơn
int16_t w0_x0=(int16_t)( w0+x0);
int16_t  h0_y0=(int16_t )(h0+y0);
  for(int16_t x=(int16_t)x0;x<w0_x0; x++){
    for(int16_t y=(int16_t)y0; y< h0_y0; y++){
      my_drawpixel(x,y,WHITE);
    }
  }


    //b2: vẽ lại vùng ảnh với tỷ lệ 

    for( uint16_t w=0; w<w0; w++){
 for( uint16_t h=0; h< h0; h++){



if(copy_buffer[w][h]==1){


// phóng to điểm ảnh thành hình vuông kích thước he_so_phong_to*he_so_phong_to
for(byte i=0; i< he_so_phong_to; i++){

for(byte j=0; j< he_so_phong_to; j++){
my_drawpixel( x0+w*he_so_phong_to+i,y0+h*he_so_phong_to+j, color);

  
}

}


}


}}



  updateBoundingBox(x0, y0, x0+w0*he_so_phong_to, y0+h0*he_so_phong_to);

  }



void  PCD8544::Asc_Char(int16_t x1, int16_t y1,unsigned char c PROGMEM,byte he_so_phong_to,  bool color) {
Asc_Char( x1,  y1, c , color);// vẽ

//rồi phóng to
if(he_so_phong_to>1){
phong_to(x1,y1, 5,7, he_so_phong_to, color );// kích thước của font ascii trong thư viejn này là 5x7 cho mỗi kí tự
}
}



void PCD8544::Asc_String(int16_t x1, int16_t y1,PGM_CHAR s ,byte he_so_phong_to, bool color) {


if(he_so_phong_to==1){
Asc_String( x1,  y1, s ,color) ;
return;///thoát

}

int16_t x=x1;
unsigned char c;
  while ((c=pgm_read_byte(s)) != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    // Mời bạn tìm hiểu thêm cách lưu biến tĩnh vào bộ nhớ flash trên trang arduino.vn
    Asc_Char(x, y1, c,he_so_phong_to, color);
    //Serial.print16_tln(c[i]);
    s++;
    x += 6*he_so_phong_to; // 6 pixels wide
    if (x + 6*he_so_phong_to >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=8*he_so_phong_to;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}

  void PCD8544::Number_Long(int16_t x,int16_t y, long a,byte select_font, byte he_so_phong_to, bool color){
if(  he_so_phong_to==1){
Number_Long(x, y, a,select_font,color);
  return;// thoát
}
 SET_SIZE(select_font);// chọn font
if(a==0){

draw_single_number(x,y,0,select_font,color);
phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);
return;
}

int16_t denta_x=0;
byte font_w=Font_width+1;
byte du;
long b;
if (a<0){
 // nếu a<0 thì đổi dấu
  a=(-1)*a;

Asc_Char(x,y+Font_width/2,8, color); //  VẼ dấu trừ (-) ở đằng trước

phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);
x+=6*he_so_phong_to;//dịch con trỏ sang phải  , bắt đàu ghi số
} 
b=a;
while(b>9){
b/=10;
denta_x+=(font_w*he_so_phong_to);
}
x+=denta_x;
while (a>0){
du=a%10;// lấy phần dư hàng đơn vị

draw_single_number(x,y,du,select_font,color);

phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);
a=((a-du)/10);// lấy chẵn rồi giảm xuống 10 lần
x-=(font_w*he_so_phong_to);// dịch con trỏ sang trái  sau mỗi lần ghi
}// kết thúc a=0

  }


void PCD8544::Number_Ulong(int16_t x, int16_t y,unsigned long a,byte select_font,byte he_so_phong_to, bool color){
if(  he_so_phong_to==1){
Number_Ulong(x, y, a, select_font,color);
  return;// thoát
}
 SET_SIZE(select_font);// chọn cỡ chữ theo font

if(a==0){

draw_single_number(x,y,0,select_font,color);

phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);
return;
}
int16_t denta_x=0;
byte font_w=Font_width+1;
byte du;
unsigned long b=a;
b=a;
while(b>9){
b/=10;
denta_x+=font_w*he_so_phong_to;
}
x+=denta_x;
while (a>0){
du=a%10;// lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);

phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);
a=((a-du)/10);// lấy chẵn rồi giảm xuống 10 lần
x-=font_w*he_so_phong_to;// dịch con trỏ sang trái  sau mỗi lần ghi
}// kết thúc a=0

}

void PCD8544::Number_Float(int16_t x, int16_t y, float a,byte n, byte select_font,byte 
 he_so_phong_to, bool color){


if(  he_so_phong_to==1){
Number_Float(x,y, a,n, select_font,color);
  return;// thoát
}

 SET_SIZE(select_font);// chọn font
 if(a==0){

draw_single_number(x,y,0,select_font,color);

phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);
return;
}
// vẽ phần trước dấu phảy

float sau;// 
unsigned long b1,b2; // để lưu trong tính toán
byte du ;// 1 số dư du

if (a<0){
 // nếu a<0 thì đổi dấu
a=(-1)*a;
Asc_Char(x,y+Font_width/2,8, color); //  VẼ dấu trừ (-) ở đằng trước

phong_to( x,y+Font_width/2, Font_width, Font_high, he_so_phong_to,color);
x+=6*he_so_phong_to;//dịch con trỏ sang phải  , bắt đàu ghi số
} 

b1=b2=(unsigned long)a;// ép về kiểu số nguyên, ta được số trước dấu phẩy
sau=a-float(b1);//tách lấy số sau dấu phẩy

byte denta_x=0;
byte font_w=Font_width+1;
while(b1>9){
  b1/=10;
  denta_x+=font_w*he_so_phong_to;
}
x+=denta_x;// VẼ TỪ PHẢI SANG TRÁI





do{
if( b2>0){
  
du=b2%10; // lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);

b2=((b2-du)/10); // kết thúc b2=0
}else{
  
draw_single_number(x,y,0,select_font,color);
}

phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);

x-=font_w*he_so_phong_to;//dịch con trỏ sang trái sau mỗi lần ghi
}while( b2>0);





// //////////////////kết thúc phần trước dấu phảy
x+=denta_x+(2+font_w*2)*he_so_phong_to;
//vẽ dấu phẩy cách đằng trước và sau 2 pixel
fillrect(x,y+(Font_high-2)*he_so_phong_to,2,2,color);

phong_to( x,y+(Font_high-2)*he_so_phong_to, 2, 2, he_so_phong_to,color);
x+=4*he_so_phong_to;
// vẽ phần sau dấu phẩy
float d_sau;
d_sau=sau*float(pow(10,n));// nhân =(10 mũ n) n_max=8
b2=(unsigned long)d_sau;

x+=((n-1)*font_w)*he_so_phong_to;//dịch con trỏ sang phải 






byte count_stop=n;// số ký tự hiển thị sau dấu phảy
do{
 
 if(b2>0){

du=b2%10; // lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);

b2=((b2-du)/10); // kết thúc b2=0
 }else{
   
draw_single_number(x,y,0,select_font,color);
 }

phong_to( x,y, Font_width, Font_high, he_so_phong_to,color);
x-=font_w*he_so_phong_to;//dịch con trỏ sang trái sau mỗi lần ghi
count_stop--;
}while( b2>0 || count_stop >0);






}




void  PCD8544::Uni_Char(int16_t x1, int16_t y1, char16_t c, byte he_so_phong_to, bool color){ 

Uni_Char( x1,  y1,  c,  color);// vẽ trước
if(he_so_phong_to>1){
phong_to( x1,y1,6,15,he_so_phong_to, color );// 
// MÕI KÍ TỰ CÓ KÍCH THƯỚC 6x15 pixel
}
}


void PCD8544::Uni_String(int16_t x1, int16_t y1,PGM_CHAR16T s , byte he_so_phong_to ,bool color) {
if(he_so_phong_to==1){
Uni_String( x1,  y1, s ,color) ;
return;///thoát

}

int16_t x=x1;
char16_t c;// kiểu char 16 bít(  2 bytes)
  while ((c=pgm_read_word(s)) != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    // Mời bạn tìm hiểu thêm cách lưu biến tĩnh vào bộ nhớ flash trên trang arduino.vn
    Uni_Char(x, y1, c, he_so_phong_to,  color);
    //Serial.print16_tln(c[i]);
    s++;
    x += 7*he_so_phong_to; // 7 pixels wide
    if (x + 7*he_so_phong_to >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=15*he_so_phong_to;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}



/*
// this doesnt touch the buffer, just clears the display RAM - might be handy
void PCD8544::clearDisplay(void) {
  
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {

    PCD8544_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      //uart_putw_dec(c);
      //uart_putchar(' ');
      PCD8544_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      PCD8544_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      PCD8544_data(0x0);
    }     
    }

}

*/
