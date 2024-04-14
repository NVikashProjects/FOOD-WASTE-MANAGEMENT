#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3uvN0YHod"
#define BLYNK_TEMPLATE_NAME "Efficient Food Waste Management System"
#define BLYNK_AUTH_TOKEN "5ExD254Iya4_2ayNrSC5JnrxpQOvpLaD"

#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "EFWM";
const char* password = "12345678";
char auth[] = BLYNK_AUTH_TOKEN;  

#define CAMERA_MODEL_AI_THINKER  
#include "camera_pins.h"
#define PIR 13
#define PHOTO 14

String local_IP;
void startCameraServer();

int change = 0;

void takePhoto() {
  delay(200);
  uint32_t randomNum = random(50000);
  Serial.println("http://" + local_IP + "/capture?_cb=" + (String)randomNum);
  Blynk.setProperty(V3, "urls", "http://" + local_IP + "/capture?_cb=" + (String)randomNum);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t* s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 3);        
    s->set_brightness(s, 1);   
    s->set_saturation(s, -2);  
  }
  s->set_framesize(s, FRAMESIZE_QVGA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  local_IP = WiFi.localIP().toString();
  Serial.println("' to connect");
  Blynk.begin(auth, ssid, password);
}

void loop() {
  Blynk.run();
  Blynk.syncVirtual(V1);
  delay(500);
}
  BLYNK_WRITE(V1) {
    int number = param.asInt();
    Serial.println(number);
    if (change < number) {
      Serial.println("Capture Photo");
      takePhoto();
      delay(1000);
      change = change + 1;
    }
  }
