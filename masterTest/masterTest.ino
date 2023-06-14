#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

Adafruit_MPU6050 mpu;

uint8_t broadcastAddress[] = { 0x2C, 0xF4, 0x32, 0x30, 0xBF, 0xFA };

typedef struct {
  float x;
  float y;
  float z;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_register_send_cb(OnDataSent);

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
      Serial.println("Failed to find MPU6050 chip");
    }
  } else
    Serial.println("success to find MPU6050 chip");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  myData.x = a.acceleration.x;
  myData.y = a.acceleration.y;
  myData.z = a.acceleration.z;

  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  delay(1000);
}