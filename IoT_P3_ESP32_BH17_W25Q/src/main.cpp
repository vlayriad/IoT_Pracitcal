#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <SPI.h>
#include <w25q64.hpp>

#define LED1_PIN 2
#define LED2_PIN 4
#define LED3_PIN 16
#define LED4_PIN 17
#define BUTTON_PIN 15

BH1750 lightMeter(0x23);
bool sensorStatus = false;
bool autoBrightness = false;
portMUX_TYPE gpioIntMux = portMUX_INITIALIZER_UNLOCKED;

w25q64 spiChip;

void buttonInterrupt() {
  portENTER_CRITICAL(&gpioIntMux);
  sensorStatus = !sensorStatus;
  portEXIT_CRITICAL(&gpioIntMux);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  spiChip.begin();

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  lightMeter.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, FALLING);

  byte chipId[4];
  spiChip.getId(chipId);

  char readBuffer[256];
  spiChip.readPages(reinterpret_cast<unsigned char*>(readBuffer), 0xFFFF, 1);
  autoBrightness = strcmp(readBuffer, "true") == 0;
}

void loop() {
  if (sensorStatus) {
    portENTER_CRITICAL(&gpioIntMux);
    sensorStatus = false;
    portEXIT_CRITICAL(&gpioIntMux);

    autoBrightness = !autoBrightness;
    ESP_LOGI("Auto Brightness", "status: %s", autoBrightness ? "true" : "false");

    char writeBuffer[6];
    strcpy(writeBuffer, autoBrightness ? "true" : "false");
    spiChip.erasePageSector(0xFFFF);
    spiChip.pageWrite(reinterpret_cast<unsigned char*>(writeBuffer), 0xFFFF);
    ESP_LOGI("W25Q64", "Done writing");
    delay(1000);
  }

  float lux = lightMeter.readLightLevel();

  if (autoBrightness) {
    ESP_LOGI("BH1750", "Intensitas Cahaya: %.2f lux", lux);
    if (lux <= 250) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, HIGH);
      digitalWrite(LED4_PIN, HIGH);
    } else if (lux <= 500) {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, HIGH);
      digitalWrite(LED4_PIN, HIGH);
    } else if (lux <= 750) {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, HIGH);
      digitalWrite(LED4_PIN, HIGH);
    } else if (lux <= 1000) {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, LOW);
      digitalWrite(LED4_PIN, HIGH);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, LOW);
      digitalWrite(LED4_PIN, LOW);
    }
  } else {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);
  }

  delay(1000);
}
