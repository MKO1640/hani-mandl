/* ****************** HARDWARELEVEL = 5 ***************
   ****       TGGO T-Display mit Onboard TFT        ***
   ****************************************************
*/

//SCREEN_TFT_ESPI_240x135   // TTGO-Display
#include <User_Setups/Setup25_TTGO_T_Display.h> // Lade die Pin-definition des onboard Displays aus eTF-Lib
#define USER_SETUP_LOADED                       // Der Lib Sagen das Die Definition bereits geladen ist.
#define use_TFT                                 // Hanimandl sagen das er die e-SPI Libs verwenden soll

// Pin Definitionen der Hardware
// Taster 
const int button_start_pin   = 13;
const int button_stop_pin    = 12;//12
// Rotary Encoder
const int outputA  = 26;
const int outputB  = 25;
const int outputSW = 27;
// Servo
const int servo_pin = 22;

// Wägezelle-IC 
const int hx711_sck_pin = 17;
const int hx711_dt_pin  = 15;

const int switch_betrieb_pin = 2;//22
const int switch_setup_pin   = 33;//22