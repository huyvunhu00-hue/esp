#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 4     
#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("Đang khởi tạo cảm biến DHT22..."));
  
  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Lỗi: Không thể đọc dữ liệu từ cảm biến DHT!"));
    return;
  }

  Serial.print(F("Độ ẩm: "));
  Serial.print(h);
  Serial.print(F("%  |  Nhiệt độ: "));
  Serial.print(t);
  Serial.println(F("°C"));
}