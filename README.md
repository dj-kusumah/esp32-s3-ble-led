# Latihan6

Project PlatformIO untuk ESP32-S3 DevKitC-1 yang mengontrol LED luar dan RGB LED onboard lewat Bluetooth Low Energy, lalu menampilkan status perangkat di OLED SSD1306.

## Fitur

1. BLE GATT server dengan nama device `ESP32-LED-BLE`.
2. Single tap command untuk toggle LED luar atau ganti warna RGB.
3. Double tap command untuk pindah mode LED luar dan RGB.
4. OLED status display untuk mode aktif, status LED, dan koneksi BLE.
5. Notify status balik ke HP lewat BLE characteristic.

## Hardware yang dipakai

1. ESP32-S3 DevKitC-1
2. OLED SSD1306 I2C 128x64
3. 1 LED eksternal
4. Resistor 220 ohm atau 330 ohm
5. Kabel jumper
6. HP dengan aplikasi BLE seperti `nRF Connect` atau `LightBlue`

## Pin dan wiring

Pin yang dipakai:

1. `GPIO 48` untuk RGB LED onboard
2. `GPIO 2` untuk LED eksternal
3. `GPIO 8` untuk `SDA` OLED
4. `GPIO 9` untuk `SCL` OLED

Koneksi OLED:

1. `VCC` -> `3.3V`
2. `GND` -> `GND`
3. `SDA` -> `GPIO 8`
4. `SCL` -> `GPIO 9`

Koneksi LED eksternal:

1. `GPIO 2` -> resistor -> anoda LED
2. `GND` -> katoda LED

RGB LED onboard tidak perlu wiring tambahan karena sudah terpasang di board.

## Dependency utama

Dependency eksternal yang dipakai di `platformio.ini`:

1. `adafruit/Adafruit NeoPixel`
2. `adafruit/Adafruit SSD1306`
3. `adafruit/Adafruit GFX Library`

Catatan: library BLE memakai API bawaan ESP32 Arduino core seperti `BLEDevice.h`, jadi tidak perlu menambah dependency BLE terpisah di `lib_deps`.

## Struktur inti project

```text
Latihan6/
|- platformio.ini
|- lib/
|  |- BleController/
|  |- Led/
|  |- Logger/
|  |- OledDisplay/
|  |- RgbLed/
|- src/
|  |- main.cpp
```

## Cara build dan upload

Jalankan dari folder project ini:

```powershell
pio run
pio run -t upload
pio device monitor
```

## Cara pakai BLE dari HP

1. Nyalakan board dan tunggu BLE advertising aktif.
2. Buka `nRF Connect` atau `LightBlue`.
3. Connect ke device `ESP32-LED-BLE`.
4. Tulis ke command characteristic:
5. `0x01` atau karakter `1` untuk single tap.
6. `0x02` atau karakter `2` untuk double tap.

UUID yang dipakai:

1. Service UUID: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`
2. CMD characteristic: `beb5483e-36e1-4688-b7f5-ea07361b26a8`
3. Status characteristic: `beb5483e-36e1-4688-b7f5-ea07361b26a9`

## Perilaku program saat ini

Saat board dinyalakan:

1. OLED diinisialisasi lebih dulu.
2. BLE server aktif dengan nama `ESP32-LED-BLE`.
3. OLED menampilkan status mode, LED luar, LED RGB, dan status koneksi BLE.
4. Single tap akan toggle LED luar jika sedang di mode LED luar.
5. Single tap akan ganti warna RGB jika sedang di mode RGB.
6. Double tap akan pindah mode dari LED luar ke RGB atau sebaliknya.

## Troubleshooting singkat

1. Kalau OLED tidak tampil, cek wiring `SDA`, `SCL`, `VCC`, dan `GND`.
2. Kalau LED eksternal tidak nyala, cek polaritas LED dan resistor.
3. Kalau HP tidak menemukan device, pastikan BLE advertising aktif dan board menyala normal.
4. Kalau perintah BLE tidak bekerja, cek data yang dikirim ke command characteristic.
5. Kalau upload gagal, pastikan port serial tidak sedang dipakai aplikasi lain.