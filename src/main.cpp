#include <Arduino.h>    
#include "DHT.h"        

#define DHTPIN 4     

// Loại cảm biến sử dụng: DHT22 (AM2302)
#define DHTTYPE DHT22   

// Khởi tạo đối tượng DHT với chân và loại cảm biến đã khai báo
DHT dht(DHTPIN, DHTTYPE);

// ------------------ HÀM SETUP CẢM BIẾN SỬ DỤNG GIAO THỨC ONE WIRE ------------------
void setup() {
  // Khởi động Serial để in dữ liệu ra Serial Monitor
  // Baudrate 115200 thường dùng cho ESP32 / ESP8266
  Serial.begin(115200);

  // In thông báo khởi tạo cảm biến
  Serial.println(F("Đang khởi tạo cảm biến DHT22..."));
  
  // Khởi động cảm biến DHT
  dht.begin();
}

// ------------------ HÀM LOOP ------------------
void loop() {
  // DHT22 cần tối thiểu ~2 giây giữa các lần đọc
  delay(2000);

  // Đọc độ ẩm (%)
  float h = dht.readHumidity();

  // Đọc nhiệt độ (đơn vị °C)
  float t = dht.readTemperature();

  // Kiểm tra nếu dữ liệu không hợp lệ (NaN = Not a Number)
  // Thường xảy ra khi cảm biến lỗi hoặc dây nối không đúng
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Lỗi: Không thể đọc dữ liệu từ cảm biến DHT!"));
    return;  // Thoát vòng lặp hiện tại
  }

  // In độ ẩm ra Serial Monitor
  Serial.print(F("Độ ẩm: "));
  Serial.print(h);
  Serial.print(F("%  |  "));

  // In nhiệt độ ra Serial Monitor
  Serial.print(F("Nhiệt độ: "));
  Serial.print(t);
  Serial.println(F("°C"));
}
