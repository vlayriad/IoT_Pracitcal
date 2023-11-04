#include <Arduino.h>
#include "DHT.h" // Sertakan pustaka DHT

#include "esp_log.h" // Sertakan pustaka ESP_LOG

static const char *TAG = "DHT"; // Tag log khusus

/* Konfigurasi PIN dan TIPE DHT11 */
#define DHTPIN 4 // Sesuaikan pin input dengan yang Anda gunakan
#define DHTTYPE DHT11 // Sesuaikan dengan jenis sensor yang digunakan

DHT dht(DHTPIN, DHTTYPE); // Membuat objek DHT

void setup() {
  Serial.begin(9600);
  ESP_LOGI(TAG, "Deteksi Suhu dan Kelembaban"); // Mencetak info log menggunakan ESP_LOG
  dht.begin();
}

void loop() {
  delay(2000); // Jeda pembacaan sensor

  float t = dht.readTemperature(); // Membaca suhu dari sensor
  float h = dht.readHumidity(); // Membaca kelembaban dari sensor

  if (isnan(h) || isnan(t)) {
    ESP_LOGW(TAG, "Gagal membaca data dari sensor DHT."); // Log peringatan jika gagal membaca data
    return;
  }

  ESP_LOGI(TAG, "Suhu: %.2f °C, Kelembaban: %.2f %%", t, h); // Mencetak suhu dan kelembaban dengan dua desimal
}
