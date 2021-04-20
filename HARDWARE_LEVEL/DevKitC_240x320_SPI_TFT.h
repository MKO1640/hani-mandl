/* ****************** HARDWARELEVEL = 4 ***************
   ****       ESP32 DevKit_C mit externem Display   ***
   ****           SCREEN_ILI9341_SPI_240x320        *** 
   ****************************************************
*/
#define use_TFT                                 // Hanimandl sagen das er die e-SPI Libs verwenden soll
#define Logo1
#define RotateScreen 1
#define OffsetX 0
#define OffsetY 0
// Pin Definitionen der Hardware
const int vext_ctrl_pin        = -1;
// Taster 
const int button_start_pin     = 26;  
const int button_start_vcc_pin = -1;  // <- Vcc Pin = -1 wenn nicht benötigt
const int button_stop_pin      = 25;
const int button_stop_vcc_pin  = -1;  // <- Vcc Pin = -1 wenn nicht benötigt
// Rotary Encoder
const int outputA  = 34;
const int outputB  = 35;
const int outputSW = 32;
// Servo
const int servo_pin = 23;
// Wägezelle-IC 
const int hx711_sck_pin = 22;
const int hx711_dt_pin  = 21;
// 3x Schalter Ein 1 - Aus - Ein 2
const int switch_betrieb_pin = 27;
const int switch_vcc_pin     = -1;  // <- Vcc Pin = -1 wenn nicht benötigt 
const int switch_setup_pin   = 14;
//Buzzer
const int buzzer_pin = 33;
//LED
const int LED_pin = -1;
const int LED_BUILTIN = -1;

#define Big_Front u8g2_font_courB24_tf
#define FrontMenue u8g2_font_courB24_tf //orginal u8g2_font_courB10_tf
