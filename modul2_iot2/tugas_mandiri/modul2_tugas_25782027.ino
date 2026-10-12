#include "DHT.h"

// Deklarasi Pin ESP32
#define LDR_PIN   A0   // Pakai PIN VP
#define DHT_PIN   13   // Pengganti D7 (GPIO 13)
#define RELAY_PIN 12   // Pengganti D6 (GPIO 12)
#define LED_PIN   5    // Pengganti D1 (GPIO 5)

#define DHTTYPE DHT22

// Sifat Relay (Active HIGH)
#define RELAY_ON  HIGH
#define RELAY_OFF LOW

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Konfigurasi Aktuator sebagai OUTPUT
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Matikan aktuator di awal demi keamanan
  digitalWrite(RELAY_PIN, RELAY_OFF);
  digitalWrite(LED_PIN, LOW);

  dht.begin();
  Serial.println("Sistem Monitoring Siap...");
}

void loop() {
  // Pembacaan Sensor
  int nilaiLDR = analogRead(LDR_PIN);
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  // Validasi pembacaan DHT (Mencegah kegagalan pembacaan / NaN)
  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca sensor DHT22!");
    delay(2000);
    return;
  }

  // Menampilkan data pembacaan di Serial Monitor
  Serial.print("LDR (ADC): ");
  Serial.print(nilaiLDR);
  Serial.print(" | Suhu: ");
  Serial.print(suhu);
  Serial.println(" C");

  // Rule Engine: Logika Kondisi OR (||)
  // Aktif jika Suhu > 34°C ATAU LDR < 300 (ESP32 ADC skala 0-4095)
  if (suhu > 34.0 || nilaiLDR < 300) {
    digitalWrite(RELAY_PIN, RELAY_ON);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Status: PERINGATAN! Kondisi Buruk -> Aktuator Aktif!");
  } else {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    digitalWrite(LED_PIN, LOW);
    Serial.println("Status: Kondisi Aman");
  }

  Serial.println("----------------------------------------");
  delay(2000); // Delay aman untuk siklus baca DHT22
}