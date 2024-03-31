
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3uvN0YHod"
#define BLYNK_TEMPLATE_NAME "Efficient Food Waste Management System"
#define BLYNK_AUTH_TOKEN "5ExD254Iya4_2ayNrSC5JnrxpQOvpLaD"

#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>
#include "HX711.h"

#define calibration_factor -7050.0
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

HX711 scale;

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "EFWM";        
char pass[] = "12345678";    

void setup() {
  Serial.begin(9600);  
  Blynk.begin(auth, ssid, pass);

  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); 
}

void loop() {
  float weight = map(scale.get_units(), -70, -200, 0, 5); 
  Serial.print("Food Waste Quantity : ");
  Serial.print(weight, 1);
  Serial.println(" kg");
  
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Food Waste: ")
  lcd.print(weight, 0);
  lcd.print(" kg");
  lcd.noBlink();
  
  Blynk.virtualWrite(V1, weight); 
  delay(1000);
}
