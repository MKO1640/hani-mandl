
// OLED fuer Heltec WiFi Kit 32 (ESP32 onboard OLED) 
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 15, /* data=*/ 4);   // HW I2C crashed den Code
#define Logo1
// Rotary Encoder
const int outputA  = 33;
const int outputB  = 26;
const int outputSW = 32;
            
// Servo
const int servo_pin = 2;
            
// 3x Schalter Ein 1 - Aus - Ein 2
#if HARDWARE_LEVEL == 1
const int switch_betrieb_pin = 19;
const int switch_vcc_pin     = 22;     // <- Vcc 
const int switch_setup_pin   = 21;
const int vext_ctrl_pin      = 21;     // Vext control pin            

#else // HARDWARE_LEVEL == 2
const int switch_betrieb_pin = 23;
const int switch_vcc_pin     = 19;     // <- Vcc 
const int switch_setup_pin   = 22;
const int vext_ctrl_pin      = 21;     // Vext control pin            
#endif

// Taster 
const int button_start_vcc_pin = 13;  // <- Vcc 
const int button_start_pin     = 12;
const int button_stop_vcc_pin  = 14;  // <- Vcc 
const int button_stop_pin      = 27;
            
// Poti
const int poti_pin = 39;
           
// Wägezelle-IC 
const int hx711_sck_pin = 17;
const int hx711_dt_pin  = 5;
            
// Buzzer - aktiver Piezo
static int buzzer_pin = 25;
int LED_pin = -1;