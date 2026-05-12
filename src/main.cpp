#include <Arduino.h>
#include <RgbLed.h>
#include <Led.h>
#include <OledDisplay.h>
#include <BleController.h>
#include <Logger.h>

// ── Pin ────────────────────────────────────────────────
#define RGB_LED_PIN  48
#define EXT_LED_PIN   2
#define I2C_SDA       8
#define I2C_SCL       9

// ── Objek ──────────────────────────────────────────────
RgbLed      rgbLed(RGB_LED_PIN);
Led         extLed(EXT_LED_PIN);
OledDisplay oled(128, 64, 0x3C);
BleController ble("ESP32-LED-BLE");

// ── Mode operasi ───────────────────────────────────────
enum Mode { MODE_LUAR, MODE_RGB };
Mode currentMode = MODE_LUAR;

// ── Warna RGB ──────────────────────────────────────────
const char* colorNames[] = {"MERAH", "KUNING", "HIJAU", "CYAN", "BIRU", "MAGENTA", "PUTIH"};
const int   NUM_COLORS   = 7;
int colorIndex = 0;

// ── Flag dari BLE callback (ISR-safe) ─────────────────
volatile uint8_t pendingCmd = 0;

// ══════════════════════════════════════════════════════
// Update OLED sesuai state saat ini
// ══════════════════════════════════════════════════════
void updateDisplay() {
    const char* modeStr   = (currentMode == MODE_LUAR) ? "LED Luar" : "LED RGB";
    const char* luarStr   = extLed.isOn() ? "NYALA" : "MATI";
    const char* rgbStr;
    if (currentMode == MODE_RGB) {
        rgbStr = colorNames[colorIndex];
    } else {
        rgbStr = rgbLed.isOn() ? colorNames[colorIndex] : "MATI";
    }
    const char* bleStr = ble.isConnected() ? "Terhubung" : "Menunggu...";

    oled.showStatus(modeStr, luarStr, rgbStr, bleStr);

    // Kirim status ke HP via BLE notify
    String statusMsg = String("Mode:") + modeStr +
                       "|Ext:" + luarStr +
                       "|RGB:" + rgbStr;
    ble.updateStatus(statusMsg);

    Logger::info("Mode:" + String(modeStr) +
                 " | Luar:" + luarStr +
                 " | RGB:" + rgbStr);
}

// ══════════════════════════════════════════════════════
// Logika single tap
// ══════════════════════════════════════════════════════
void handleSingleTap() {
    if (currentMode == MODE_LUAR) {
        extLed.toggle();
        Logger::info("Single tap - LED Luar: " + String(extLed.isOn() ? "NYALA" : "MATI"));
    } else {
        colorIndex = (colorIndex + 1) % NUM_COLORS;
        switch (colorIndex) {
            case 0: rgbLed.red();     break;
            case 1: rgbLed.yellow();  break;
            case 2: rgbLed.green();   break;
            case 3: rgbLed.cyan();    break;
            case 4: rgbLed.blue();    break;
            case 5: rgbLed.magenta(); break;
            case 6: rgbLed.white();   break;
        }
        Logger::info("Single tap - RGB: " + String(colorNames[colorIndex]));
    }
    updateDisplay();
}

// ══════════════════════════════════════════════════════
// Logika double tap
// ══════════════════════════════════════════════════════
void handleDoubleTap() {
    if (currentMode == MODE_LUAR) {
        // Pindah ke MODE_RGB: matikan LED luar, nyalakan RGB warna pertama
        extLed.off();
        currentMode = MODE_RGB;
        colorIndex  = 0;
        rgbLed.red();
        Logger::info("Double tap - Masuk Mode RGB");
    } else {
        // Kembali ke MODE_LUAR: matikan RGB, reset
        rgbLed.off();
        extLed.off();
        currentMode = MODE_LUAR;
        Logger::info("Double tap - Kembali ke Mode LED Luar");
    }
    updateDisplay();
}

// ══════════════════════════════════════════════════════
// Callback BLE (dipanggil dari BLE task)
// ══════════════════════════════════════════════════════
void onBleCommand(uint8_t cmd) {
    pendingCmd = cmd;  // diproses di loop() agar aman
}

// ══════════════════════════════════════════════════════
void setup() {
    Logger::begin(115200);

    // Init hardware
    rgbLed.begin();
    extLed.begin();  // state awal: mati

    // Init OLED
    oled.begin(I2C_SDA, I2C_SCL);
    oled.showMessage("Memulai BLE...", "ESP32-LED-BLE");
    delay(800);

    // Init BLE
    ble.setCommandCallback(onBleCommand);
    ble.begin();

    // Tampilan awal
    updateDisplay();

    Logger::info("=== BLE LED Control ===");
    Logger::info("Koneksi HP ke: ESP32-LED-BLE");
    Logger::info("Kirim 0x01 = single tap | 0x02 = double tap");
}

void loop() {
    // Proses perintah BLE di main loop (thread-safe)
    if (pendingCmd != 0) {
        uint8_t cmd = pendingCmd;
        pendingCmd = 0;

        if (cmd == BLE_CMD_SINGLE_TAP) {
            handleSingleTap();
        } else if (cmd == BLE_CMD_DOUBLE_TAP) {
            handleDoubleTap();
        }
    }

    // Update status BLE di OLED saat koneksi berubah
    static bool lastConnected = false;
    bool nowConnected = ble.isConnected();
    if (nowConnected != lastConnected) {
        lastConnected = nowConnected;
        updateDisplay();
    }

    delay(10);
}