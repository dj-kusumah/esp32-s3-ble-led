#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUID Service & Characteristic
#define BLE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CMD_CHAR_UUID       "beb5483e-36e1-4688-b7f5-ea07361b26a8"  // WRITE
#define BLE_STATUS_CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a9"  // NOTIFY

// Nilai perintah yang dikirim dari HP
#define BLE_CMD_SINGLE_TAP  0x01   // kirim byte 0x01 atau karakter '1'
#define BLE_CMD_DOUBLE_TAP  0x02   // kirim byte 0x02 atau karakter '2'

/**
 * BleController — BLE GATT Server untuk kontrol LED
 *
 * Cara pakai dari HP (nRF Connect / LightBlue):
 *  1. Scan & connect ke "ESP32-LED-BLE"
 *  2. Tulis ke CMD characteristic:
 *     - 0x01 = single tap
 *     - 0x02 = double tap
 *  3. Status characteristic otomatis notify perubahan
 */
class BleController {
public:
    typedef void (*CommandCallback)(uint8_t cmd);

    BleController(const char* deviceName = "ESP32-LED-BLE");

    void begin();
    void setCommandCallback(CommandCallback cb);
    void updateStatus(const String& status);
    bool isConnected();

private:
    const char* _deviceName;
    CommandCallback _callback;
    BLECharacteristic* _statusChar;
    bool _connected;

    friend class BleServerCallbacks;
    friend class BleCmdCallbacks;
};

#endif
