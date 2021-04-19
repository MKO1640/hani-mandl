            /*----------------------------------------------------------------------
            Setup von Freddy3108 zur verwendung eines zusätzlichem Größeren I2C Displays
            Infos zum Setup gibt es hier:
            https://github.com/Freddy3108/HaniMandl-3.0/tree/c5d2e61d9c17aef4c3fafdd942442437090cf66d/HARDWARE
            */
            // OLED fuer ESP32 outboard OLED 
            U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ 16);
            #define Logo1
            // Rotary Encoder
            const int outputA  = 33;
            const int outputB  = 26;
            const int outputSW = 32;
            
            // Servo
            const int servo_pin = 0;
            
            // 3x Schalter Ein 1 - Aus - Ein 2
            const int switch_betrieb_pin = 15;
            const int switch_vcc_pin     = 9;     // <- Vcc 
            const int switch_setup_pin   = 4;
            
            // Taster 
            const int button_start_vcc_pin = 13;  // <- Vcc 
            const int button_start_pin     = 12;
            const int button_stop_vcc_pin  = 14;  // <- Vcc 
            const int button_stop_pin      = 27;
            
            // Poti
            const int poti_pin = 7;
            
            // Wägezelle-IC 
            const int hx711_sck_pin = 17;
            const int hx711_dt_pin  = 5;
            
            // Buzzer - aktiver Piezo
            static int buzzer_pin = 25;
            const int LED_pin = 2;
            const int LED_BUILTIN = -1;