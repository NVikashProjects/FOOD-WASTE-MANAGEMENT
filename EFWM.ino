
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3uvN0YHod"
#define BLYNK_TEMPLATE_NAME "Efficient Food Waste Management System"
#define BLYNK_AUTH_TOKEN "5ExD254Iya4_2ayNrSC5JnrxpQOvpLaD"

#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>
#include "HX711.h"

#define calibration_factor -7050.0
#define LOADCELL_DOUT_PIN 3
#define LOADCELL_SCK_PIN 2
#define CAMARA 14

HX711 scale;

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "EFWM";
char pass[] = "12345678";

float AfterWeight, currentWeight = 0.0;
int personCount = 0;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(CAMARA, OUTPUT);

  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
}

void loop() {
  float weight = map(scale.get_units(), -70, -200, 0.0, 5.0);
  float millweight = map(scale.get_units(), -76, -200, 0.0, 5000.0);
  Serial.print("Food Waste Quantity : ");
  Serial.print(weight, 1);
  Serial.println(" kg   ");
  Serial.print(millweight, 1);
  Serial.println(" gram");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Food Waste: ");
  lcd.print(millweight, 1);
  lcd.print(" kg");

  lcd.setCursor(0, 1);
  lcd.print("Persons : ");
  lcd.print(personCount);
  
  lcd.noBlink();

  if ( millweight > currentWeight) {
    AfterWeight = millweight - currentWeight;

    if (AfterWeight > 200.0) {
      personCount += 1;
      digitalWrite(CAMARA, HIGH);
      delay(500);
      digitalWrite(CAMARA, LOW);
    }
    currentWeight = millweight;
  }

  Blynk.virtualWrite(V2, personCount);
  Blynk.virtualWrite(V1, weight);
  delay(1500);
}
