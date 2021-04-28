/*
  Abfuellwaage Version 0.2.9
  --------------------------
  Copyright (C) 2018-2021 by Marc Vasterling, Marc Wetzel, Clemens Gruber, Marc Junker, Andreas Holzhammer, Johannes Kuder, Jeremias Bruker, Andreas Motl
            
  2018-05 Marc Vasterling    | initial version, 
                               published in the Facebook group "Imkerei und Technik. Eigenbau",
                               Marc Vasterling: "meinen Code kann jeder frei verwenden, ändern und hochladen wo er will, solange er nicht seinen eigenen Namen drüber setzt."
  2018-06 Marc Vasterling    | improved version, 
                               published in the Facebook group also
  2019-01 Marc Wetzel        | Refakturierung und Dokumentation, 
                               published in the Facebook group also
  2019-02 Clemens Gruber     | code beautifying mit kleineren Umbenennungen bei Funktionen und Variablen
                               Anpssung fuer Heltec WiFi Kit 32 (ESP32 onboard OLED) 
                               - pins bei OLED-Initialisierung geaendert
                               - pins geaendert, um Konflikte mit hard wired pins des OLEDs zu vermeiden 
  2019-02 Clemens Gruber     | Aktivierung der internen pull downs für alle digitalen Eingaenge
  2019-02 Clemens Gruber     | "normale" pins zu Vcc / GND geaendert um die Verkabelung etwas einfacher und angenehmer zu machen
  2020-05 Andreas Holzhammer | Anpassungen an das veränderte ;-( pin-Layout der Version 2 des Heltec 
                               wird verkauft als "New Wifi Kit 32" oder "Wifi Kit 32 V2"
  2020-05 Marc Junker        | - Erweiterung von Poti auf Rotary Encoder 
                               - alle Serial.prints in #ifdef eingeschlossen
                               - "Glas" nun als Array mit individuellem Tara
                               - Korrekturwert und Auswahl der Füllmenge über Drücken & Drehen des Rotary einstellbar
  2020-05 Andreas Holzhammer | - Tara pro abzufüllendem Glas automatisch anpassen (Variable tara_glas)
                               - Code läuft auch ohne Waage
  2020-06 Andreas Holzhammer | - Code wahlweise mit Heltec V1 oder V2 nutzbar
                               - Code wahlweise mit Poti oder Rotary nutzbar
                               - Tara pro Glas einstellbar
                               - Öffnungswinkel für Maximale Öffnung und Feindosierung im Setup konfigurierbar
                               - Korrektur und Glasgröße im Automatikmodus per Rotary Encoder Button wählbar
                               - Preferences löschbar über Setup
                               - Gewicht blinkt bei Vollautomatik, wenn nicht vollständig gefüllt
                               - Nicht kalibrierte Waage anzeigen, fehlende Waage anzeigen
                               - Tara wird nur bei >20g gesetzt, verhindert den Autostart bei leerer Waage
                               - Tarieren der Waage bei jedem Start bis +-20g. Sonst Warnung
  2020-07 Andreas Holzhammer | Version 0.2.4
                               - SCALE_READS auf 2 setzen? ca. 100ms schneller als 3, schwankt aber um +-1g
                               - Reihenfolge der Boot-Meldungen optimiert, damit nur relevante Warnungen ausgegeben werden
                               - Autokorrektur implementiert
                               - LOGO! und Umlaute (Anregung von Johannes Kuder)
                               - Stop-Taste verlässt Setup-Untermenüs (Anregung von Johannes Kuder)
                               - Preferences nur bei Änderung speichern
  2020-07 Andreas Holzhammer | Version 0.2.5
                               - Anzeige der vorherigen Werte im Setup
                               - Kulanzwert für Autokorrektur einstellbar
                               - Setup aufgeräumt, minimaler Servowinkel einstellbar
  2020-07 Andreas Holzhammer | Version 0.2.6
                               - Kalibrierung der Waage verbessert; Messewerte runden; Waage "aufheizen" vor Bootscreen
                               - Aktiver Piezo-Buzzer (Idee von Johannes Kuder)
  2020-07 Johannes Kuder     | Version 0.2.7
                               - Zählwerk für abgefüllte Gläser und Gewicht (nur im Automatikbetrieb)
  2020-07 Jeremias Bruker    | Version 0.2.8
                               - "GlasTyp" in allen Menüs und Automatikmodus integriert
                               - 5 Gläser können vom User im Menüpunkt "Fuellmenge" in Gewicht und GlasTyp konfiguriert werden 
                                 und werden nichtflüchtig gespeichert. So kann sich jeder User seine eigenen üblichen 5 Gläser anlegen
                               - Stabilisierung des Waagenwerts nach Wunsch (define FEHLERKORREKTUR_WAAGE)
                               - das Kalibriergewicht kann beim Kalibrierungsvorgang vom User verändert 
                                 werden (nicht jeder hat 500g als Eichgewicht) und wird nichtflüchtig gespeichert
                               - rotierendes Hauptmenü (optinal)
                               - Umkehrbarer Servo für linksseitige Quetschhähne :-)
  2020-10 Andreas Holzhammer | Version 0.2.8.1
                               - Bugfix: Servo konnte im Manuellen Modus unter Minimum bewegt werden
                               - Display umgestellt auf Hardware-I2C für schnellere Updates
                               - Glastoleranz auf +/-20g angepasst
  2020-11 Andreas Motl       | Version 0.2.8.2
                               - PlatformIO-Support eingerichtet 
  2020-12 Andreas Holzhammer | Version 0.2.8.3
                               - Drehrichtung im Scroll-Menu umgestellt
  2020-12 Clemens Gruber     | Version 0.2.8.4
                               - Binär-Datei hinzugefügt
  2020-12 Andreas Holzhammer | Version 0.2.9.0
                               - Fortschrittsanzeige eingebaut
                               - Servo-Bibliothek geändert, jetzt ESP32Servo aus dem Bibliotheksverwalter und
                                 Servo-defaults für die neue Bibliothek angepasst
  2021-01 Andreas Motl       | Version 0.2.9.1
                               - PlatformIO-Support an neue Servo-Bibliothek angepasst

                                  
  This code is in the public domain.
   
  Hinweise zur Hardware
  ---------------------
  - bei allen digitalen Eingängen sind interne pull downs aktiviert, keine externen-Widerstände nötig! 
*/

#include <Arduino.h>
#include <Wire.h>
#include <HX711.h>        // aus dem Arduino-Bibliotheksverwalter
#include <ESP32Servo.h>   // aus dem Arduino-Bibliotheksverwalter
#include <Preferences.h>  // aus dem Board Support Package (BSP) von espressif, wird verfügbar, wenn ein ESP32-board ausgewählt ist

//
// Hier den Code auf die verwendete Hardware einstellen

//#define HARDWARE_LEVEL 4        // 1 = originales Layout mit Schalter auf Pin 19/22/21
                                // 2 = Layout für V2 mit Schalter auf Pin 23/19/22
                                // 3 = ESP32 DEVKit V2 AZ-Delivery
                                // 4 = ESP32 DEVKitC 240x320 SPI TFT
                                // 5 = TTGO_T-DISPLAY_Onboard TFT

                                // In Voreinstellung https://dl.espressif.com/dl/package_esp32_index.json mit eintragen
                                // Bei Werkzeuge -> Board_> ESP32 Arduino->ESP32 Module auswählen
                                // Beim flashen (übetragen) die Boottaste gedrückt halten und anschließend Rest drücken.
                                
#define SERVO_ERWEITERT         // definieren, falls die Hardware mit dem alten Programmcode mit Poti aufgebaut wurde oder der Servo zu wenig fährt
                                // Sonst bleibt der Servo in Stop-Position einige Grad offen! Nach dem Update erst prüfen!
#define ROTARY_SCALE 2          // in welchen Schritten springt unser Rotary Encoder. 
                                // Beispiele: KY-040 = 2, HW-040 = 1, für Poti-Betrieb auf 1 setzen
#define USE_ROTARY              // Rotary benutzen
#define USE_ROTARY_SW           // Taster des Rotary benutzen
//#define USE_POTI              // Poti benutzen -> ACHTUNG, im Normalfall auch USE_ROTARY_SW deaktivieren!
//#define FEHLERKORREKTUR_WAAGE   // falls Gewichtssprünge auftreten, können diese hier abgefangen werden
                                // Achtung, kann den Wägeprozess verlangsamen. Vorher Hardware prüfen.
//#define QUETSCHHAHN_LINKS       // Servo invertieren, falls der Quetschhahn von links geöffnet wird. Mindestens ein Exemplar bekannt
//
// Ende Benutzereinstellungen!
// 

//
// Ab hier nur verstellen wenn Du genau weisst, was Du tust!
//
//#define isDebug 0             // serielle debug-Ausgabe aktivieren. Mit >3 wird jeder Messdurchlauf ausgegeben
                                // ACHTUNG: zu viel Serieller Output kann einen ISR-Watchdog Reset auslösen!
//#define POTISCALE             // Poti simuliert eine Wägezelle, nur für Testbetrieb!

// Ansteuerung der Waage
#define SCALE_READS 2      // Parameter für hx711 Library. Messwert wird aus der Anzahl gemittelt
#define SCALE_GETUNITS(n)  (waage_vorhanden ? round(scale.get_units(n)) : simulate_scale(n) )

// Ansteuerung Servo
#ifdef QUETSCHHAHN_LINKS
#define SERVO_WRITE(n)     servo.write(180-n)
#else
#define SERVO_WRITE(n)     servo.write(n)
#endif

// Rotary Encoder Taster zieht Pegel auf Low, Start/Stop auf High!
#ifdef USE_ROTARY_SW
#define SELECT_SW outputSW
#define SELECT_PEGEL LOW
#else
#define SELECT_SW button_start_pin
#define SELECT_PEGEL HIGH
#endif

// Betriebsmodi 
#define MODE_SETUP       0
#define MODE_AUTOMATIK   1
#define MODE_HANDBETRIEB 2

// Buzzer Sounds
#define BUZZER_SHORT   1
#define BUZZER_LONG    2
#define BUZZER_SUCCESS 3
#define BUZZER_ERROR   4


// ** Definition der pins 

// ---------------------- Laden der Vordefinierten Setups-----------------------------
#if (HARDWARE_LEVEL == 1 || HARDWARE_LEVEL == 2)
#include "HARDWARE_LEVEL/1_Heltec_WiFi_Kit_onboardOLED.h"
#elif (HARDWARE_LEVEL == 3 )
#include "HARDWARE_LEVEL/3_Heltec_DEVKit_V2_128x64_OLED.h"  // onbord LED und externes I2C 128 x 64 px SSD1309 Display     
#elif (HARDWARE_LEVEL == 4)
#include "HARDWARE_LEVEL/4_DevKitC_240x320_SPI_TFT.h"
#elif (HARDWARE_LEVEL == 5)
#include "HARDWARE_LEVEL/5_TTGO_T-DISPLAY_Onboard_TFT.h"
#elif (HARDWARE_LEVEL == 6)
#include "HARDWARE_LEVEL/6_TTGO_lily_Pi.h"
#else
#error Hardware Level nicht definiert! Korrektes #define setzen!
#endif

#ifdef use_TFT
#include <SPI.h>
#include <TFT_eSPI.h>
#include "U8g2_for_TFT_eSPI.h"
//#include "Free_Fonts.h"
extern TFT_eSPI tft;
TFT_eSPI tft = TFT_eSPI();           // TFT object
TFT_eSprite spr = TFT_eSprite(&tft); // Sprite object
U8g2_for_TFT_eSPI u8g2;               // U8g2 font instanz
#else
// colors we will use in use_TFT Mode must be define here
#define TFT_WHITE 0

#endif


Servo servo;
HX711 scale;
Preferences preferences;

// Datenstrukturen für Rotary Encoder
struct rotary {                        
  int Value;
  int Minimum;
  int Maximum;
  int Step;
};
#define SW_WINKEL    0
#define SW_KORREKTUR 1
#define SW_MENU      2
struct rotary rotaries[3];         // Werden in setup() initialisiert
int rotary_select = SW_WINKEL;
static boolean rotating = false;   // debounce management für Rotary Encoder

// Füllmengen für 5 verschiedene Gläser
struct glas { 
  int Gewicht;
  int GlasTyp;    //JB
  int Tara;
  int TripCount;  //Kud
  int Count;      //Kud
};
char *GlasTypArray[3] = { "DIB", "TOF", "DEE"};//DIB = DeutscherImkerBund-Glas DEE= DeepTwist-Glas TOF= TwistOff-Glas //JB
struct glas glaeser[5] =            { 
                                         {  125, 0, -9999, 0, 0 },
                                         {  250, 1, -9999, 0, 0 },
                                         {  250, 2, -9999, 0, 0 },
                                         {  500, 1, -9999, 0, 0 },
                                         {  500, 0, -9999, 0, 0 } 
                                    };

// Allgemeine Variablen
int i;                          // allgemeine Zählvariable
int pos;                        // aktuelle Position des Poti bzw. Rotary 
int gewicht;                    // aktuelles Gewicht
int tara;                       // Tara für das ausgewählte Glas, für Automatikmodus
int tara_glas;                  // Tara für das aktuelle Glas, falls Glasgewicht abweicht
long gewicht_leer;              // Gewicht der leeren Waage
float faktor;                   // Skalierungsfaktor für Werte der Waage
int fmenge;                     // ausgewählte Füllmenge
int fmenge_index;               // Index in gläser[]
int korrektur;                  // Korrekturwert für Abfüllmenge
int autostart;                  // Vollautomatik ein/aus
int autokorrektur;              // Autokorrektur ein/aus
int kulanz_gr;                  // gewollte Überfüllung im Autokorrekturmodus in Gramm
int winkel;                     // aktueller Servo-Winkel
int winkel_hard_min = 0;        // Hard-Limit für Servo
int winkel_hard_max = 180;      // Hard-Limit für Servo
int winkel_min = 0;             // konfigurierbar im Setup
int winkel_max = 85;            // konfigurierbar im Setup
int winkel_fein = 35;           // konfigurierbar im Setup
float fein_dosier_gewicht = 60; // float wegen Berechnung des Schliesswinkels
int servo_aktiv = 0;            // Servo aktivieren ja/nein
int kali_gewicht = 500;         // frei wählbares Gewicht zum kalibrieren
char ausgabe[30];               // Fontsize 12 = 13 Zeichen maximal in einer zeile
int modus = -1;                 // Bei Modus-Wechsel den Servo auf Minimum fahren
int auto_aktiv = 0;             // Für Automatikmodus - System ein/aus?
int waage_vorhanden = 0;        // HX711 nicht ansprechen, wenn keine Waage angeschlossen ist, sonst Crash
long preferences_chksum;        // Checksumme, damit wir nicht sinnlos Prefs schreiben
int buzzermode = 0;             // 0 = aus, 1 = ein. TODO: Tastentöne als buzzermode 2?
bool gezaehlt = false;          // Kud Zähl-Flag
bool setup_modern = 0;          // Setup als rotierendes Menu
int glastoleranz = 20;          // Gewicht für autostart darf um +-20g schwanken, insgesamt also 40g!
int zeile = MENU_HEIGHT/6;

// Simuliert die Dauer des Wägeprozess, wenn keine Waage angeschlossen ist. Wirkt sich auf die Blinkfrequenz im Automatikmodus aus.
long simulate_scale(int n) {
    long sim_gewicht = 9500;
    while (n-- >= 1) { 
      delay(10);    // empirisch ermittelt. n=2: 10, n=3: 40, n=4: 50
    }
#ifdef POTISCALE
    sim_gewicht = (map(analogRead(poti_pin), 0, 4095, 0, 700));
#endif   
    return sim_gewicht;
}

#ifdef USE_ROTARY_SW
// Rotary Taster. Der Interrupt kommt nur im Automatikmodus zum Tragen und nur wenn der Servo inaktiv ist.
// Der Taster schaltet in einen von drei Modi, in denen unterschiedliche Werte gezählt werden.
void IRAM_ATTR isr1() {
  static unsigned long last_interrupt_time = 0; 
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 300) {      // If interrupts come faster than 300ms, assume it's a bounce and ignore
    if ( modus == MODE_AUTOMATIK && servo_aktiv == 0 ) { // nur im Automatik-Modus interessiert uns der Click
      rotary_select = (rotary_select + 1) % 3;
#ifdef isDebug
    Serial.print("Rotary Button changed to ");
    Serial.println(rotary_select);
#endif 
    }
    last_interrupt_time = interrupt_time;
  }
}
#endif

#ifdef USE_ROTARY
// Rotary Encoder. Zählt in eine von drei Datenstrukturen: 
// SW_WINKEL    = Einstellung des Servo-Winkels
// SW_KORREKTUR = Korrekturfaktor für Füllgewicht
// SW_MENU      = Zähler für Menuauswahlen  
void IRAM_ATTR isr2() {
  static int aState;
  static int aLastState = 2;  // reale Werte sind 0 und 1
  
  if ( rotating ) delay (1);  // wait a little until the bouncing is done
   
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
    if (aState != aLastState) {     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) {
         rotaries[rotary_select].Value -= rotaries[rotary_select].Step;
      } else {    // counter-clockwise
         rotaries[rotary_select].Value += rotaries[rotary_select].Step;
      }
      rotaries[rotary_select].Value = constrain( rotaries[rotary_select].Value, rotaries[rotary_select].Minimum, rotaries[rotary_select].Maximum );
      rotating = false;
  #ifdef isDebug
  #if isDebug >= 5
      Serial.print(" Rotary Value changed to ");
      Serial.println(getRotariesValue(rotary_select));
  #endif 
  #endif 
    }
    aLastState = aState; // Updates the previous state of the outputA with the current state
 }
 #endif

//
 // Skalierung des Rotaries für verschiedene Rotary Encoder oder Simulation über Poti
int getRotariesValue( int rotary_mode ) {
#ifdef USE_ROTARY
    return (rotaries[rotary_mode].Value / ROTARY_SCALE);
#elif defined USE_POTI
    int poti_min = (rotaries[rotary_mode].Minimum / ROTARY_SCALE);
    int poti_max = (rotaries[rotary_mode].Maximum / ROTARY_SCALE);
    if( rotaries[rotary_mode].Step > 0 ) {
       return (map(analogRead(poti_pin), 0, 4095, poti_min, poti_max));
    } else {
       return (map(analogRead(poti_pin), 0, 4095, poti_max, poti_min));
    }
#else
#error Weder Rotary noch Poti aktiviert!
#endif 
}
void setRotariesValue( int rotary_mode, int rotary_value ) {
    rotaries[rotary_mode].Value = rotary_value * ROTARY_SCALE;
}
void initRotaries( int rotary_mode, int rotary_value, int rotary_min, int rotary_max, int rotary_step ) {
    rotaries[rotary_mode].Value     = rotary_value * ROTARY_SCALE;
    rotaries[rotary_mode].Minimum   = rotary_min   * ROTARY_SCALE;
    rotaries[rotary_mode].Maximum   = rotary_max   * ROTARY_SCALE;
    rotaries[rotary_mode].Step      = rotary_step;

#ifdef isDebug
    Serial.print("initRotaries..."); 
    Serial.print(" Rotary Mode: ");  Serial.print(rotary_mode);
    Serial.print(" rotary_value: "); Serial.print(rotary_value);
    Serial.print(" Value: ");        Serial.print(rotaries[rotary_mode].Value);
    Serial.print(" Min: ");          Serial.print(rotaries[rotary_mode].Minimum);
    Serial.print(" Max: ");          Serial.print(rotaries[rotary_mode].Maximum);
    Serial.print(" Step: ");         Serial.print(rotaries[rotary_mode].Step);
    Serial.print(" Scale: ");        Serial.println(ROTARY_SCALE);
#endif
}
// Ende Funktionen für den Rotary Encoder
//


void getPreferences(void) {
    preferences.begin("EEPROM", false);            // Parameter aus dem EEPROM lesen

    faktor        = preferences.getFloat("faktor", 0.0);  // falls das nicht gesetzt ist -> Waage ist nicht kalibriert
    pos           = preferences.getUInt("pos", 0);
    gewicht_leer  = preferences.getUInt("gewicht_leer", 0); 
    korrektur     = preferences.getUInt("korrektur", 0);
    autostart     = preferences.getUInt("autostart", 0);
    autokorrektur = preferences.getUInt("autokorrektur", 0);
    kulanz_gr     = preferences.getUInt("kulanz_gr", 5);
    fmenge_index  = preferences.getUInt("fmenge_index", 3);
    winkel_min    = preferences.getUInt("winkel_min", winkel_min);
    winkel_max    = preferences.getUInt("winkel_max", winkel_max);
    winkel_fein   = preferences.getUInt("winkel_fein", winkel_fein);
    buzzermode    = preferences.getUInt("buzzermode", buzzermode);
    kali_gewicht  = preferences.getUInt("kali_gewicht", kali_gewicht); //JB 
    setup_modern  = preferences.getUInt("setup_modern", setup_modern);

    preferences_chksum = faktor + pos + gewicht_leer + korrektur + autostart + autokorrektur + fmenge_index + winkel_min + winkel_max + winkel_fein + kulanz_gr + buzzermode + kali_gewicht + setup_modern;

    i = 0;
    int ResetGewichte[] = {125,250,250,500,500,};
    int ResetGlasTyp[] = {0,1,2,1,0,};
    while( i < 5) {
      sprintf(ausgabe, "Gewicht%d", i); //JB
      glaeser[i].Gewicht = preferences.getInt(ausgabe, ResetGewichte[i]); //JB
      preferences_chksum += glaeser[i].Gewicht; //JB
      
      sprintf(ausgabe, "GlasTyp%d", i); //JB
      glaeser[i].GlasTyp = preferences.getInt(ausgabe, ResetGlasTyp[i]); //JB
      preferences_chksum += glaeser[i].GlasTyp; //JB
      
      sprintf(ausgabe, "Tara%d", i);
      glaeser[i].Tara= preferences.getInt(ausgabe, -9999);
      preferences_chksum += glaeser[i].Tara;
      
      sprintf(ausgabe, "TripCount%d", i); //Kud
      glaeser[i].TripCount = preferences.getInt(ausgabe, 0);//Kud
      preferences_chksum += glaeser[i].TripCount;
      
      sprintf(ausgabe, "Count%d", i); //Kud
      glaeser[i].Count = preferences.getInt(ausgabe, 0);//Kud
      preferences_chksum += glaeser[i].Count;
      i++;
    }

    preferences.end();

#ifdef isDebug
    Serial.println("get Preferences:");
    Serial.print("pos = ");          Serial.println(pos);
    Serial.print("faktor = ");       Serial.println(faktor);
    Serial.print("gewicht_leer = "); Serial.println(gewicht_leer);
    Serial.print("korrektur = ");    Serial.println(korrektur);
    Serial.print("autostart = ");    Serial.println(autostart);
    Serial.print("autokorrektur = ");Serial.println(autokorrektur);
    Serial.print("kulanz_gr = ");    Serial.println(kulanz_gr);
    Serial.print("fmenge_index = "); Serial.println(fmenge_index);
    Serial.print("winkel_min = ");   Serial.println(winkel_min);
    Serial.print("winkel_max = ");   Serial.println(winkel_max);
    Serial.print("winkel_fein = ");  Serial.println(winkel_fein);
    Serial.print("buzzermode = ");   Serial.println(buzzermode);
    Serial.print("kali_gewicht = "); Serial.println(kali_gewicht);//JB 
    Serial.print("setup_modern = "); Serial.println(setup_modern); 

    i = 0;
    while( i < 5 ) {
      sprintf(ausgabe, "Gewicht%d = ", i);
      Serial.print(ausgabe);         
      Serial.println(glaeser[i].Gewicht);
      
      sprintf(ausgabe, "GlasTyp%d = ", i);
      Serial.print(ausgabe);         
      Serial.println(GlasTypArray[glaeser[i].GlasTyp]);
      
      sprintf(ausgabe, "Tara%d = ", i);
      Serial.print(ausgabe);         
      Serial.println(glaeser[i].Tara);

      i++;
    }
    Serial.print("Checksumme:");    
    Serial.println(preferences_chksum);    
#endif
}

void setPreferences(void) {
    long preferences_newchksum;
    int winkel = getRotariesValue(SW_WINKEL);

    preferences_newchksum = faktor + winkel + gewicht_leer + korrektur + autostart + autokorrektur + fmenge_index + winkel_min + winkel_max + winkel_fein + kulanz_gr + buzzermode + kali_gewicht + setup_modern;
    i = 0;
    while( i < 5 ) {
      preferences_newchksum += glaeser[i].Gewicht;//JB
      preferences_newchksum += glaeser[i].GlasTyp;//JB
      preferences_newchksum += glaeser[i].Tara;
      preferences_newchksum += glaeser[i].TripCount;//Kud
      preferences_newchksum += glaeser[i].Count;//Kud
      i++;
    }

    if( preferences_newchksum == preferences_chksum ) {
      #ifdef isDebug
      Serial.println("Preferences unverändert");
      #endif
      getPreferences();
      return;
    }
    preferences_chksum = preferences_newchksum;
    
    preferences.begin("EEPROM", false);
    preferences.putFloat("faktor", faktor);
    preferences.putUInt("gewicht_leer", gewicht_leer);
    preferences.putUInt("pos", winkel);
    preferences.putUInt("korrektur", korrektur);
    preferences.putUInt("autostart", autostart);
    preferences.putUInt("autokorrektur", autokorrektur);
    preferences.putUInt("kulanz_gr", kulanz_gr);
    preferences.putUInt("winkel_min", winkel_min);
    preferences.putUInt("winkel_max", winkel_max);
    preferences.putUInt("winkel_fein", winkel_fein);
    preferences.putUInt("fmenge_index", fmenge_index);
    preferences.putUInt("buzzermode", buzzermode);
    preferences.putUInt("kali_gewicht", kali_gewicht);//JB
    preferences.putUInt("setup_modern", setup_modern);

    i = 0;
    while( i < 5 ) {
      sprintf(ausgabe, "Gewicht%d", i);
      preferences.putInt(ausgabe, glaeser[i].Gewicht);
      sprintf(ausgabe, "GlasTyp%d", i);
      preferences.putInt(ausgabe, glaeser[i].GlasTyp);  
      sprintf(ausgabe, "Tara%d", i);
      preferences.putInt(ausgabe, glaeser[i].Tara);
      sprintf(ausgabe, "TripCount%d", i);
      preferences.putInt(ausgabe, glaeser[i].TripCount);//Kud
      sprintf(ausgabe, "Count%d", i);
      preferences.putInt(ausgabe, glaeser[i].Count);//Kud
      i++;
    }
    preferences.end();

    #ifdef isDebug
    Serial.println("Set Preferences:");
    Serial.print("pos = ");          Serial.println(winkel);
    Serial.print("faktor = ");       Serial.println(faktor);
    Serial.print("gewicht_leer = "); Serial.println(gewicht_leer);
    Serial.print("korrektur = ");    Serial.println(korrektur);
    Serial.print("autostart = ");    Serial.println(autostart);
    Serial.print("autokorrektur = ");Serial.println(autokorrektur);
    Serial.print("kulanz_gr = ");    Serial.println(kulanz_gr);
    Serial.print("fmenge_index = "); Serial.println(fmenge_index);
    Serial.print("winkel_min = ");   Serial.println(winkel_min);
    Serial.print("winkel_max = ");   Serial.println(winkel_max);
    Serial.print("winkel_fein = ");  Serial.println(winkel_fein);
    Serial.print("buzzermode = ");   Serial.println(buzzermode);
    Serial.print("kali_gewicht = "); Serial.println(kali_gewicht); //JB
    Serial.print("setup_modern = "); Serial.println(setup_modern);
    i = 0;
    while( i < 5 ) {
      sprintf(ausgabe, "Gewicht%d = ", i);
      Serial.print(ausgabe);         Serial.println(glaeser[i].Gewicht);
      sprintf(ausgabe, "GlasTyp%d = ", i);
      Serial.print(ausgabe);         Serial.println(GlasTypArray[glaeser[i].GlasTyp]);
      sprintf(ausgabe, "Tara%d = ", i);
      Serial.print(ausgabe);         Serial.println(glaeser[i].Tara);
      i++;
    }
    #endif
  }

void setupTripCounter(void) { //Kud
  int j;
  i = 1;
  float TripAbfuellgewicht = 0;
  u8g2.setFont(FRONT_MENU);
  int zeile = MENU_HEIGHT/6;
  int spalte = u8g2.getUTF8Width("A");  i = 1;
  while (i > 0) { //Erster Screen: Anzahl pro Glasgröße
    j = 0;
    if ((digitalRead(button_stop_pin)) == HIGH)
      return;

    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      //verlasse Screen
      i = 0;
      delay(250);
    }
    clearScreen();
    while ( j < 5 ) {
      u8g2.setCursor(0, zeile + (j * zeile));
      sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]);
      u8g2.print(ausgabe);
      u8g2.setCursor(spalte*8, zeile + (j * zeile));
      sprintf(ausgabe, "%5dSt.", glaeser[j].TripCount);
      u8g2.print(ausgabe);
      j++;
    }
    screen_zeigen();
    delay(100);
  }
//**************************************************************************
//              Zweiter Screen: Gewicht pro Glasgröße
  i = 1;
  while (i > 0) { 
    j = 0;
    if ((digitalRead(button_stop_pin)) == HIGH)
      return;

    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      //verlasse Screen
      i = 0;
      delay(250);
    }
    clearScreen();
    while ( j < 5  ) {
      u8g2.setCursor(0, zeile + (j * zeile));
      sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht,GlasTypArray[glaeser[j].GlasTyp]);
      u8g2.print(ausgabe);
      u8g2.setCursor(spalte*8, zeile + (j * zeile));
      //      Serial.println(glaeser[j].Gewicht);
      //      Serial.print("\t");
      //      Serial.print(glaeser[j].TripCount);
      //      Serial.print("\t");
      //      Serial.print(glaeser[j].Gewicht * glaeser[j].TripCount / 1000.0);
      //      Serial.println();

      sprintf(ausgabe, "%5.1fkg", glaeser[j].Gewicht * glaeser[j].TripCount / 1000.0);
      u8g2.print(ausgabe);
      j++;
    }
    screen_zeigen();
    delay(100);
  }

  i = 1;
  while (i > 0) { //Dritter Screen: Gesamtgewicht
    TripAbfuellgewicht = 0;
    j = 0;
    if ((digitalRead(button_stop_pin)) == HIGH)
      return;

    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      //verlasse Screen
      i = 0;
      delay(250);
    }

    while ( j < 5) {
      TripAbfuellgewicht += glaeser[j].Gewicht * glaeser[j].TripCount / 1000.0;
      j++;
    }
    clearScreen();
    u8g2.setCursor(0, zeile);
    u8g2.print("Summe Trip:");
    u8g2.setCursor(spalte*3, 3*zeile);
    sprintf(ausgabe, "%5.1fkg", TripAbfuellgewicht);
    u8g2.print(ausgabe);
    screen_zeigen();
    delay(100);
  }

  i = 1;
  while (i > 0) { //Vierter Screen: Zurücksetzen
    initRotaries(SW_MENU, 1, 0, 1, -1);

    i = 1;
    while (i > 0) {
      if ((digitalRead(button_stop_pin)) == HIGH)
        return;

      pos = getRotariesValue(SW_MENU);
      clearScreen();
      u8g2.setCursor(spalte, zeile);    u8g2.print("Reset");
      u8g2.setCursor(spalte, 2*zeile);    u8g2.print("Abbrechen");

      u8g2.setCursor(0, zeile + ((pos) * zeile));
      u8g2.print("*");
      screen_zeigen();

      if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
        u8g2.setCursor(spalte*11, zeile + ((pos) * zeile));
        u8g2.print("OK");
        screen_zeigen();
        if ( pos == 0) {
          j = 0;
          while ( j < 5  ) {
            glaeser[j].TripCount = 0;
            j++;
          }
          setPreferences();
        }
        delay(1000);
        i = 0;
      }
    }
  }
}

void setupCounter(void) { //Kud
  int j;
  i = 1;
  float Abfuellgewicht = 0;
  u8g2.setFont(FRONT_MENU);
  int zeile = MENU_HEIGHT/6;
  int spalte = u8g2.getUTF8Width("A");  i = 1;
  while (i > 0) { //Erster Screen: Anzahl pro Glasgröße
    j = 0;
    if ((digitalRead(button_stop_pin)) == HIGH)
      return;

    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      //verlasse Screen
      i = 0;
      delay(250);
    }
    clearScreen();
    while ( j < 5 ) {
      u8g2.setCursor(0, zeile + (j * zeile));
      sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht,GlasTypArray[glaeser[j].GlasTyp]);
      u8g2.print(ausgabe);
      u8g2.setCursor(spalte*7, zeile + (j * zeile));
      sprintf(ausgabe, "%5dSt.", glaeser[j].Count);
      u8g2.print(ausgabe);
      j++;
    }
    screen_zeigen();
    delay(100);
  }

  i = 1;
  while (i > 0) { //Zweiter Screen: Gewicht pro Glasgröße
    j = 0;
    if ((digitalRead(button_stop_pin)) == HIGH)
      return;

    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      //verlasse Screen
      i = 0;
      delay(250);
    }
    clearScreen();
    while ( j < 5) {
      u8g2.setCursor(0, zeile + (j * zeile));
      sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht,GlasTypArray[glaeser[j].GlasTyp]);
      u8g2.print(ausgabe);
      u8g2.setCursor(spalte*8, zeile + (j * zeile));
      //      Serial.println(glaeser[j].Gewicht);
      //      Serial.print("\t");
      //      Serial.print(glaeser[j].Count);
      //      Serial.print("\t");
      //      Serial.print(glaeser[j].Gewicht * glaeser[j].Count / 1000.0);
      //      Serial.println();

      sprintf(ausgabe, "%5.1fkg", glaeser[j].Gewicht * glaeser[j].Count / 1000.0);
      u8g2.print(ausgabe);
      j++;
    }
    screen_zeigen();
    delay(100);
  }

  i = 1;
  while (i > 0) { //Dritter Screen: Gesamtgewicht
    Abfuellgewicht = 0;
    j = 0;
    if ((digitalRead(button_stop_pin)) == HIGH)
      return;

    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      //verlasse Screen
      i = 0;
      delay(250);
    }

    while ( j < 5  ) {
      Abfuellgewicht += glaeser[j].Gewicht * glaeser[j].Count / 1000.0;
      j++;
    }
    clearScreen();
    u8g2.setCursor(spalte, zeile);
    u8g2.print("Summe:");
    u8g2.setCursor(spalte, 3*zeile);
    sprintf(ausgabe, "%5.1fkg", Abfuellgewicht);
    u8g2.print(ausgabe);
    screen_zeigen();
    delay(100);
  }

  i = 1;
  while (i > 0) { //Vierter Screen: Zurücksetzen
    initRotaries(SW_MENU, 1, 0, 1, -1);

    i = 1;
    while (i > 0) {
      if ((digitalRead(button_stop_pin)) == HIGH)
        return;

      pos = getRotariesValue(SW_MENU);
      clearScreen();
      u8g2.setCursor(spalte,   zeile);    u8g2.print("Zähler Summe:");
      u8g2.setCursor(spalte, 2*zeile);    u8g2.print("Reset?");
      u8g2.setCursor(spalte, 3*zeile);    u8g2.print("Abbrechen");

      u8g2.setCursor(0, (2*zeile) + ((pos) * zeile));
      u8g2.print("*");
      screen_zeigen();

      if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
        u8g2.setCursor(spalte*9, (2*zeile) + ((pos) * zeile));
        u8g2.print("OK");
        screen_zeigen();
        if ( pos == 0) {
          j = 0;
          while ( j < 5  ) {
            glaeser[j].Count = 0;
            glaeser[j].TripCount = 0;
            j++;
          }
          setPreferences();

        }
        delay(1000);
        i = 0;
      }
    }
  }
}

void setupTara(void) {
    int j;
    tara = 0;
    u8g2.setFont(FRONT_MENU);
    int zeile = MENU_HEIGHT/6;
    int spalte = u8g2.getUTF8Width("A");  i = 1;

    initRotaries( SW_MENU, fmenge_index, 0, 4, -1 );   // Set Encoder to Menu Mode, four Selections, inverted count
      
    i = 0;
    while ( i == 0 ) {
      if ((digitalRead(button_stop_pin)) == HIGH)
         return;
      
      if ( digitalRead(SELECT_SW) == SELECT_PEGEL ) {
        tara = (int(SCALE_GETUNITS(10)));
        if ( tara > 20 ) {                  // Gläser müssen mindestens 20g haben
           glaeser[getRotariesValue(SW_MENU)].Tara = tara; 
        }
        i++;
      }
      
      clearScreen();

      j = 0;
      while( j < 5  ) {
        u8g2.setCursor(spalte, zeile+(j*zeile));
        if ( glaeser[j].Gewicht < 1000 ) {
          sprintf(ausgabe, "%3d-%3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]); 
        } else {
          sprintf(ausgabe, "%3s-%3s", "1kg", GlasTypArray[glaeser[j].GlasTyp]); 
        }
        u8g2.print(ausgabe);
        u8g2.setCursor(spalte*8, zeile+(j*zeile));
        if ( glaeser[j].Tara > 0 ) { 
          sprintf(ausgabe, " %4dg", glaeser[j].Tara); 
          u8g2.print(ausgabe);
        } else {
          u8g2.print(" fehlt");
        }
        j++;
      }
      #ifdef Use_MENU_FRAMES
      spr.drawRoundRect(4, (getRotariesValue(SW_MENU) * zeile)+(zeile/5), spalte*14, zeile+2, zeile/4, TFT_LIGHTGREY);
      #else
      u8g2.setFont(FRONT_MENU);
      u8g2.setCursor(0, zeile+(getRotariesValue(SW_MENU)*zeile) );
      u8g2.print("*");
      #endif
      screen_zeigen();
    }
    delay(2000);
}

void setupCalibration(void) {
  float gewicht_raw;
  u8g2.setFont(FRONT_MENU);
  int zeile = MENU_HEIGHT/6;
  int spalte = u8g2.getUTF8Width("A");  i = 1;
  clearScreen();
  u8g2.setCursor(spalte, zeile);      u8g2.print("Bitte Waage");
  u8g2.setCursor(spalte, 2*zeile);    u8g2.print("leeren");
  u8g2.setCursor(spalte, 3*zeile);    u8g2.print("und mit OK");
  u8g2.setCursor(spalte, 4*zeile);    u8g2.print("bestätigen");
  screen_zeigen();
    
  i = 1;
  while (i > 0) {
    if ((digitalRead(button_stop_pin)) == HIGH) 
      return;
         
    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      scale.set_scale();
      scale.tare(10);
      delay(500);
        i = 0;
    }
  }
  initRotaries( SW_MENU, kali_gewicht, 100, 9999, 1 ); 
  i = 1;
  while (i > 0) {
    if ((digitalRead(button_stop_pin)) == HIGH) 
      return;
      
    kali_gewicht = getRotariesValue(SW_MENU);  
  
    int blinktime = (millis()/10) % 5;
    clearScreen();
    u8g2.setCursor(spalte, zeile);u8g2.print("Bitte "); 

    if (blinktime < 3) {
      sprintf(ausgabe, "%dg", kali_gewicht);
    } else {
      sprintf(ausgabe, "     ");
    }
    u8g2.print(ausgabe);
    u8g2.setCursor(spalte, 2*zeile);    u8g2.print("aufstellen");
    u8g2.setCursor(spalte, 3*zeile);    u8g2.print("und mit OK");
    u8g2.setCursor(spalte, 4*zeile);    u8g2.print("bestätigen");
    screen_zeigen();
      
    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
      clearScreen();
      u8g2.setCursor(spalte, 1*zeile);    u8g2.print("Die Waage");
      u8g2.setCursor(spalte, 2*zeile);    u8g2.print("wurde auf");
      u8g2.setCursor(spalte, 3*zeile);u8g2.print(" "); 
      sprintf(ausgabe, "%dg", kali_gewicht);
      u8g2.print(ausgabe);
      u8g2.setCursor(spalte, 4*zeile);    u8g2.print("kalibriert.");
      screen_zeigen();
      gewicht_raw  = scale.get_units(10);
      faktor       = gewicht_raw / kali_gewicht;
      scale.set_scale(faktor);
      gewicht_leer = scale.get_offset();    // Leergewicht der Waage speichern
#ifdef isDebug
      Serial.print("kalibrier_gewicht = ");
      Serial.println(kali_gewicht);
      Serial.print("gewicht_leer = ");
      Serial.println(gewicht_leer);
      Serial.print("gewicht_raw = ");
      Serial.println(gewicht_raw);
      Serial.print(" Faktor = ");
      Serial.println(faktor);
#endif        
      delay(1000);
      i = 0;        
    }
  }
}

void setupKorrektur(void) {
    int korrektur_alt = korrektur;

    rotary_select = SW_KORREKTUR;
    u8g2.setFont(FRONT_MENU);
    int zeile = MENU_HEIGHT/6;
    int spalte = u8g2.getUTF8Width("A");  i = 1;

    i = 1;
    while (i > 0) {
      if ((digitalRead(button_stop_pin)) == HIGH) {
         setRotariesValue(SW_KORREKTUR, korrektur_alt);
         korrektur = korrektur_alt;
         rotary_select = SW_MENU;
         return;
      }
      
      korrektur = getRotariesValue(SW_KORREKTUR);
      clearScreen();
      u8g2.setCursor(spalte, zeile);
      u8g2.print("Korrektur");
      
      if (korrektur>9){
        u8g2.setCursor(spalte*4, 2*zeile);     // A.P.
      }
      else{
        u8g2.setCursor(spalte*5, 2*zeile);
      }     // A.P.
      u8g2.print(korrektur);

      u8g2.setCursor(spalte, 3*zeile);     // A.P.
      u8g2.print("alter Wert");   // A.P.
      if (korrektur_alt>9){
        u8g2.setCursor(spalte*4, 4*zeile);     // A.P.
      }
      else{
        u8g2.setCursor(spalte*5, 4*zeile);
        }     // A.P.
      u8g2.print(korrektur_alt);  // A.P.
      
      screen_zeigen();
      
      if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {
        u8g2.setCursor(spalte*11, 2*zeile);
        u8g2.print("OK");
        screen_zeigen();
        delay(1000);
        i = 0;
      }
    }
    rotary_select = SW_MENU;
}

void setupServoWinkel(void) {
  int menuitem;
  int lastmin  = winkel_min;
  int lastfein = winkel_fein;
  int lastmax  = winkel_max;
  int wert_alt;
  bool wert_aendern = false;
  bool servo_live = false;
  
  initRotaries(SW_MENU, 0, 0, 4, -1);
  u8g2.setFont(FRONT_MENU);
  int zeile = MENU_HEIGHT/6;
  int spalte = u8g2.getUTF8Width("A")+1;  
  i = 1;
  i = 1;
  while (i > 0) {
    if ((digitalRead(button_stop_pin)) == HIGH) {
      winkel_min  = lastmin;
      winkel_fein = lastfein;
      winkel_max  = lastmax;
      if ( servo_live == true ) SERVO_WRITE(winkel_min);
      return;
    }

    if ( wert_aendern == false ) {
      menuitem = getRotariesValue(SW_MENU);
    } else {
      switch (menuitem) {
        case 0: servo_live  = getRotariesValue(SW_MENU);
                break;
        case 1: winkel_min  = getRotariesValue(SW_MENU);
                if ( servo_live == true ) SERVO_WRITE(winkel_min);
                break;
        case 2: winkel_fein = getRotariesValue(SW_MENU);
                if ( servo_live == true ) SERVO_WRITE(winkel_fein);
                break;
        case 3: winkel_max  = getRotariesValue(SW_MENU);
                if ( servo_live == true ) SERVO_WRITE(winkel_max);
                break;
      }
    }

    clearScreen();
    u8g2.setCursor(spalte, 2*zeile); sprintf(ausgabe,"Minimum   %3d", winkel_min);  u8g2.print(ausgabe);
    u8g2.setCursor(spalte, 3*zeile); sprintf(ausgabe,"Feindos.  %3d", winkel_fein); u8g2.print(ausgabe);
    u8g2.setCursor(spalte, 4*zeile); sprintf(ausgabe,"Maximum   %3d", winkel_max);  u8g2.print(ausgabe);
    u8g2.setCursor(spalte, 5*zeile); u8g2.print(     "Speichern");
    
    #ifdef Use_MENU_FRAMES
    if ( wert_aendern == false ) {
    u8g2.setCursor(spalte, zeile); sprintf(ausgabe,"Livesetup %3s", (servo_live==false?"aus":"ein")); u8g2.print(ausgabe);
    spr.drawRoundRect(4, (menuitem * zeile)+(zeile/5), spalte*14, zeile+2, zeile/4, TFT_LIGHTGREY);
    } 
    else {
      if ( menuitem != 0 ) { 
        u8g2.setCursor(spalte, zeile); sprintf(ausgabe,"  vorher: %3d", wert_alt); u8g2.print(ausgabe);
        } 
      else {
          u8g2.setCursor(spalte, zeile); sprintf(ausgabe,"Livesetup %3s", (servo_live==false?"aus":"ein")); u8g2.print(ausgabe);
        }
        spr.drawRoundRect(spalte*10, (menuitem * zeile)+(zeile/5), spalte*5, zeile+2, zeile/4, TFT_LIGHTGREY);   
      spr.drawRoundRect(spalte*10, (menuitem * zeile)+(zeile/5), spalte*5, zeile+2, zeile/4, TFT_LIGHTGREY);
    }  
    #else
    if ( wert_aendern == false ) {
      u8g2.setCursor(spalte, zeile); sprintf(ausgabe,"Livesetup %3s", (servo_live==false?"aus":"ein")); u8g2.print(ausgabe);
      u8g2.setCursor(0, zeile+(menuitem*zeile)); u8g2.print("*");
    } 
    else {
       if ( menuitem != 0 ) { 
          u8g2.setCursor(spalte, zeile); sprintf(ausgabe,"  vorher: %3d", wert_alt); u8g2.print(ausgabe);
        } 
        else {
          u8g2.setCursor(spalte, zeile); sprintf(ausgabe,"Livesetup %3s", (servo_live==false?"aus":"ein")); u8g2.print(ausgabe);
        }
        u8g2.setCursor( 0, zeile+(menuitem*zeile)); u8g2.print("-");   
    }    
    #endif
    screen_zeigen();

    if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) 
         && (menuitem < 4 )
         && (wert_aendern == false) ) {

         // debounce
         delay(10);  
         while( digitalRead(SELECT_SW) == SELECT_PEGEL )
            ;
         delay(10);
           
         switch (menuitem) { 
           case 0: initRotaries(SW_MENU, servo_live, 0, 1, 1);
                   break;
           case 1: initRotaries(SW_MENU, winkel_min,  winkel_hard_min, winkel_fein,     1);
                   wert_alt = lastmin;
                   break;
           case 2: initRotaries(SW_MENU, winkel_fein, winkel_min,      winkel_max,      1);
                   wert_alt = lastfein;
                   break;
           case 3: initRotaries(SW_MENU, winkel_max,  winkel_fein,     winkel_hard_max, 1);
                   wert_alt = lastmax;
                   break;
         }
         wert_aendern = true;
      }

      if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) 
           && (menuitem < 4 )
           && (wert_aendern == true) ) {

         // debounce
         delay(10);
         while( digitalRead(SELECT_SW) == SELECT_PEGEL )
            ;
         delay(10);

         if ( servo_live == true )
           SERVO_WRITE(winkel_min);
         initRotaries(SW_MENU, menuitem, 0, 4, -1);
         wert_aendern = false;
      }

      if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) && (menuitem == 4) ) {
        u8g2.setCursor(spalte*11, zeile+(menuitem*zeile));
        u8g2.print("OK");
        screen_zeigen();
        delay(1000);
        i = 0;
      }
    }
}

void setupAutomatik(void) {
  int menuitem;
  int lastautostart     = autostart;
  int lastautokorrektur = autokorrektur;
  int lastkulanz        = kulanz_gr;
  bool wert_aendern = false;

  initRotaries(SW_MENU, 0, 0, 3, -1);

  u8g2.setFont(FRONT_MENU);
  int zeile = MENU_HEIGHT/6;
  int spalte = u8g2.getUTF8Width("A")+1;  i = 1;
  while (i > 0) {
    if ((digitalRead(button_stop_pin)) == HIGH) {
      autostart     = lastautostart;
      autokorrektur = lastautokorrektur;
      kulanz_gr     = lastkulanz;
      return;
    }

    if ( wert_aendern == false ) {
      menuitem = getRotariesValue(SW_MENU);
      if ( menuitem == 3 )
        menuitem = 4;  // Eine zeile Abstand zu "Speichern"
    } else {
      switch (menuitem) {
        case 0: autostart     = getRotariesValue(SW_MENU);
                break;
        case 1: autokorrektur = getRotariesValue(SW_MENU);
                break;
        case 2: kulanz_gr     = getRotariesValue(SW_MENU);
                break;
      }
    }

    // Menu
    clearScreen();
    u8g2.setCursor(spalte, zeile); sprintf(ausgabe,"Autostart %3s", (autostart==0?"aus":"ein"));     u8g2.print(ausgabe);
    u8g2.setCursor(spalte, zeile*2); sprintf(ausgabe,"Autokorr. %3s", (autokorrektur==0?"aus":"ein")); u8g2.print(ausgabe);
    u8g2.setCursor(spalte, zeile*3); sprintf(ausgabe,"-> Kulanz %2dg", kulanz_gr);                     u8g2.print(ausgabe);
    u8g2.setCursor(spalte, zeile*5); u8g2.print(     "Speichern");

    // Positionsanzeige im Menu. "*" wenn nicht ausgewählt, Pfeil wenn ausgewählt
    #ifdef Use_MENU_FRAMES
    if ( wert_aendern == false ) {
    spr.drawRoundRect(4, (menuitem * zeile)+(zeile/5), spalte*14, zeile+2, zeile/4, TFT_LIGHTGREY);
    int menuzeile = getRotariesValue(SW_MENU);
    } 
    else {
    spr.drawRoundRect(spalte*10, (menuitem * zeile)+(zeile/5), spalte*5, zeile+2, zeile/4, TFT_LIGHTGREY);
    }    
    #else
    if ( wert_aendern == false ) {
      u8g2.setCursor(0, zeile+(menuitem*zeile)); u8g2.print("*");
    } 
    else {
    u8g2.setCursor(0, (zeile)+((menuitem)*zeile)); u8g2.print("-");
    }    
    #endif
    screen_zeigen();

    // Menupunkt zum Ändern ausgewählt
    if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) 
         && (menuitem < 3 )
         && (wert_aendern == false) ) {

      // debounce
      delay(10);  
      while( digitalRead(SELECT_SW) == SELECT_PEGEL )
        ;
      delay(10);
           
      switch (menuitem) { 
        case 0: initRotaries(SW_MENU, autostart, 0, 1, 1);
                break;
        case 1: initRotaries(SW_MENU, autokorrektur, 0, 1, 1);
                break;
        case 2: initRotaries(SW_MENU, kulanz_gr, 0, 99, 1);
                break;
      }
      wert_aendern = true;
    }

    // Änderung im Menupunkt übernehmen
    if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) 
         && (menuitem < 3 )
         && (wert_aendern == true) ) {

      // debounce
      delay(10);
      while( digitalRead(SELECT_SW) == SELECT_PEGEL )
        ;
      delay(10);

      initRotaries(SW_MENU, menuitem, 0, 3, -1);
      wert_aendern = false;
    }

    // Menu verlassen 
    if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) && (menuitem == 4) ) {
      u8g2.setCursor(spalte*11, zeile+(menuitem*zeile));
      u8g2.print("OK");
      screen_zeigen();
      delay(1000);
      i = 0;
    }
  }
}

void setupFuellmenge(void) {
    int j,k;
    int blinktime;
    initRotaries(SW_MENU, fmenge_index, 0, 4, -1);
    u8g2.setFont(FRONT_MENU);
    int zeile = MENU_HEIGHT/6;
    int spalte = u8g2.getUTF8Width("A");
    i = 1;
    while (i > 0) {
      if ((digitalRead(button_stop_pin)) == HIGH)
         return;
      
      pos = getRotariesValue(SW_MENU);
      clearScreen();
      j = 0;
      while( j < 5  ) {
        u8g2.setCursor(spalte, zeile+(j*zeile));    
        sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]);
        u8g2.print(ausgabe);
        j++;
      }
      #ifdef Use_MENU_FRAMES
      spr.drawRoundRect(4, (getRotariesValue(SW_MENU) * zeile)+(zeile/5), spalte*14, zeile+2, zeile/4, TFT_LIGHTGREY);
      int selectzeile=getRotariesValue(SW_MENU);
      #else
      u8g2.setFont(FRONT_MENU);
      u8g2.setCursor(0, zeile+(getRotariesValue(SW_MENU)*zeile));
      u8g2.print("*");
      #endif
      screen_zeigen();

      if ( digitalRead(SELECT_SW) == SELECT_PEGEL ) { // Füllmenge gewählt
         delay(500);

         initRotaries(SW_MENU, glaeser[pos].Gewicht, 30, 1000, 5);
          k = 1;
          while (k > 0){

          if ((digitalRead(button_stop_pin)) == HIGH) return;
            blinktime = (millis()/10) % 5;
            glaeser[pos].Gewicht = getRotariesValue(SW_MENU); 
            clearScreen();
             
              
      j = 0;
      while( j < 5  ) {
        u8g2.setCursor(spalte, zeile+(j*zeile));
        if (j == pos){ 
          if (blinktime < 3) { sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]);  } 
          else { sprintf(ausgabe, "%5s %3s","   ",GlasTypArray[glaeser[j].GlasTyp]);}
          }
        else {sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]);}
        u8g2.print(ausgabe);
        j++;
        }
        #ifdef Use_MENU_FRAMES
        spr.drawRoundRect(spalte, (selectzeile * zeile)+(zeile/5), spalte*6, zeile+2, zeile/4, TFT_LIGHTGREY);
        #endif
                    
        screen_zeigen();

             if ( digitalRead(SELECT_SW) == SELECT_PEGEL ) { // Gewicht bestätigt
                delay(500);
                initRotaries(SW_MENU, glaeser[pos].GlasTyp, 0, 2, 1);

                while (k > 0){ 

                  if ((digitalRead(button_stop_pin)) == HIGH) return;
                 blinktime = (millis()/10) % 5; 
                 glaeser[pos].GlasTyp = getRotariesValue(SW_MENU);
                 clearScreen();

      j = 0;
      while( j < 5  ) {
        u8g2.setCursor(spalte, zeile+(j*zeile));
        if (j == pos){ 
          if (blinktime < 3) {sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]);} 
          else {sprintf(ausgabe, "%4dg %3s",glaeser[pos].Gewicht,"  ");}
          }
        else {sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]);}
        u8g2.print(ausgabe);
        j++;
        }
        #ifdef Use_MENU_FRAMES
        spr.drawRoundRect(spalte*6, (selectzeile * zeile)+(zeile/5), spalte*5, zeile+2, zeile/4, TFT_LIGHTGREY);
        #endif

        screen_zeigen();


          if ( digitalRead(SELECT_SW) == SELECT_PEGEL ) { //GlasTyp bestätigt
             clearScreen();
             j = 0;
            while( j < 5  ) {
            u8g2.setCursor(spalte, zeile+(j*zeile));    
            sprintf(ausgabe, "%4dg %3s", glaeser[j].Gewicht, GlasTypArray[glaeser[j].GlasTyp]);
            u8g2.print(ausgabe);
            j++;
            }

        u8g2.setCursor(spalte*11, zeile+(zeile*pos));    
        u8g2.print("OK");
        screen_zeigen();
        delay(1000);
        k = 0; //raus

        }
     }
        
   }
}  
        fmenge = glaeser[pos].Gewicht;
        tara   = glaeser[pos].Tara;
        fmenge_index = pos; 
        i = 0;
      }
    }
}

void setupParameter(void) {
  int menuitem;
  int lastbuzzer    = buzzermode;
  int lastsetup     = setup_modern;
  bool wert_aendern = false;

  initRotaries(SW_MENU, 0, 0, 2, -1);

  i = 1;
  while (i > 0) {
    if ((digitalRead(button_stop_pin)) == HIGH) {
       buzzermode   = lastbuzzer;
       setup_modern = lastsetup;
       return;
    }

    if ( wert_aendern == false ) {
      menuitem = getRotariesValue(SW_MENU);
      if ( menuitem == 2 )
        menuitem = 4;  // Eine Zeile Abstand zu "Speichern"
    } else {
      switch (menuitem) {
        case 0: buzzermode    = getRotariesValue(SW_MENU);
                break;
        case 1: setup_modern  = getRotariesValue(SW_MENU);
                break;
      }
    }

    // Menu
    u8g2.setFont(FRONT_MENU);
    int zeile = MENU_HEIGHT/6;
    int spalte = u8g2.getUTF8Width("A")+1;

    clearScreen();
    sprintf(ausgabe,"Buzzer    %3s", (buzzermode==0?"aus":"ein"));     
    u8g2.setCursor(spalte, zeile);    u8g2.print(ausgabe);
    sprintf(ausgabe,"Menu   %6s", (setup_modern==0?" Liste":"Scroll"));     
    u8g2.setCursor(spalte, zeile*2);    u8g2.print(ausgabe);
    u8g2.setCursor(spalte, zeile*5);    u8g2.print("Speichern");

    // Positionsanzeige im Menu. "*" wenn nicht ausgewählt, Pfeil wenn ausgewählt
    //if ( wert_aendern == false ) {
    //   u8g2.setCursor(0, (zeile)+((menuitem)*zeile)); u8g2.print("*");
    //} else {
    //   u8g2.setCursor(0, (zeile)+((menuitem)*zeile)); u8g2.print("-");
    //}
        #ifdef Use_MENU_FRAMES
    if ( wert_aendern == false ) {
    spr.drawRoundRect(4, (menuitem * zeile)+(zeile/5), spalte*14, zeile+2, zeile/4, TFT_LIGHTGREY);
    int menuzeile = getRotariesValue(SW_MENU);
    } 
    else {
    spr.drawRoundRect(spalte*8, (menuitem * zeile)+(zeile/5), spalte*6, zeile+2, zeile/4, TFT_LIGHTGREY);
    }    
    #else
    if ( wert_aendern == false ) {
      u8g2.setCursor(0, zeile+(menuitem*zeile)); u8g2.print("*");
    } 
    else {
    u8g2.setCursor(0, (zeile)+((menuitem)*zeile)); u8g2.print("-");
    }    
    #endif

    screen_zeigen();
    
    // Menupunkt zum Ändern ausgewählt
    if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) 
         && (menuitem < 2 )
         && (wert_aendern == false) ) {

         // debounce
         delay(10);  
         while( digitalRead(SELECT_SW) == SELECT_PEGEL )
            ;
         delay(10);
           
         switch (menuitem) { 
           case 0: initRotaries(SW_MENU, buzzermode, 0, 1, 1);
                   break;
           case 1: initRotaries(SW_MENU, setup_modern, 0, 1, 1);
                   break;
         }
         wert_aendern = true;
      }

      // Änderung im Menupunkt übernehmen
      if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) 
           && (menuitem < 2 )
           && (wert_aendern == true) ) {

         // debounce
         delay(10);
         while( digitalRead(SELECT_SW) == SELECT_PEGEL )
            ;
         delay(10);

         initRotaries(SW_MENU, menuitem, 0, 2, -1);
         wert_aendern = false;
      }

      // Menu verlassen 
      if ( (digitalRead(SELECT_SW) == SELECT_PEGEL) && (menuitem == 4) ) {
        u8g2.setCursor(spalte*11, ((menuitem+1)*zeile));
        u8g2.print("OK");
        screen_zeigen();
                
        delay(1000);
        i = 0;
      }
    }
}

void setupClearPrefs(void) {
  initRotaries(SW_MENU, 1, 0, 1, -1);
  
  i = 1;
  while (i > 0) {
    if ((digitalRead(button_stop_pin)) == HIGH)
       return;
    
    pos = getRotariesValue(SW_MENU);
    u8g2.setFont(FRONT_MENU);
    int zeile = MENU_HEIGHT/6;
    int spalte = u8g2.getUTF8Width("A");
    clearScreen();
    u8g2.setCursor(spalte, zeile);    u8g2.print("Löschen");
    u8g2.setCursor(spalte, 2*zeile);    u8g2.print("Zurück!");
    
    u8g2.setCursor(0, zeile+((pos)*zeile));
    u8g2.print("*");
    screen_zeigen();
 
    if ((digitalRead(SELECT_SW)) == SELECT_PEGEL) {      
      u8g2.setCursor(spalte*11, zeile+((pos)*zeile));
      u8g2.print("OK");
      screen_zeigen();
      if ( pos == 0) {
        preferences.begin("EEPROM", false);
        preferences.clear();
        preferences.end();
        // gelöschte Werte einlesen, sonst bleiben die Variablen erhalten
        getPreferences();
      }
      delay(1000);
      i = 0;
    }
  }
}   

void processSetup(void) {
  if ( setup_modern == 0 ) 
     processSetupList();
  else
     processSetupScroll();
}

void processSetupList(void) {
  if ( modus != MODE_SETUP ) {
     modus = MODE_SETUP;
     winkel = winkel_min;          // Hahn schliessen
     servo_aktiv = 0;              // Servo-Betrieb aus
     SERVO_WRITE(winkel);
     rotary_select = SW_MENU;
     initRotaries(SW_MENU, 0, 0, 9, -1);
  }

  int menuitem = getRotariesValue(SW_MENU);

  u8g2.setFont(FRONT_MENU);
  int Zeile = (MENU_HEIGHT-(MENU_HEIGHT/10))/5;
  int spalte = u8g2.getUTF8Width("A");
  clearScreen();
  if( menuitem < 5 ) {
     u8g2.setCursor(spalte, zeile);   u8g2.print("Tara");
     u8g2.setCursor(spalte, 2*zeile);   u8g2.print("Kalibrieren");
     u8g2.setCursor(spalte, 3*zeile);   u8g2.print("Korrektur");
     u8g2.setCursor(spalte, 4*zeile);   u8g2.print("Füllmenge");
     u8g2.setCursor(spalte, 5*zeile);   u8g2.print("Automatik");
     u8g2.setFont(FRONT_MENU_GL);
     u8g2.setFontMode(1);
     spr.fillRoundRect (((spalte*14)-(spalte/3)), (zeile*4)+5, spalte*2, zeile+2, zeile/4, TFT_BLUE);
     spr.drawRoundRect (((spalte*14)-(spalte/3)), (zeile*4)+5, spalte*2, zeile+2, zeile/4, TFT_LIGHTGREY);
     u8g2.drawGlyph(spalte*14, 5*zeile+5, 0x40);  
  } else {
     u8g2.setCursor(spalte, zeile);   u8g2.print("Servowinkel");
     u8g2.setCursor(spalte, 2*zeile);   u8g2.print("Parameter");
     u8g2.setCursor(spalte, 3*zeile);   u8g2.print("Zähler");//Kud
     u8g2.setCursor(spalte, 4*zeile);   u8g2.print("Zähler Trip");//Kud     
     u8g2.setCursor(spalte, 5*zeile);   u8g2.print("Clear Prefs");
     u8g2.setFont(FRONT_MENU_GL);
     u8g2.setFontMode(1);
     spr.fillRoundRect (((spalte*14)-(spalte/3)), 0, spalte*2, zeile+4, zeile/4, TFT_BLUE);
     spr.drawRoundRect (((spalte*14)-(spalte/3)), 0, spalte*2, zeile+4, zeile/4, TFT_LIGHTGREY);
     u8g2.drawGlyph(spalte*14, zeile, 0x43);
     u8g2.setFont(FRONT_MENU);  
  }
  #ifdef Use_MENU_FRAMES
  spr.drawRoundRect(4, (((menuitem)%5) * zeile)+(zeile/5), spalte*13, zeile+2, zeile/4, TFT_LIGHTGREY);
  #else
  u8g2.setFont(FRONT_MENU);
  u8g2.setCursor(0, (zeile) + (((menuitem)%5) * zeile));
  u8g2.print("*");
  #endif
  screen_zeigen();

  if ( digitalRead(SELECT_SW) == SELECT_PEGEL ) {
    // sollte verhindern, dass ein Tastendruck gleich einen Unterpunkt wählt
    delay(250);
    while( digitalRead(SELECT_SW) == SELECT_PEGEL ) {
    }
#ifdef isDebug 
    Serial.print("Setup Position: ");
    Serial.println(menuitem);
#endif

    int lastpos = menuitem;
    if (menuitem == 0)   setupTara();              // Tara 
    if (menuitem == 1)   setupCalibration();       // Kalibrieren 
    if (menuitem == 2)   setupKorrektur();         // Korrektur 
    if (menuitem == 3)   setupFuellmenge();        // Füllmenge 
    if (menuitem == 4)   setupAutomatik();         // Autostart/Autokorrektur konfigurieren 
    if (menuitem == 5)   setupServoWinkel();       // Servostellungen Minimum, Maximum und Feindosierung
    if (menuitem == 6)   setupParameter();         // Sonstige Einstellungen
    if (menuitem == 7)   setupCounter();           // Kud Zählwerk Trip
    if (menuitem == 8)   setupTripCounter();       // Kud Zählwerk
    setPreferences();

    if (menuitem == 9)   setupClearPrefs();        // EEPROM löschen
    initRotaries(SW_MENU, lastpos, 0, 9, -1);      // Menu-Parameter könnten verstellt worden sein
  }
}

void processSetupScroll(void) {
  if ( modus != MODE_SETUP ) {
     modus = MODE_SETUP;
     winkel = winkel_min;          // Hahn schliessen
     servo_aktiv = 0;              // Servo-Betrieb aus
     SERVO_WRITE(winkel);
     rotary_select = SW_MENU;
     initRotaries(SW_MENU, 124, 0,255, -1);
  }
  int MenuepunkteAnzahl = 10;
  char *menuepunkte[] = {
    " Tarawerte","Kalibrieren"," Korrektur"," Füllmenge"," Automatik"," Servoeinst."," Parameter","  Zählwerk","ZählwerkTrip","Clear Prefs"
  };
  int menuitem = getRotariesValue(SW_MENU);
  menuitem = menuitem % MenuepunkteAnzahl;

  clearScreen();
  //obere Zeile
  int oberpos = menuitem-1;
  if (menuitem == 0) 
    oberpos = (MenuepunkteAnzahl-1);

  u8g2.setFont(u8g2_font_courB08_tf);
  u8g2.setCursor(30,12);   
  u8g2.print(menuepunkte[oberpos]);
   
  //untere Zeile
  int unterpos = menuitem+1;
  if (unterpos == MenuepunkteAnzahl)
    unterpos=0;
  u8g2.setCursor(30,62);   
  u8g2.print(menuepunkte[unterpos]);

  //Mittelzeile
  
  u8g2.setFont(u8g2_font_courB12_tf);
  u8g2.setCursor(6, 38);   
  u8g2.print(menuepunkte[menuitem]);
  drawLine(1, 20, 120, 20,TFT_WHITE);
  drawLine(1, 47, 128, 47,TFT_WHITE);
  screen_zeigen();
  int lastpos = menuitem;
  
    if ( digitalRead(SELECT_SW) == SELECT_PEGEL ) {
    // sollte verhindern, dass ein Tastendruck gleich einen Unterpunkt wählt
    delay(250);
    while( digitalRead(SELECT_SW) == SELECT_PEGEL ) {}
#ifdef isDebug 
    Serial.print("Setup Position: ");
    Serial.println(menuitem);
#endif

    int lastpos = menuitem;
    if (menuitem == 0)   setupTara();              // Tara 
    if (menuitem == 1)   setupCalibration();       // Kalibrieren 
    if (menuitem == 2)   setupKorrektur();         // Korrektur 
    if (menuitem == 3)   setupFuellmenge();        // Füllmenge 
    if (menuitem == 4)   setupAutomatik();         // Autostart/Autokorrektur konfigurieren 
    if (menuitem == 5)   setupServoWinkel();       // Servostellungen Minimum, Maximum und Feindosierung
    if (menuitem == 6)   setupParameter();         // Sonstige Einstellungen
    if (menuitem == 7)   setupCounter();           // Kud Zählwerk
    if (menuitem == 8)   setupTripCounter();       // Kud Zählwerk Trip
    setPreferences();

    if (menuitem == 9)   setupClearPrefs();        // EEPROM löschen
    initRotaries(SW_MENU,lastpos, 0,255, -1); // Menu-Parameter könnten verstellt worden sein
  }
}

void processAutomatik(void) { 
  int zielgewicht;           // Glas + Korrektur
  long blinktime;
  static int autokorrektur_gr; 
  int erzwinge_servo_aktiv = 0;
  boolean voll = false; //Kud

  static int gewicht_vorher;    // Gewicht des vorher gefüllten Glases
  static long time_vorher;      // Messung der Durchlaufzeit
  static int sammler_num = 5;   // Anzahl identischer Messungen für Nachtropfen

  if ( modus != MODE_AUTOMATIK ) {
     modus = MODE_AUTOMATIK;
     winkel = winkel_min;          // Hahn schliessen
     servo_aktiv = 0;              // Servo-Betrieb aus
     SERVO_WRITE(winkel);
     auto_aktiv = 0;               // automatische Füllung starten
     tara_glas = 0;
     rotary_select = SW_WINKEL;    // Einstellung für Winkel über Rotary
     initRotaries(SW_MENU, fmenge_index, 0, 4, 1);
     gewicht_vorher = glaeser[fmenge_index].Gewicht + korrektur;
     autokorrektur_gr = 0;
  }

  pos = getRotariesValue(SW_WINKEL);
  // nur bis winkel_fein regeln, oder über initRotaries lösen?
  if ( pos < ((winkel_fein*100)/winkel_max) ) {                      
    pos = ((winkel_fein*100)/winkel_max);
    setRotariesValue(SW_WINKEL, pos);
  }

  #ifdef USE_ROTARY                                                    // TODO: kann das Poti hier überhaupt etwas ändern?
  korrektur    = getRotariesValue(SW_KORREKTUR);
  fmenge_index = getRotariesValue(SW_MENU);
  #endif
  fmenge       = glaeser[fmenge_index].Gewicht;
  tara         = glaeser[fmenge_index].Tara;
  if ( tara <= 0 ) 
    auto_aktiv = 0;

  // wir starten nur, wenn das Tara für die Füllmenge gesetzt ist!
  // Ein erneuter Druck auf Start erzwingt die Aktivierung des Servo
  if (((digitalRead(button_start_pin)) == HIGH) && (tara > 0)) {
    // debounce
    delay(10);  
    while( digitalRead(button_start_pin) == HIGH )
       ;
    delay(10);

    if ( auto_aktiv == 1 ) {
      erzwinge_servo_aktiv = 1;
    #ifdef isDebug
      Serial.println("erzwinge Servo aktiv");      
    #endif
    }
    auto_aktiv    = 1;             // automatisches Füllen aktivieren
    rotary_select = SW_WINKEL;     // falls während der Parameter-Änderung auf Start gedrückt wurde    
    setPreferences();              // falls Parameter über den Rotary verändert wurden
  }
  
  if ((digitalRead(button_stop_pin)) == HIGH) {
    winkel      = winkel_min;
    servo_aktiv = 0;
    auto_aktiv  = 0;
    tara_glas   = 0;
    autokorrektur_gr = 0;  
  }
  // Fehlerkorrektur der Waage, falls Gewicht zu sehr schwankt 
  #ifdef FEHLERKORREKTUR_WAAGE
  int Vergleichsgewicht = (int(SCALE_GETUNITS(SCALE_READS))) - tara;
  for (byte j = 0 ; j < 3; j++) { // Anzahl der Wiederholungen, wenn Abweichung zu hoch
    gewicht = (int(SCALE_GETUNITS(SCALE_READS))) - tara;
    if (abs(gewicht - Vergleichsgewicht) < 50)  // Abweichung für Fehlererkennung
      break; 
    delay(100);
  }
  #else
  gewicht = (int(SCALE_GETUNITS(SCALE_READS))) - tara;
  #endif 
  
  // Glas entfernt -> Servo schliessen
  if (gewicht < -20) {
    winkel      = winkel_min;
    servo_aktiv = 0;
    tara_glas   = 0;
    if ( autostart != 1 ) {  // Autostart nicht aktiv
      auto_aktiv  = 0;
    }
    }

  // Automatik ein, leeres Glas aufgesetzt, Servo aus -> Glas füllen
  if ((auto_aktiv == 1) && (abs(gewicht) <= glastoleranz) && (servo_aktiv == 0)) {
    rotary_select = SW_WINKEL;     // falls während der Parameter-Änderung ein Glas aufgesetzt wird    
    clearScreen();
    u8g2.setFont(u8g2_font_courB24_tf);
    u8g2.setCursor(15, 43);
    u8g2.print("START");
    screen_zeigen();
    // kurz warten und prüfen ob das Gewicht nicht nur eine zufällige Schwankung war 
    delay(1500);  
    gewicht = (int(SCALE_GETUNITS(SCALE_READS))) - tara;
    voll = false; //Kud
    gezaehlt = false; //Kud

    if ( abs(gewicht) <= glastoleranz ) {
      tara_glas   = gewicht;
  #ifdef isDebug 
      Serial.print("gewicht: ");            Serial.print(gewicht);
      Serial.print(" gewicht_vorher: ");    Serial.print(gewicht_vorher);
      Serial.print(" zielgewicht: ");       Serial.print(fmenge + korrektur + tara_glas + autokorrektur_gr);
      Serial.print(" kulanz: ");            Serial.print(kulanz_gr);
      Serial.print(" Autokorrektur: ");     Serial.println(autokorrektur_gr);
  #endif      
      servo_aktiv = 1;
      sammler_num = 0;
      buzzer(BUZZER_SHORT);
    }
  }
  zielgewicht = fmenge + korrektur + tara_glas + autokorrektur_gr;

  // Anpassung des Autokorrektur-Werts
  if ( autokorrektur == 1 )
  {                                                       
    if ( (auto_aktiv == 1)                                // Automatik ist aktiviert
       && (servo_aktiv == 0 ) && (winkel == winkel_min)   // Hahn ist geschlossen
       && (gewicht >= zielgewicht )                       // Glas ist voll
       && (sammler_num <= 5)                              // tropfmenge noch nicht erfasst
       ) {     
    voll = true;//Kud                          
    if ( (gewicht == gewicht_vorher) && (sammler_num < 5) ) {   // wir wollen 5x das identische Gewicht sehen  
      sammler_num++;
    } else if ( gewicht != gewicht_vorher ) {             // sonst gewichtsänderung nachführen
      gewicht_vorher = gewicht;
      sammler_num = 0;
    } else if ( sammler_num == 5 ) {                      // gewicht ist 5x identisch, autokorrektur bestimmen
      autokorrektur_gr = (fmenge + kulanz_gr + tara_glas) - (gewicht - autokorrektur_gr);
      if ( korrektur + autokorrektur_gr > kulanz_gr ) {   // Autokorrektur darf nicht überkorrigieren, max Füllmenge plus Kulanz
        autokorrektur_gr = kulanz_gr - korrektur;
  #ifdef isDebug
        Serial.print("Autokorrektur begrenzt auf ");
        Serial.println(autokorrektur_gr);
  #endif
      }
      buzzer(BUZZER_SUCCESS);
      sammler_num++;                                      // Korrekturwert für diesen Durchlauf erreicht
    }

    if ((voll == true) && (gezaehlt == false)) { //Kud
      glaeser[fmenge_index].TripCount++;
      glaeser[fmenge_index].Count++;
      setPreferences();
      gezaehlt = true;
    }
  #ifdef isDebug
      Serial.print("Nachtropfen:");
      Serial.print(" gewicht: ");        Serial.print(gewicht);
      Serial.print(" gewicht_vorher: "); Serial.print(gewicht_vorher);
      Serial.print(" sammler_num: ");    Serial.print(sammler_num);
      Serial.print(" Korrektur: ");      Serial.println(autokorrektur_gr);
      Serial.print(" Zähler Trip: ");    Serial.print(glaeser[fmenge_index].TripCount); //Kud
      Serial.print(" Zähler: ");         Serial.println(glaeser[fmenge_index].Count); //Kud
  #endif
    }
  }

  // Glas ist teilweise gefüllt. Start wird über Start-Taster erzwungen
  if ((auto_aktiv == 1) && (gewicht > 5) && (erzwinge_servo_aktiv == 1) ) {
    servo_aktiv = 1;
    voll = false; //Kud
    gezaehlt = false;//Kud
    buzzer(BUZZER_SHORT);
  }
  
  if (servo_aktiv == 1) {
    winkel = ((winkel_max * pos) / 100);
  }
  
  if ((servo_aktiv == 1) && (( zielgewicht - gewicht ) <= fein_dosier_gewicht)) {
    winkel = ( ((winkel_max*pos) / 100) * ((zielgewicht-gewicht) / fein_dosier_gewicht) );
  }
  
  if ((servo_aktiv == 1) && (winkel <= winkel_fein)) {
    winkel = winkel_fein;
  }
  
  // Glas ist voll
  if ((servo_aktiv == 1) && (gewicht >= zielgewicht)) {
    winkel      = winkel_min;
    servo_aktiv = 0;

    if (gezaehlt == false) { //Kud
      glaeser[fmenge_index].TripCount++;
      glaeser[fmenge_index].Count++;
      setPreferences();
      gezaehlt = true;
    }
    if ( autostart != 1 )       // autostart ist nicht aktiv, kein weiterer Start
      auto_aktiv = 0;
    if ( autokorrektur == 1 )   // autokorrektur, gewicht merken
      gewicht_vorher = gewicht;
    buzzer(BUZZER_SHORT);
  }
  
  SERVO_WRITE(winkel);
  
  #ifdef isDebug
  #if isDebug >= 4
    Serial.print("Automatik:");  
    Serial.print(" Gewicht: ");        Serial.print(gewicht);
    Serial.print(" Winkel: ");         Serial.print(winkel);
  //    Serial.print(" Dauer ");           Serial.print(millis() - scaletime);
  //    Serial.print(" Füllmenge: ");      Serial.print(fmenge);
  //    Serial.print(" Korrektur: ");      Serial.print(korrektur);
  //    Serial.print(" Tara_glas:");       Serial.print(tara_glas);
    Serial.print(" Autokorrektur: ");  Serial.print(autokorrektur_gr);
    Serial.print(" Zielgewicht ");     Serial.print(zielgewicht);
  //    Serial.print(" Erzwinge Servo: "); Serial.print(erzwinge_servo_aktiv);
  //    Serial.print(" servo_aktiv ");     Serial.print(servo_aktiv);
    Serial.print(" auto_aktiv ");      Serial.println(auto_aktiv);
  #endif 
  #endif
  time_vorher = millis();

  clearScreen();

  // Gewicht blinkt, falls unter der definierten Füllmenge
  // Korrekturfaktor und Füllmenge blinken, wenn sie über den Rotary verstellt werden
  blinktime = (millis()/10) % 5;

  // wenn kein Tara für unser Glas definiert ist, wird kein Gewicht sondern eine Warnung ausgegeben
  if ( tara > 0 ) {
    // kein Glas aufgestellt 
    if ( gewicht < -20 ) {
      u8g2.setFont(u8g2_font_courB12_tf);
      u8g2.setCursor(28, 30); u8g2.print("Bitte Glas");
      u8g2.setCursor(28, 44); u8g2.print("aufstellen");
    } else {
      u8g2.setCursor(10, 42);
      u8g2.setFont(u8g2_font_courB24_tf);
   
      if( (autostart == 1) && (auto_aktiv == 1 ) && (servo_aktiv == 0) && (gewicht >= -5) && (gewicht - tara_glas < fmenge) && (blinktime < 2) ) {
        sprintf(ausgabe,"%5s", "     ");
      } else {
        sprintf(ausgabe,"%5dg", gewicht - tara_glas);
      }
      u8g2.print(ausgabe);
    }
  } else {
     u8g2.setCursor(42, 38);
     u8g2.setFont(u8g2_font_courB14_tf);
     sprintf(ausgabe,"%6s", "no tara!");
     u8g2.print(ausgabe);
  }

  // Play/Pause Icon, ob die Automatik aktiv ist
  u8g2.setFont(u8g2_font_open_iconic_play_2x_t);
  #ifdef use_TFT
  if (auto_aktiv==1) {
    u8g2.setForegroundColor(TFT_GREEN);
    }
  else {
    u8g2.setForegroundColor(TFT_RED);
  }
  u8g2.drawGlyph(0, 40, (auto_aktiv==1)?0x45:0x44 );
  u8g2.setForegroundColor(TFT_WHITE);

  #else
  u8g2.drawGlyph(0, 40, (auto_aktiv==1)?0x45:0x44 );
  #endif

  u8g2.setFont(u8g2_font_courB12_tf);
  // Zeile oben, Öffnungswinkel absolut und Prozent, Anzeige Autostart
  u8g2.setCursor(0, 11);
  sprintf(ausgabe,"W=%-3d %2s %3d%%", winkel, (autostart==1)?"AS":"  ", pos);
  u8g2.print(ausgabe);
  
  u8g2.setFont(u8g2_font_courB10_tf);
  // Zeile unten, aktuell zu verstellende Werte blinken. 
  // Verstellung nur wenn Automatik inaktiv, gesteuert über Interrupt-Funktion 
  if( autokorrektur == 1 ){
    u8g2.setCursor( 0, 64);
    u8g2.print("a");
    u8g2.setCursor(10, 64);
  } else {
    u8g2.setCursor( 0, 64);    
  }

  if(servo_aktiv == 1) {
    int progressbar = 128.0*((float)gewicht/(float)zielgewicht);
    progressbar = constrain(progressbar,0,128);

    
    
    drawBox  (0, 50, progressbar, 14,TFT_WHITE );
    drawFrame(0, 50, 128, 14,TFT_WHITE);
    screen_zeigen();
  }
  else
  {
  if( rotary_select == SW_KORREKTUR && blinktime < 2 ) {
    if (glaeser[fmenge_index].Gewicht > 999){
      sprintf(ausgabe,"k=   %s %3s-%3s",(autokorrektur==1)?"":" ", "1kg", GlasTypArray[glaeser[fmenge_index].GlasTyp]  );
    } else {
      sprintf(ausgabe,"k=   %s %3d-%3s",(autokorrektur==1)?"":" ", glaeser[fmenge_index].Gewicht, GlasTypArray[glaeser[fmenge_index].GlasTyp] ); 
    }
  } else if ( rotary_select == SW_MENU && blinktime < 2 ) {
    sprintf(ausgabe,"k=%-3d" , korrektur + autokorrektur_gr, (autokorrektur==1)?"":" " );
  } else {
      if (glaeser[fmenge_index].Gewicht > 999){
        sprintf(ausgabe,"k=%-3d%s %3s-%3s", korrektur + autokorrektur_gr, (autokorrektur==1)?"":" ", "1kg", GlasTypArray[glaeser[fmenge_index].GlasTyp] );
      } else {
        sprintf(ausgabe,"k=%-3d%s %3d-%3s", korrektur + autokorrektur_gr, (autokorrektur==1)?"":" ", glaeser[fmenge_index].Gewicht, GlasTypArray[glaeser[fmenge_index].GlasTyp] ); 
      }
  }
  u8g2.print(ausgabe);
  screen_zeigen();
  }


  
  
}

void processHandbetrieb(void)
{
  static unsigned long scaletime;
  static unsigned long dauer;
  
  if ( modus != MODE_HANDBETRIEB ) {
     modus = MODE_HANDBETRIEB;
     winkel = winkel_min;          // Hahn schliessen
     servo_aktiv = 0;              // Servo-Betrieb aus
     SERVO_WRITE(winkel);
     rotary_select = SW_WINKEL;
     tara = 0;
  }

  pos = getRotariesValue(SW_WINKEL);
  gewicht = SCALE_GETUNITS(SCALE_READS) - tara;

  if ((digitalRead(button_start_pin)) == HIGH) {
    servo_aktiv = 1;
  }
  
  if ((digitalRead(button_stop_pin)) == HIGH) {
    servo_aktiv = 0;
  }

#ifdef USE_ROTARY_SW
  if ( ((digitalRead(outputSW)) == LOW) /*&& (tara == 0) */ ) {  // sonst muss der Taster entprellt werden!
      tara = SCALE_GETUNITS(SCALE_READS);
  }
#endif

  if (servo_aktiv == 1) {
    winkel = ((winkel_max * pos) / 100);
  } else { 
    winkel = winkel_min;
  }
  winkel = constrain(winkel, winkel_min, winkel_max);
  SERVO_WRITE(winkel);

#ifdef isDebug
#if isDebug >= 4
    Serial.print("Handbetrieb:");  
    Serial.print(" Gewicht ");     Serial.print(gewicht);
    Serial.print(" Winkel ");      Serial.print(winkel);
    Serial.print(" Dauer ");       Serial.print(millis() - scaletime);
    Serial.print(" servo_aktiv "); Serial.println(servo_aktiv);
#endif
#endif
  scaletime = millis();
  // Ausgabe OLED. Dauert ca. 170ms
  clearScreen();

  u8g2.setFont(u8g2_font_courB24_tf);
  u8g2.setCursor(10, 42);
  sprintf(ausgabe,"%5dg", gewicht);
//  sprintf(ausgabe,"%5dg", dauer);
  u8g2.print(ausgabe);

  u8g2.setFont(u8g2_font_open_iconic_play_2x_t);
  u8g2.drawGlyph(0, 40, (servo_aktiv==1)?0x45:0x44 );

  u8g2.setFont(u8g2_font_courB12_tf);
  u8g2.setCursor(0, 11);
  sprintf(ausgabe,"W=%-3d    %3d%%", winkel, pos);
  u8g2.print(ausgabe);
  u8g2.setCursor(0, 64);
  sprintf(ausgabe, "Manuell  %s", (tara>0?"Tara":"    "));
  u8g2.print(ausgabe);

  screen_zeigen();
  dauer = millis() - scaletime;
}

void setup()
{
  // enable internal pull downs for digital inputs 
  pinMode(button_start_pin, INPUT_PULLDOWN);
  pinMode(button_stop_pin, INPUT_PULLDOWN);
  pinMode(switch_betrieb_pin, INPUT_PULLDOWN);
  pinMode(switch_setup_pin, INPUT_PULLDOWN);
  pinMode(vext_ctrl_pin, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_pin, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
  }
#ifdef isDebug
    Serial.println("Hanimandl Start");
    Serial.print("Debuglevel = ");   Serial.println(isDebug);
    Serial.print("Hardwarelevel = ");   Serial.println(HARDWARE_LEVEL);
    #ifdef use_TFT
    Serial.println("TFT_Biblioteken geladen");
    #endif
#endif
  
  // Rotary
#ifdef USE_ROTARY_SW
  pinMode(outputSW, INPUT_PULLUP);
  attachInterrupt(outputSW, isr1, FALLING);
#endif
#ifdef USE_ROTARY
  pinMode(outputA,INPUT);
  pinMode(outputB,INPUT);
  attachInterrupt(outputA, isr2, CHANGE);
#endif

// switch Vcc / GND on normal pins for convenient wiring
// output is 3.3V for VCC
  digitalWrite (switch_vcc_pin, HIGH); 
  digitalWrite (button_start_vcc_pin, HIGH); 
  digitalWrite (button_stop_vcc_pin, HIGH); 
  
//  pinMode (_GND, OUTPUT);     // turn on GND pin first (important!)
// turn on VCC power
  pinMode (switch_vcc_pin, OUTPUT);
  pinMode (button_start_vcc_pin, OUTPUT);
  pinMode (button_stop_vcc_pin, OUTPUT);

// Buzzer
  pinMode(buzzer_pin, OUTPUT);

  // short delay to let chip power up
  delay (100); 

// Preferences aus dem EEPROM lesen
  getPreferences();

// Servo initialisieren und schliessen
#ifdef SERVO_ERWEITERT
  servo.attach(servo_pin,  750, 2500); // erweiterte Initialisierung, steuert nicht jeden Servo an
#else
  servo.attach(servo_pin, 1000, 2000); // default Werte. Achtung, steuert den Nullpunkt weniger weit aus!
#endif
  SERVO_WRITE(winkel_min);

// Waage erkennen - machen wir vor dem Boot-Screen, dann hat sie 3 Sekunden Zeit zum aufwärmen
  scale.begin(hx711_dt_pin, hx711_sck_pin);
  if (scale.wait_ready_timeout(1000)) {               // Waage angeschlossen?
    scale.power_up();
    waage_vorhanden = 1;
#ifdef isDebug
      Serial.println("Waage erkannt");
#endif
  }

// Boot Screen
#ifdef use_TFT
tft.init();  // initialize
tft.setRotation(RotateScreen);
tft.fillScreen(TFT_BLACK);
tft.setTextDatum(BL_DATUM);

spr.setColorDepth(8);
#if (RotateScreen == 1||RotateScreen == 1)
spr.createSprite(TFT_HEIGHT, TFT_WIDTH);
#else 
spr.createSprite(TFT_WIDTH, TFT_HEIGHT);
#endif
spr.setTextDatum(BL_DATUM);

u8g2.begin(spr);                     // connect u8g2 procedures to TFT_eSPI
u8g2.setFontMode(0);                 // use u8g2 none transparent mode
u8g2.setFontDirection(0);            // left to right (this is default)
u8g2.setForegroundColor(TFT_WHITE);  // apply color
#else
  u8g2.setBusClock(800000);   // experimental
  u8g2.begin();
  u8g2.enableUTF8Print();
  clearScreen();
#endif
  print_logo();
  buzzer(BUZZER_SHORT);
  delay(3000);

// Setup der Waage, Skalierungsfaktor setzen
  if (waage_vorhanden ==1 ) {                         // Waage angeschlossen?
    if ( faktor == 0 ) {                              // Vorhanden aber nicht kalibriert
      viewMessageFull("Nicht","kalibr.");
#ifdef isDebug
      Serial.println("Waage nicht kalibriert!");
#endif
      delay(2000);
    } else {                                          // Tara und Skalierung setzen
      scale.set_scale(faktor);
      scale.set_offset(long(gewicht_leer));
#ifdef isDebug
      Serial.println("Waage initialisiert");
#endif
    }
  } else {                                            // Keine Waage angeschlossen
    viewMessageFull("Keine","Waage!");
    buzzer(BUZZER_ERROR);
#ifdef isDebug
    Serial.println("Keine Waage!");
#endif
    delay(2000);
  }
  
// initiale Kalibrierung des Leergewichts wegen Temperaturschwankungen
// Falls mehr als 20g Abweichung steht vermutlich etwas auf der Waage.
  if (waage_vorhanden == 1) {
    gewicht = SCALE_GETUNITS(SCALE_READS);
    if ( (gewicht > -20) && (gewicht < 20) ) {
      scale.tare(10);
      buzzer(BUZZER_SUCCESS);
#ifdef isDebug
      Serial.print("Tara angepasst um: ");
      Serial.println(gewicht);
#endif
    } else if (faktor != 0) {
      viewMessageFull("Waage","leeren!");
#ifdef isDebug
        Serial.print("Gewicht auf der Waage: ");
        Serial.println(gewicht);
#endif
      delay(5000);

      // Neuer Versuch, falls Gewicht entfernt wurde
      gewicht = SCALE_GETUNITS(SCALE_READS);
      if ( (gewicht > -20) && (gewicht < 20) ) {
        scale.tare(10);
        buzzer(BUZZER_SUCCESS);
#ifdef isDebug
        Serial.print("Tara angepasst um: ");
        Serial.println(gewicht);
#endif
      } else {    // Warnton ausgeben
        buzzer(BUZZER_LONG);
      }
    }
  }
  
// die drei Datenstrukturen des Rotaries initialisieren
  initRotaries(SW_WINKEL,    0,   0, 100, 5 );     // Winkel
  initRotaries(SW_KORREKTUR, 0, -20,  20, 1 );     // Korrektur
  initRotaries(SW_MENU,      0,   0,   7, 1 );     // Menuauswahlen

// Parameter aus den Preferences für den Rotary Encoder setzen
  setRotariesValue(SW_WINKEL,    pos);   
  setRotariesValue(SW_KORREKTUR, korrektur);
  setRotariesValue(SW_MENU,      fmenge_index);
}

void loop()
{
  rotating = true;     // debounce Management
  
  // Setup Menu 
  if ((digitalRead(switch_setup_pin)) == HIGH)
    processSetup();

  // Automatik-Betrieb 
  if ((digitalRead(switch_betrieb_pin)) == HIGH)
    processAutomatik();

  // Handbetrieb 
  if ((digitalRead(switch_betrieb_pin) == LOW)
      && (digitalRead(switch_setup_pin) == LOW))
    processHandbetrieb();
}

void print_logo() {
#ifdef Logo1
  const unsigned char logo_biene1[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 
  0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x60, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC1, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x3F, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x70, 0x00, 0xF0, 0xFF, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0xFF, 0xFF, 0x80, 0xF1, 0x47, 0xF0, 0x07, 0x00, 0x3E, 0xE0, 0xFF, 0xFF, 0x07, 
  0xF9, 0x07, 0x7E, 0x00, 0x00, 0x78, 0xF0, 0x03, 0xE0, 0x1F, 0xF8, 0x07, 0x1F, 0x00, 0x00, 0x70, 0x3C, 0x00, 0x00, 0xFE, 0x38, 0xC0, 0x03, 0x00, 
  0x00, 0xF0, 0x0E, 0x00, 0x00, 0xF8, 0x03, 0xF8, 0x00, 0x00, 0x00, 0xE0, 0x06, 0x00, 0x00, 0xC0, 0x0F, 0x7C, 0x00, 0x00, 0x00, 0xE0, 0x06, 0x00, 
  0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x70, 0x03, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x70, 0x03, 0x00, 0x00, 0x00, 0xF0, 0x03, 
  0x00, 0x00, 0x00, 0x38, 0x03, 0x00, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x07, 0x00, 0x00, 0x0F, 
  0x0F, 0x00, 0x00, 0x78, 0x78, 0xE0, 0x3F, 0x00, 0xC0, 0x07, 0x3E, 0x00, 0x80, 0xFF, 0x3C, 0xC0, 0x7F, 0x00, 0xF0, 0x01, 0xFC, 0x00, 0xE0, 0xFF, 
  0x1C, 0x80, 0xFF, 0x01, 0x7E, 0x00, 0xF0, 0xFF, 0xFF, 0x3F, 0x0E, 0x00, 0xFE, 0xFF, 0x0F, 0x00, 0xC0, 0xFF, 0xFF, 0x07, 0x0F, 0x00, 0xC0, 0x1F, 
  0x00, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x07, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x80, 0x03, 0x80, 0x03, 0xE0, 0x00, 0x70, 0x00, 0x00, 0x00, 0xC0, 
  0x01, 0xC0, 0x03, 0xC0, 0x01, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xE0, 0x81, 0xC3, 0x01, 0xC0, 0x01, 0x00, 0x00, 0x70, 0x00, 0xE0, 0xF1, 0x8F, 
  0x03, 0x80, 0x03, 0x00, 0x00, 0x38, 0x00, 0xF0, 0xFC, 0x9F, 0x07, 0x00, 0x07, 0x00, 0x00, 0x1C, 0x00, 0xF8, 0x1C, 0x1C, 0x0F, 0x00, 0x06, 0x00, 
  0x00, 0x1C, 0x00, 0xFE, 0x00, 0x00, 0x1F, 0x00, 0x0C, 0x00, 0x00, 0x0E, 0x00, 0xF7, 0x00, 0x00, 0x7F, 0x00, 0x0C, 0x00, 0x00, 0x06, 0x80, 0x73, 
  0x00, 0x00, 0xE6, 0x00, 0x0C, 0x00, 0x00, 0x07, 0xE0, 0x71, 0x00, 0x00, 0xC6, 0x03, 0x0C, 0x00, 0x00, 0x07, 0x70, 0x70, 0xF0, 0x0F, 0x86, 0x07, 
  0x0C, 0x00, 0x00, 0x03, 0x3C, 0x70, 0xFC, 0x3F, 0x06, 0x1F, 0x0E, 0x00, 0x00, 0x03, 0x1E, 0x70, 0xFE, 0x3F, 0x06, 0xFC, 0x07, 0x00, 0x00, 0x87, 
  0x0F, 0x70, 0x1E, 0x38, 0x06, 0xF0, 0x03, 0x00, 0x00, 0xFE, 0x03, 0xF0, 0x00, 0x00, 0x06, 0xC0, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xF0, 0x00, 0x00, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x80, 0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0x0F, 0x07, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xE0, 0xF1, 0x9F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3B, 0x9C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
  0x07, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x0D, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
#endif
#ifdef Logo2  
 const unsigned char logo_biene1[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0xc0, 0x01, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x80, 0x03, 0x18, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xc0, 0x01, 0x06, 0x0e, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
   0x04, 0x02, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x18, 0x03,
   0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x18, 0x01, 0x0c, 0x00,
   0x00, 0x00, 0x00, 0xda, 0x03, 0x0e, 0x0c, 0x03, 0x06, 0xb8, 0x01, 0x00,
   0xc0, 0x00, 0x07, 0x0c, 0x1e, 0x07, 0x03, 0x0c, 0x30, 0x00, 0x40, 0x0c,
   0x3c, 0x1c, 0x9e, 0x0f, 0x83, 0x87, 0x22, 0x00, 0x00, 0x00, 0x60, 0x38,
   0x9e, 0xcf, 0xe1, 0x20, 0x00, 0x00, 0x80, 0xc1, 0xc9, 0xf1, 0xfe, 0xe7,
   0x39, 0x33, 0x30, 0x00, 0x00, 0xb0, 0x0d, 0xc7, 0x0d, 0x7b, 0x1e, 0x93,
   0x18, 0x00, 0x00, 0x83, 0x91, 0x9d, 0x03, 0x18, 0x9b, 0x3d, 0x04, 0x00,
   0x00, 0x1c, 0xe9, 0x7b, 0x02, 0xec, 0x69, 0x01, 0x02, 0x00, 0x00, 0x20,
   0x10, 0xf1, 0x01, 0x70, 0x08, 0x61, 0x00, 0x00, 0x00, 0x60, 0xfe, 0x00,
   0x03, 0x18, 0xf0, 0xcf, 0x00, 0x00, 0x00, 0x20, 0x11, 0xf8, 0xe1, 0xd8,
   0x83, 0x1d, 0x00, 0x00, 0x00, 0x60, 0x21, 0x46, 0xb0, 0x60, 0x84, 0xc8,
   0x00, 0x00, 0x00, 0x00, 0x3c, 0x80, 0x03, 0x18, 0x4c, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc2, 0x03, 0x78, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x35, 0xf1, 0x00, 0xe0, 0x89, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
   0x07, 0xcc, 0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0xec, 0x9f, 0x03, 0x98,
   0x7f, 0x07, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xc7, 0x0f, 0x3f, 0xfc, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf0, 0xff, 0xef, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
   0x00, 0xe0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0xff, 0xdf,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0xff, 0x9f, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x9e, 0xff, 0x1f, 0x07, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x1e, 0xf8, 0x03, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x06, 0x07, 0x0c, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87,
   0xff, 0x1f, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xff, 0x1f,
   0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xff, 0x0f, 0x18, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x01, 0xff, 0x0f, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x01, 0xfe, 0x0f, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01,
   0xfe, 0x0f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
   0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00
};  

#endif

 

          #ifdef use_TFT
          spr.drawXBitmap(0, 0,logo_biene1,80,64,TFT_YELLOW);
          u8g2.setFont(u8g2_font_courB14_tf);
          u8g2.setForegroundColor(TFT_BROWN);
          u8g2.setCursor(85, 27);    u8g2.print("HANI");
          u8g2.setCursor(75, 43);    u8g2.print("MANDL");
          u8g2.setForegroundColor(TFT_GREENYELLOW);
          u8g2.setFont(u8g2_font_courB08_tf);
          u8g2.setCursor(85, 64);    u8g2.print("v.0.2.9");
          u8g2.setForegroundColor(TFT_WHITE);
          #else
          clearScreen();
          u8g2.drawXBM(0,0,80,64,logo_biene1);
          u8g2.setFont(u8g2_font_courB14_tf);
          u8g2.setCursor(85, 27);    u8g2.print("HANI");
          u8g2.setCursor(75, 43);    u8g2.print("MANDL");
          u8g2.setFont(u8g2_font_courB08_tf);
          u8g2.setCursor(85, 64);    u8g2.print("v.0.2.9");
          #endif
          screen_zeigen();

}

// Wir nutzen einen aktiven Summer, damit entfällt die tone Library, die sich sowieso mit dem Servo beisst.
void buzzer(byte type) {
  if (buzzermode == 1) {
    switch (type) {
      case BUZZER_SHORT: //short
        digitalWrite(buzzer_pin,HIGH);
        digitalWrite(LED_pin,HIGH);
        delay(100);
        digitalWrite(buzzer_pin,LOW);
        digitalWrite(LED_pin,LOW);
        break;

      case BUZZER_LONG: //long
        digitalWrite(buzzer_pin,HIGH);
        digitalWrite(LED_pin,HIGH);
        delay(500);
        digitalWrite(buzzer_pin,LOW);
        digitalWrite(LED_pin,LOW);
        break;

      case BUZZER_SUCCESS: //success
        digitalWrite(buzzer_pin,HIGH);
        digitalWrite(LED_pin,HIGH);
        delay(100);
        digitalWrite(buzzer_pin,LOW);
        digitalWrite(LED_pin,LOW);
        delay(100);
        digitalWrite(buzzer_pin,HIGH);
        digitalWrite(LED_pin,HIGH);
        delay(100);
        digitalWrite(buzzer_pin,LOW);
        digitalWrite(LED_pin,LOW);
        delay(100);
        digitalWrite(buzzer_pin,HIGH);
        digitalWrite(LED_pin,HIGH);
        delay(100);
        digitalWrite(buzzer_pin,LOW);
        digitalWrite(LED_pin,LOW);
        break;

      case BUZZER_ERROR: //error
        digitalWrite(buzzer_pin,HIGH);
        digitalWrite(LED_pin,HIGH);
        delay(1500);
        digitalWrite(buzzer_pin,LOW);
        digitalWrite(LED_pin,LOW);    
        break;
    }
  }
}
void clearScreen(){
  #ifdef use_TFT
  //tft.fillScreen(TFT_BLACK);
  spr.fillSprite(TFT_BLACK);
  #else
  u8g2.clearBuffer();
  #endif
}
void drawLine(int x0,int y0, int x1, int y1, uint32_t color){
  #ifdef use_TFT
  spr.drawLine(x0,y0,x1,y1,color);
  #else
  u8g2.drawLine(x0,y0,x1,x0);
  #endif
}

void screen_zeigen(void) {
  #ifdef use_TFT
  #ifdef Use_Switch_Bar
  modusButton();
  #endif

  spr.pushSprite(OffsetX, OffsetY);

  #else
   u8g2.sendBuffer();
  #endif  
}
void drawFrame(int x,int y, int w, int h, uint32_t color){
  #ifdef use_TFT
  spr.drawRect(x,y,w,h,color);
  #else
  u8g2.drawFrame(0, 50, 128, 14);
  #endif
}
void drawBox(int x,int y, int w, int h, uint32_t color){
  #ifdef use_TFT
  spr.fillRect (x,y,w,h,color);
  #else
  u8g2.drawBox(x, y, w, h);
  #endif
}
void PrintCenter(int x,int y,int w,const char* text){
  int center=w/2-u8g2.getUTF8Width(text)/2;
  u8g2.setCursor( x+center, y); 
  u8g2.print(text);
}
void PrintRight(int x,int y,int w,const char* text){
  int RightPos=w-u8g2.getUTF8Width(text);
  u8g2.setCursor( RightPos+x, y); 
  u8g2.print(text);
}
void viewMessageFull (const char* zeile1, const char* zeile2){
  
  //todo : Funtion ist aktuell eher Holzfällermetode, geht sicher einiges schöner.
  // Offset durch diese Funktion auch nicht mehr möglich evtl.noch mit einbauen z.B."PrintCenter( 0, 2*yteil,TFT_HEIGHT-(2xOffsetX),zeile1);""
  clearScreen();
  #ifdef use_TFT
  u8g2.setFont(BIG_FRONT);
  int yteil = TFT_WIDTH/5;    
  PrintCenter( 0, 2*yteil,TFT_HEIGHT,zeile1);
  PrintCenter( 0, 3*yteil,TFT_HEIGHT,zeile2);
  u8g2.setFont(u8g2_font_courB10_tf);
  #else
  u8g2.setFont(u8g2_font_courB18_tf);
  u8g2.setCursor( 24, 24); u8g2.print(zeile1);
  u8g2.setCursor( 10, 56); u8g2.print(zeile2);
  #endif
  screen_zeigen();
}

void modusButton(){
u8g2.setFont(FRONT_MENU);
u8g2.setFontMode(1);
int hoehe = u8g2.getFontAscent();
if (modus ==MODE_AUTOMATIK) {spr.fillRoundRect (10, MENU_HEIGHT, 90, 40, 10, TFT_DARKGREY);}
else      {spr.fillRoundRect (10, MENU_HEIGHT, 90, 40, 10, TFT_BLUE);}
spr.drawRoundRect(10, MENU_HEIGHT, 90, 40, 10, TFT_LIGHTGREY);
u8g2.setCursor( 15, MENU_HEIGHT+hoehe+8);    
u8g2.print("AUTO");

if (modus ==MODE_HANDBETRIEB) {spr.fillRoundRect (110, MENU_HEIGHT, 90, 40, zeile/4, TFT_DARKGREY);}
else      {spr.fillRoundRect (110, MENU_HEIGHT, 90, 40, zeile/4, TFT_BLUE);}
spr.drawRoundRect (110, MENU_HEIGHT, 90, 40, zeile/4,TFT_LIGHTGREY);
u8g2.setCursor( 125, MENU_HEIGHT+hoehe+8); 
u8g2.print("MAN");

if (modus ==MODE_SETUP) {spr.fillRoundRect (210, MENU_HEIGHT, 110, 40, zeile/4,TFT_DARKGREY);}
else      {spr.fillRoundRect (210, MENU_HEIGHT, 110, 40, zeile/4, TFT_BLUE);}
spr.drawRoundRect (210, MENU_HEIGHT, 110, 40, zeile/4, TFT_LIGHTGREY);
u8g2.setCursor( 217, MENU_HEIGHT+hoehe+8); 
u8g2.print("Setup");
}