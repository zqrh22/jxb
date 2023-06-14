#include <ESP8266WiFi.h>
#include <espnow.h>

#include <Servo.h>
Servo servo2, servo1;
typedef struct struct_message {
  float x;
  float y;
  float z;
} struct_message;

struct_message myData;
int value = 0;

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("x: ");
  Serial.println(myData.x);
  Serial.print("y: ");
  Serial.println(myData.y);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  servo1.attach(D3, 500, 5000);
  servo1.write(90);
  servo2.attach(D4, 500, 5000);
  servo2.write(90);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (myData.x < 10 && myData.x > -10 && myData.y < 4 && myData.y > -4) {
    value = map(myData.x, -10, 10, 0, 180);
    int temp = servo1.read();
    if (temp < value) {
      for (int i = 0; i < 10 && temp < value; ++i) {
        temp++;
        if (temp < 180)
          servo1.write(temp);
        delay(25);
      }
    }
    if (temp > value) {
      for (int i = 0; i < 10 && temp > value; ++i) {
        temp--;
        if (temp > 0)
          servo1.write(temp);
        delay(25);
      }
    }
    Serial.println("up");
    Serial.print(value);
  }

  if (myData.y < 10 && myData.y > -10 && myData.x < 4 && myData.x > -4) {

    value = map(myData.y, -10, 10, 0, 180);
    int temp = servo2.read();
    if (temp < value) {
      for (int i = 0; i < 10 && temp < value; ++i) {
        temp++;
        if (temp < 180)
          servo2.write(temp);
        delay(25);
      }
    }
    if (temp > value) {
      for (int i = 0; i < 10 && temp > value; ++i) {
        temp--;
        if (temp > 0)
          servo2.write(temp);
        delay(25);
      }
    }
    Serial.println("Right");
    Serial.print(value);
  }
  delay(100);
  Serial.print("x: ");
  Serial.println(myData.x);
  Serial.print("y: ");
  Serial.println(myData.y);
}
