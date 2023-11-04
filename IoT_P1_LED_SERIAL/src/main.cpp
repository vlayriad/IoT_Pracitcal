#include <Arduino.h>

/* Berapa banyak LED yang akan dikendalikan */
#define LED_COUNT 3

/* Daftar pin LED yang akan dikendalikan */
int ledPins[LED_COUNT] = {4, 16, 15}; // Ganti dengan pin yang sesuai

void setup()
{
  /* Inisialisasi pin LED */
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Matikan semua LED saat inisialisasi
  }

  Serial.begin(9600); // Mulai komunikasi serial
}

void loop()
{
  if (Serial.available() > 0)
  {
    /* Baca input dari Serial Monitor */
    String input = Serial.readStringUntil('\n');

    /* Konversi input menjadi bilangan bulat */
    int numericValue = input.toInt();

    if (numericValue != 0) {
      /* Matikan semua LED terlebih dahulu */
      for (int i = 0; i < LED_COUNT; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      if (numericValue <= LED_COUNT) {
        /* Nyalakan LED dengan nomor sesuai input */
        digitalWrite(ledPins[numericValue - 1], HIGH);
        Serial.print("LED ");
        Serial.print(numericValue);
        Serial.println(" ON");
        delay(2000);
        digitalWrite(ledPins[numericValue - 1], LOW);
        Serial.print("LED ");
        Serial.print(numericValue);
        Serial.println(" OFF");
      } else {
        Serial.println("Input tidak valid. Masukkan angka antara 1 hingga 3.");
      }
    } else {
      Serial.println("Input tidak valid. Masukkan angka.");
    }
  }
}
