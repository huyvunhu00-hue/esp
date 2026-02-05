#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// --- ĐỊNH NGHĨA CHÂN ---
#define RST_PIN    14
#define SS_PIN     10
#define DHTPIN     4
#define DHTTYPE    DHT22
#define SDA_I2C    8
#define SCL_I2C    9

// Khởi tạo đối tượng
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

// ================================================================
// PHẦN 1: GIAO THỨC I2C (MÀN HÌNH)
// ================================================================
void setup_I2C() {
    Wire.begin(SDA_I2C, SCL_I2C);
    lcd.init();
    lcd.backlight();
    lcd.print("He thong San sang");
    delay(2000);
    lcd.clear();
}

void hien_thi_I2C(float t, float h) {
    lcd.setCursor(0, 0);
    lcd.print("T: "); lcd.print(t, 1); lcd.print("C ");
    lcd.print("H: "); lcd.print(h, 1); lcd.print("%");
    
    lcd.setCursor(0, 1);
    lcd.print("Moi quet the...");
}

// ================================================================
// PHẦN 2: GIAO THỨC SPI (RFID)
// ================================================================
void setup_SPI() {
    SPI.begin(12, 13, 11, 10); // SCK, MISO, MOSI, SS
    mfrc522.PCD_Init();
}

void xu_ly_RFID_SPI() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ID The:");
        
        String uid = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            uid += String(mfrc522.uid.uidByte[i], HEX);
        }
        uid.toUpperCase();
        
        lcd.setCursor(0, 1);
        lcd.print(uid);
        Serial.println("Quet the: " + uid);
        
        delay(2000); // Tạm dừng để người dùng kịp nhìn ID
        lcd.clear();
        mfrc522.PICC_HaltA(); 
    }
}

// ================================================================
// PHẦN 3: GIAO THỨC ONE-WIRE (DHT22)
// ================================================================
void setup_OneWire() {
    dht.begin();
}

void doc_cam_bien_OneWire() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
        hien_thi_I2C(t, h); // Gọi hàm hiển thị từ module I2C
    }
}

// ================================================================
// CHƯƠNG TRÌNH CHÍNH
// ================================================================
void setup() {
    Serial.begin(115200);
    
    setup_I2C();      // Khởi tạo I2C
    setup_SPI();      // Khởi tạo SPI
    setup_OneWire();  // Khởi tạo DHT
}

void loop() {
    xu_ly_RFID_SPI();      // Ưu tiên kiểm tra thẻ RFID
    doc_cam_bien_OneWire(); // Cập nhật nhiệt độ độ ẩm
    
    delay(200); // Tốc độ phản hồi hệ thống
}
