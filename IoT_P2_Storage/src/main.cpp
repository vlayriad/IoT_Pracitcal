#include <Arduino.h>
#include <Preferences.h>

// Nama penyimpanan untuk Preferences API
#define STORAGE_NAME "storage"

// Kunci penyimpanan untuk status LED
#define LED_STATUS_KEY "led"

// Pin untuk tombol
#define BUTTON_PIN 21

// Variabel untuk menyimpan status LED
bool ledStatus = LOW;

// Flag untuk menandai perubahan status LED
bool changeLedStatus = false;

// Objek Preferences untuk menyimpan dan mengambil preferensi
Preferences storage;

// Interrupt Service Routine (ISR) untuk menangani interupsi dari tombol
void IRAM_ATTR gpioISR()
{
  // Menandai perubahan status saat interupsi terdeteksi
  changeLedStatus = true;
}

void setup()
{
  // Konfigurasi pin tombol sebagai input dengan pull-up internal
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Konfigurasi pin built-in LED sebagai output
  pinMode(BUILTIN_LED, OUTPUT);

  // Mendaftarkan fungsi ISR untuk menangani interupsi tombol saat FALLING edge terdeteksi
  attachInterrupt(BUTTON_PIN, &gpioISR, FALLING);

  // Inisialisasi komunikasi serial dengan baud rate 9600
  Serial.begin(9600);

  // Memulai Preferences dengan nama penyimpanan yang telah ditentukan
  storage.begin(STORAGE_NAME);

  // Mendapatkan status LED dari penyimpanan dan mengatur status LED sesuai
  ledStatus = storage.getBool(LED_STATUS_KEY);

  // Menutup Preferences setelah mendapatkan nilai yang dibutuhkan
  storage.end();

  // Mengatur status LED sesuai dengan nilai yang diperoleh
  digitalWrite(BUILTIN_LED, ledStatus);
}

void loop()
{
  // Memeriksa apakah terjadi perubahan status LED
  if (changeLedStatus)
  {
    // Menonaktifkan interupsi untuk menghindari konflik akses
    noInterrupts();
    changeLedStatus = false;

    // Mengaktifkan kembali interupsi setelah menangani perubahan status
    interrupts();
    // Mengubah status LED
    ledStatus = !ledStatus;
    // Mengatur status LED sesuai dengan perubahan
    digitalWrite(BUILTIN_LED, ledStatus);

    // Memulai Preferences untuk menyimpan status LED
    storage.begin(STORAGE_NAME);

    // Menyimpan status LED ke penyimpanan
    storage.putBool(LED_STATUS_KEY, ledStatus);

    // Menutup Preferences setelah menyimpan nilai yang diperlukan
    storage.end();
  }
}
