#define Logo2
#define TFT_MISO 19
#define TFT_MOSI 5
#define TFT_SCLK 18
#define TFT_CS   4 
#define TFT_DC   17
#define TFT_RST  16
#include "DISPLAY/240_320_SPI_TFT.h" // TFT-Display Laden

const int vext_ctrl_pin      = -1;
// Taster 
const int button_start_pin   = 26;
const int button_start_vcc_pin = -1;  // <- Vcc Pin = -1 wenn nicht benutzt  
const int button_stop_pin    = 25;
const int button_stop_vcc_pin  = -1;  
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
const int switch_vcc_pin     = -1;  // <- Vcc Pin = -1 wenn nicht benutzt 
const int switch_setup_pin   = 14;

const int buzzer_pin = 33;
const int LED_pin = -1;

