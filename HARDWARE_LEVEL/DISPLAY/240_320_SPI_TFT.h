
#define use_TFT
#define USER_SETUP_LOADED
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT
#define ILI9341_DRIVER
#define RotateScreen 1
#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

#include <TFT_eSPI.h>
#include "U8g2_for_TFT_eSPI.h"


struct screen_font {
  const uint8_t *name;
  int basis;
  int breite;
  int hoehe;
} screen_fonts[] = { { u8g2_font_inb24_mf, 24, 21, 38 },      // SETUP - ok
                     { u8g2_font_inb27_mf, 27, 23, 43 },      // STANDARD - ok
//                     { u8g2_font_inb16_mf, 16, 14, 26 },    // Standard mit ausgeschriebem "Autostart"
                     { u8g2_font_inb49_mf, 40, 40, 75 },      // Schriften Warnung - ok
                     { u8g2_font_inb49_mf, 49, 41, 52 },      // Schrift Gewicht - ok
                     { u8g2_font_open_iconic_all_6x_t, 48, 48, 48 } };    

struct  {
  int breite;
  int hoehe;
  int font;
  int offset_x;
  int offset_y;
} screen_size = { 300, 240, 0, 15, 0 };   // 290 (320-2*15) wegen offset_y, 15 zeilen rechts/links leer!
