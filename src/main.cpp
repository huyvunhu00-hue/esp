#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <BH1750.h>
#include <TFT_eSPI.h>

// ========================================================
// PHẦN 1: CẤU HÌNH VÀ XỬ LÝ DHT22 (ONE WIRE)
// ========================================================
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float tempValue = 0;
float humiValue = 0;

void readDHT() {
    tempValue = dht.readTemperature();
    humiValue = dht.readHumidity();
    if (isnan(tempValue) || isnan(humiValue)) {
        Serial.println("Lỗi: Không đọc được dữ liệu từ DHT22!");
    }
}

// ========================================================
// PHẦN 2: CẤU HÌNH VÀ XỬ LÝ BH1750 (I2C)
// ========================================================
BH1750 lightMeter;
float luxValue = 0;

void readLight() {
    luxValue = lightMeter.readLightLevel();
    if (luxValue < 0) {
        Serial.println("Lỗi: Không đọc được dữ liệu từ BH1750!");
    }
}

// ========================================================
// PHẦN 3: CẤU HÌNH VÀ HIỂN THỊ TFT LCD (SPI)
// ========================================================
TFT_eSPI tft = TFT_eSPI();

void initLCD() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    // Vẽ tiêu đề cố định
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("DASHBOARD GIAM SAT", 40, 10);
    tft.drawFastHLine(0, 35, 320, TFT_GREY);
}

void updateLCD() {
    // Khu vực hiển thị Nhiệt độ
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(10, 60);
    tft.printf("Nhiet do: %.1f C", tempValue);

    // Khu vực hiển thị Độ ẩm
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(10, 100);
    tft.printf("Do am   : %.1f %%", humiValue);

    // Khu vực hiển thị Ánh sáng
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setCursor(10, 140);
    tft.printf("Anh sang: %.1f lx", luxValue);
}

// ========================================================
// KHỞI TẠO VÀ VÒNG LẶP CHÍNH
// ========================================================

void setup() {
    Serial.begin(115200);
    
    // Khởi tạo các phần cứng
    dht.begin();                      // Khởi tạo DHT
    Wire.begin(21, 22);               // Khởi tạo I2C (SDA=21, SCL=22)
    lightMeter.begin();               // Khởi tạo BH1750
    initLCD();                        // Khởi tạo Màn hình
    
    Serial.println("He thong da san sang!");
}

void loop() {
    // 1. Thu thập dữ liệu
    readDHT();
    readLight();

    // 2. Xuất dữ liệu ra Serial (để debug)
    Serial.printf("T: %.1f C | H: %.1f %% | L: %.1f lx\n", tempValue, humiValue, luxValue);

    // 3. Hiển thị lên LCD
    updateLCD();

    delay(2000); // Cập nhật sau mỗi 2 giây
}
