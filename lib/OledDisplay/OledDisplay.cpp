#include "OledDisplay.h"

OledDisplay::OledDisplay(uint8_t width, uint8_t height, uint8_t address)
    : _display(width, height, &Wire, -1), _address(address), _width(width), _height(height) {}

bool OledDisplay::begin(int sdaPin, int sclPin) {
    Wire.begin(sdaPin, sclPin);
    if (!_display.begin(SSD1306_SWITCHCAPVCC, _address)) {
        Serial.println("[ERROR] OLED tidak ditemukan!");
        return false;
    }
    _display.clearDisplay();
    _display.display();
    return true;
}

void OledDisplay::showStatus(const char* mode, const char* ledLuar,
                             const char* ledRgb, const char* bleStatus) {
    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);

    // Header
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println("=== BLE LED Control ===");
    _display.drawLine(0, 10, _width, 10, SSD1306_WHITE);

    // Mode aktif
    _display.setCursor(0, 13);
    _display.print("Mode   : ");
    _display.println(mode);

    // Status LED luar
    _display.setCursor(0, 25);
    _display.print("LED ext: ");
    _display.println(ledLuar);

    // Status LED RGB
    _display.setCursor(0, 37);
    _display.print("LED RGB: ");
    _display.println(ledRgb);

    // Status BLE
    _display.drawLine(0, 49, _width, 49, SSD1306_WHITE);
    _display.setCursor(0, 52);
    _display.print("BLE: ");
    _display.println(bleStatus);

    _display.display();
}

void OledDisplay::showMessage(const char* line1, const char* line2) {
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0, 20);
    _display.println(line1);
    if (line2) { _display.setCursor(0, 36); _display.println(line2); }
    _display.display();
}

void OledDisplay::clear() {
    _display.clearDisplay();
    _display.display();
}

Adafruit_SSD1306& OledDisplay::getDisplay() { return _display; }
