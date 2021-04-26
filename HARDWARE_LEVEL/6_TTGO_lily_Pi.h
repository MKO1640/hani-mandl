/* ****************** HARDWARELEVEL = 4 **************
   ****            LILYGO ESP32 Lily _Pi            **
   ****           SCREEN_ILI9341_SPI_240x320        **
   ***************************************************
*/
#define use_TFT                                 // Han
#define Logo1
#define RotateScreen 0
#define OffsetX 0
#define OffsetY 0
// Pin Definitionen der Hardware
const int vext_ctrl_pin        = -1;
// Taster 
const int button_start_pin     = 26;  
const int button_start_vcc_pin = -1;  // <- Vcc Pin = 
const int button_stop_pin      = 25;
const int button_stop_vcc_pin  = -1;  // <- Vcc Pin = 
// Rotary Encoder
const int outputA  = -1;
const int outputB  = -1;
const int outputSW = -1;
// Servo
const int servo_pin = 25;
// Wägezelle-IC 
const int hx711_sck_pin = 22;
const int hx711_dt_pin  = 21;
// 3x Schalter Ein 1 - Aus - Ein 2
const int switch_betrieb_pin = -1;
const int switch_vcc_pin     = -1;  // <- Vcc Pin = -1
const int switch_setup_pin   = -1;
//Buzzer
const int buzzer_pin = 33;
//LED
const int LED_pin = -1;
//const int LED_BUILTIN = -1; //in Pinns declaration von pinns_arduino.h schon integriert
//TOUCH
#define USE_TOUCH_I2C
const int T_SDA_Pin = 21;
const int T_SCL_Pin = 22;
const int T_int_Pin = 34;    // Interupt Pin 

// settings fürs Menue
#define MENU_WIDTH 480
#define MENU_HEIGHT 280 // etwas platz für die Modus Buttons
#define Use_Switch_Bar // Modus Button´s unterhalb des Menus
#define Use_MENU_FRAMES  //Rahmen um die Menüpunkte
#define BIG_FRONT u8g2_font_courB24_tf   //Schrifttyp und größe der Menüschrift
#define FRONT_MENU u8g2_font_courB24_tf //orginal u8g2
#define FRONT_MENU_GL u8g2_font_open_iconic_arrow_4x_t
#define Use_TOUCH
/* TODO für TOUCH FT6236 Chip
Touch Library einbinden z.B. https://github.com/DustinWatts/FT6236/blob/main/FT6236.cpp ein Beispiel ohne Lib
findet sich auch hier: https://github.com/daumemo/IPS_LCD_R61529_FT6236_Arduino_eSPI_Test/blob/master/src/main.cpp
- wenn touch aktiviert und nicht kalibriert ein Kalibrierfenster beim Start
- INIT PIN Überwachen entweder per Interrupt oder innerhalb der einzelnen Funktionen 
- Wenn Init Pin Aktiviert auslesen des I2C und Mappen auf Bildschirposition 
- ermitteln der Schaltflächen je nach Modus und den Menupunkten/Screens 
- Übergabe an Programcode 
*/