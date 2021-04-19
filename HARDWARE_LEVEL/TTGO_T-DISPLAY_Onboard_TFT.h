/* ****************** HARDWARELEVEL = 5 ***************
   ****       TGGO T-Display mit Onboard TFT        ***
   ****           SCREEN_TFT_ESPI_240x135           *** 
   ****************************************************
*/

#define use_TFT  // Hanimandl sagen das er die e-SPI Libs verwenden soll
#define Logo1
#define RotateScreen 1
#define OffsetX 50
#define OffsetY 30
// Pin Definitionen der Hardware
const int vext_ctrl_pin        = -1;
// Taster 
const int button_start_pin     = 13;
const int button_start_vcc_pin = -1;  //<- Vcc auf -1 wenn nicht benötigt
const int button_stop_pin      = 12;
const int button_stop_vcc_pin  = -1;  //<- Vcc auf -1 wenn nicht benötigt
// Rotary Encoder
const int outputA  = 26;
const int outputB  = 25;
const int outputSW = 27;
// Servo
const int servo_pin = 22;
// Wägezelle-IC 
const int hx711_sck_pin = 17;
const int hx711_dt_pin  = 15;
// Schalter Betriebsmodus
const int switch_betrieb_pin = 36;  
const int switch_setup_pin   = 33;
const int switch_vcc_pin     = -1;     // <- Vcc auf -1 wenn nicht benötigt
// Buzzer
const int buzzer_pin = 33;
// LED
const int LED_pin = -1;  //<- Vcc auf -1 wenn nicht benötig
const int LED_BUILTIN=-1;