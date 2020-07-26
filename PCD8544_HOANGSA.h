//https://github.com/NickChungVietNam/HOANG_SA_LIBRARY_LCD5110_NOKIA.git
#ifndef _ADAFRUIT_PCD8544_H
#define _ADAFRUIT_PCD8544_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
#endif

#include <SPI.h>

#if  defined(__SAM3X8E__) || defined(ARDUINO_ARCH_SAMD)
  typedef volatile RwReg PortReg;
  typedef uint32_t PortMask;
#else
  typedef volatile uint8_t PortReg;
  typedef uint8_t PortMask;
#endif




#define swap(a, b) {int t = a; a = b; b = t; }
#define DELETE 0
#define MIRROR 1
#define NO_MIRROR 0
#define CLEAR 0
#define BLACK 1
#define WHITE 0
#define black 1
#define  white 0
#define LCDWIDTH 84
#define LCDHEIGHT 48
#define X_MAX 83
#define Y_MAX 47


//lựa chọn font chữ số
#define ASCII_NUMBER 0
#define CASIO_NUMBER 1
#define STYLE_NUMBER 2
//
#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF
/** tắt - bật màn hình
*/




///////////ĐỔI TÊN HÀM THÀNH VIẾT HOA
 #define allpixel AllPixel  
 #define   drawline DrawLine 
 #define drawpixel DrawPixel     
 #define getpixel GetPixel    
 #define  fillcircle FillCircle   
 #define circle Circle   
 #define corner Corner  
 #define fillcorner FillCorner 
 #define rect Rect   
 #define  fillrect FillRect   
#define  tri Tri 
 #define  filltri FillTri   
#define  elip Elip  
#define fillelip FillElip  
 #define bitmap Bitmap  
#define  plus_bitmap  Plus_Bitmap   
 #define asc_char Asc_Char  
#define asc_string Asc_String  
 #define uni_char Uni_Char  
#define uni_string Uni_String 
#define runstatus RunStatus 
 #define number_long Number_Long  
#define number_ulong  Number_Ulong 
#define number_float Number_Float  
#define  keep_angle Keep_Angle  
#define  find_xy_elip Find_XY_Elip  
#define   x_elip X_Elip  
#define  y_elip Y_Elip  
#define  find_xy_sphere Find_XY_Sphere 
#define  x_sphere X_Sphere 
#define  y_sphere Y_Sphere  
#define analog Analog 
#define pullup_4 Pullup_4 

#define clear Clear
#define display Display





#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

#define CMD_SET_DISP_NORMAL 0xC
/** hiển thị bình thường -không âm bản
*/
#define CMD_SET_DISP_REVERSE 0xD
/** hiển thị âm bản- màu sắc của tất cả sẽ được chuyển  ngược lại đen<>trắng
*/

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

// Default to max SPI clock speed for PCD8544 of 4 mhz (16mhz / 4) for normal Arduinos.
// This can be modified to change the clock speed if necessary (like for supporting other hardware).
#define PCD8544_SPI_CLOCK_DIV SPI_CLOCK_DIV4























class PCD8544 {
 public:
  // Software SPI with explicit CS pin.
  PCD8544(uint8_t RST,uint8_t CS,uint8_t DC,uint8_t DIN,uint8_t SCLK);
  // Software SPI with CS tied to ground.  Saves a pin but other pins can't be shared with other hardware.
  PCD8544(uint8_t RST,uint8_t DC,uint8_t DIN,uint8_t SCLK);
  // Hardware SPI based on hardware controlled SCK (SCLK) and MOSI (DIN) pins. CS is still controlled by any IO pin.
  // NOTE: MISO and SS will be set as an input and output respectively, so be careful sharing those pins!
  PCD8544(uint8_t RST, uint8_t CS,uint8_t DC);
   void ON(void);
  void SET(uint8_t contrast,bool negative, bool rotation, bool mirror, uint8_t bias);
  
  void command(uint8_t c);
  void data(uint8_t c);
  
  void Clear(void); 
  void Display();
  void Clear(int16_t x0, int16_t y0, int16_t width, int16_t high);
  void DrawPixel(int16_t  x, int16_t y, bool color);
  bool GetPixel(int16_t  x, int16_t y);











void AllPixel( bool val);

  void Circle(int16_t x0,int16_t y0, int16_t r, bool color);

  void FillCircle(int16_t x0,int16_t y0, int16_t r, bool color);


void Corner( int16_t x,int16_t y,int16_t w,int16_t h, int16_t r, bool color);
void FillCorner( int16_t x,int16_t y, int16_t w, int16_t h, int16_t r, bool color);
/* corner vẽ hình chữ nhật có góc bo tròn
vd: corner( 5,7,40, 40, 5, BLACK);

x=5: tọa độ (ngang )của đỉnh ( góc trái trên cùng) tính từ trái sang.
y=7: tọa độ (trục đứng )của đỉnh ( góc trái trên cùng) tính từ trên xuống 
Note! : Chúng ta không được nhầm lẫn với trục tung trục hoành trong hình học phổ thông.
w=40: chiều dài hình chữ nhật.
h= 40, chiều cao hỉnh chữ nhật.
( đây là hình vuông)
r=5: bán kính của đường tròn bo góc .
Hãy thử vẽ với ví dụ sau:
drawconer(5,7, 40,40,20,BLACK);
Kết quả: hình tròn!
*/
  void FillRect(int16_t x,int16_t y, int16_t w, int16_t h, bool color);
  void Rect(int16_t x,int16_t y, int16_t w, int16_t h, bool color);
  void DrawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,  bool color);

  void Bitmap(int16_t x, int16_t y,uint16_t w, uint16_t h,  const  uint8_t *bitmap ,   bool color);
/* vẽ một ảnh bitmap
x,y : tọa độ góc trái của ảnh
*bitmap: tên của ảnh ( được đặt trong tệp bmps.h)
w,h: cỡ của ảnh sẽ hiển thị
Note! Phải khai báo đúng kích cỡ của ảnh trong file hex, nếu không ảnh sẽ bị dịch lỗi.
Xem thêm trong phần ví dụ.
*/
void Plus_Bitmap(int16_t x0, int16_t y0, uint16_t w, uint16_t h,const uint8_t *bitmap , int16_t goc, bool mirror, bool color);

  //void drawchar(byte x1, byte line, char c);

//chuẩn mỹ
  void Asc_Char(int16_t x1, int16_t y1,unsigned char c PROGMEM, bool color);
typedef unsigned char prog_char PROGMEM;
#ifndef PGM_CHAR
#define PGM_CHAR const prog_char *
#endif

//#define ss ({})

#define asc(s) ( {const static char __c[] PROGMEM = (s);&__c[0];})

#define Asc(s) ( {const static char __c[] PROGMEM = (s);&__c[0];})

void Asc_String(int16_t x1, int16_t y1,PGM_CHAR s , bool color);


// chuẩn quốc tế và tiếng việt thuộc chuẩn này


void  Uni_Char(int16_t x1, int16_t y1, char16_t c, bool color);

// mình đã trích đoạn này trong tệp pgmspace.h và có chỉnh sửa
typedef char16_t prog_char16_t PROGMEM;
#ifndef PGM_CHAR16T
#define PGM_CHAR16T const prog_char16_t *
#endif
#define Uni(s) ({const static char16_t __c[] PROGMEM= (s); &__c[0];})
#define uni(s) ({const static char16_t __c[] PROGMEM= (s); &__c[0];})

void Uni_String(int16_t x1, int16_t y1,PGM_CHAR16T s , bool color);
// hoàn thiện font việt 25/10/2016
//phùng thái sơn (thuật toán vẫn tốn khoảng 5kb nền, hãy cải thiện thuật toán nếu có thể)
 // viết một chữ số kiểu byte

  void Asc_Char(int16_t x1, int16_t y1,unsigned char c PROGMEM,byte he_so_phong_to, bool color);
 void Asc_String(int16_t x1, int16_t y1,PGM_CHAR s , byte he_so_phong_to, bool color);
 void  Uni_Char(int16_t x1, int16_t y1, char16_t c, byte he_so_phong_to, bool color);
 void Uni_String(int16_t x1, int16_t y1,PGM_CHAR16T s , byte he_so_phong_to,  bool color);
  void Number_Long(int16_t x, int16_t y,long a,byte select_font, byte he_so_phong_to, bool color);
 void Number_Ulong(int16_t x, int16_t y,unsigned long a,byte select_font, byte he_so_phong_to, bool color);
 void Number_Float(int16_t x, int16_t y, float a,byte n, byte select_font, byte he_so_phong_to, bool color);




/* drawchar - viết một kí tự trong bảng ASCII mở rộng 
unsigned char là kiểu dữ liệu kí tự, có miền giá trị 0 đến 255
nếu dùng số:
 lcd.drawchar( 2,5, 65,BLACK);
Hoặc viết 
lcd.drawchar (2,5, 'a');
lcd.display();
delay(10000);
>> chữ a

*/

void RunStatus( int16_t x, int16_t y, byte a,uint16_t t, const char *c PROGMEM, bool color) ;

/** runstatus- string- chuỗi chữ chạy ví dụ:
x1,y tọa độ con trỏ
a=10: số cột hiển thị- mỗi cột tương ứng với một chữ sẽ được hiển thị, a từ 0 đến 22.
t=12: tốc độ chạy chữ, t từ 1 đến 100 ( speed min- max).
cuối cùng là chuỗi kí tự *c, color màu
*/
// mình đã trích đoạn này trong tệp pgmspace.h và có chỉnh sửa
  void Number_Long(int16_t x, int16_t y,long a,byte select_font, bool color);
  // in ra một số kiểu dữ liệu long
  // x, line : tọa đọ con trỏ
  // a biến vào kiểu long

void Number_Ulong(int16_t x, int16_t y,unsigned long a,byte select_font, bool color);
//biến kiểu unsigned long -nguyên dương

void Number_Float(int16_t x, int16_t y, float a,byte n, byte select_font,bool color);
// biến vào kiểu float 
// n là số chữ số muốn láy sau dấu phẩy
//float số dương lớn nhất là 999999,9
//float dương bé nhất là 0,000001

void  draw_single_number(int16_t x1, int16_t y1, byte single_number, byte select_font,bool color);
 /*
 Font_style_select:
 ASCII_NUMBER hoặc 0
 CASIO_NUMBER hoặc 1
 STYLE_NUMBER hoặc 2
*/

//hoàn thiện draw_number 12:25 AM-1/9/2016
  // Phùng Thái Sơn 

/* drawstring viết một chuỗi kí tự
x : tọa độ ngang của con trỏ. x từ 0-128
line: dòng bắt đầu . line từ 0-7
a: số chữ hiển thị trên 1 dòng
*/
  void FillTri (int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3, bool color);
/* vẽ tam giác với tọa độ 3 đỉnh
*/

void Tri(int16_t x1,int16_t y1,int16_t x2,int16_t y2 ,int16_t x3,int16_t y3, bool color);


/* vẽ tập hợp điểm ellipse thử nghiermj */

/* thử hàm get_pixel_ellipse
*/

int16_t Keep_Angle(int16_t goc);
  // luôn giữ cho góc nhập vào (độ) nằm trong khoảng 0->360 độ

void Find_XY_Elip(int16_t x0, int16_t y0,  int16_t a,  int16_t b,int16_t goc_alpha) ;
int16_t X_Elip() ;
  
int16_t Y_Elip();
/* Lấy tọa độ điểm rên mặt cầu
*/

void Find_XY_Sphere( int16_t x0, int16_t y0, int16_t a0, int16_t b0, int16_t goc_alpha, int16_t goc_beta);
  
int16_t X_Sphere();

int16_t Y_Sphere();
/* Vẽ ellipse 
*/







void Analog( int16_t x, int16_t y, int16_t w, int16_t h,  int16_t value,bool color);







void DrawEllipse_private(long CX, long  CY, long XRadius,long YRadius, bool color, bool fill);
void Plot4EllipsePoint16_ts(int16_t  CX,int16_t   CY, int16_t X, int16_t  Y, bool color, bool fill);

void FillElip( long xCenter, long  yCenter,  long xRadius, long  yRadius, bool color);

void Elip(long xCenter, long  yCenter, long   xRadius,  long  yRadius, bool color);

/* vẽ elip
vi du: 
lcd.drawellipse(60, 30, 10, 20, BLACK);
lcd.fillellipse(60, 30, 10, 20, BLACK);
60,60 tọa độ x, y của tâm elip
xRadius=10: bán kính trục ngang
yRaadius=20: bán kính trục đứng

*/


byte Pullup_4(byte right_pin,  byte up_pin, byte left_pin, byte down_pin);
/*
cài dặt nối Pullup nội trở trước khi sử dụng chức năng này
4 biến pin vào nhé

=0 nếu không có nút nào được nhấn
=1 right
=2 up
=3 left 
=4 down
10*(1*2)=20 right+up
10*(1*3)=30 right+left
10*(1*4)=40 right+down
10*(2*3)=60 up+left
10*(2*4)=80 up+down
10*(3*4)=120 left + down

_______[2]
___[3]_____[1]
_______[4]



 */

///////// giả lập serial

//10/11/2016 hoàn thiện bởi thái sơn
//giả lập serial















 private:
  int8_t _din, _sclk, _dc, _rst, _cs;
  volatile PortReg  *mosiport, *clkport;
  PortMask mosipinmask, clkpinmask;


  void spiWrite(uint8_t c);
  bool isHardwareSPI();
private:

  void phong_to(int16_t x0, int16_t y0, uint16_t w0, uint16_t h0, uint8_t he_so_phong_to, bool color);
 
  void my_drawpixel(int16_t x, int16_t y, bool color);
  byte tim_thu_tu( char16_t  c);
int16_t Font_width, Font_high,Font_column_bitmap;// cài kích thước cho font được chọn
 
 byte SERIAL_FONT;
int16_t X_SPHERE,Y_SPHERE,X_ELLIPSE,Y_ELLIPSE;

void duong_nam_ngang( long x1, long x2, long y0,bool color);

  //vẽ đường thẳng nối điểm A(X1,Y0) và B(X2,Y0) với x1<=x2 
void duong_thang_dung(long x0, long y1, long y2, bool color);

// Font_column_bitmap ảnh bitmap mỗi hàng có bao nhiêu cột(chiều dài)
// font denta là độ dịch bytes so với địa chỉ của font đó
void SET_SIZE ( byte select_font){
switch(select_font){
case CASIO_NUMBER:
Font_width=12;
Font_high=16;
Font_column_bitmap=120;//12*10
break;

case STYLE_NUMBER:
Font_width=16;
Font_high=16;
Font_column_bitmap=160;//16*10
break;
default:
//ascii
Font_width=5;
Font_high=7;
break;

}//switch
  }//SET_SIZE
};

#endif
