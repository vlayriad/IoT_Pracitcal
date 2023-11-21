#include <Arduino.h>

/* Daftar pin LED yang akan dikendalikan */
int ledPins[] = {15, 4, 5}; // Ganti dengan pin yang sesuai

void setup()
{
  /* Inisialisasi pin LED */
  int lengthArr = sizeof(ledPins) / sizeof(ledPins[0]);
  for (int i = 0; i < lengthArr; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Matikan semua LED saat inisialisasi
  }

  Serial.begin(9600); // Mulai komunikasi serial
}

void loop()
{
  int lengthArr = sizeof(ledPins) / sizeof(ledPins[0]); // Pindahkan deklarasi ke dalam loop()

  if (Serial.available() > 0)
  {
    /* Baca input dari Serial Monitor */
    String input = Serial.readStringUntil('\n');

    /* Konversi input menjadi bilangan bulat */
    int numericValue = input.toInt();

    if (numericValue != 0) {
      /* Matikan semua LED terlebih dahulu */
      for (int i = 0; i < lengthArr; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      if (numericValue <= lengthArr && numericValue >= 1) {
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
