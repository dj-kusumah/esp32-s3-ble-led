#include "BleController.h"

// ── Instance global untuk callback access ────────────
static BleController* _instance = nullptr;

// ── Callback koneksi/diskoneksi ───────────────────────
class BleServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* server) override {
        if (_instance) _instance->_connected = true;
        Serial.println("[BLE] Client terhubung");
    }
    void onDisconnect(BLEServer* server) override {
        if (_instance) _instance->_connected = false;
        Serial.println("[BLE] Client terputus — advertising ulang...");
        BLEDevice::startAdvertising();
    }
};

// ── Callback saat HP menulis ke CMD characteristic ────
class BleCmdCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override {
        if (!_instance || !_instance->_callback) return;

        String rxValue = pChar->getValue().c_str();
        if (rxValue.length() == 0) return;

        uint8_t cmd = (uint8_t)rxValue[0];

        // Terima '1'/'2' (karakter ASCII) atau 0x01/0x02 (byte mentah)
        if (cmd == '1') cmd = BLE_CMD_SINGLE_TAP;
        if (cmd == '2') cmd = BLE_CMD_DOUBLE_TAP;

        if (cmd == BLE_CMD_SINGLE_TAP || cmd == BLE_CMD_DOUBLE_TAP) {
            _instance->_callback(cmd);
        }
    }
};

// ── Implementasi BleController ────────────────────────
BleController::BleController(const char* deviceName)
    : _deviceName(deviceName), _callback(nullptr),
      _statusChar(nullptr), _connected(false) {
    _instance = this;
}

void BleController::begin() {
    BLEDevice::init(_deviceName);

    BLEServer* server = BLEDevice::createServer();
    server->setCallbacks(new BleServerCallbacks());

    BLEService* service = server->createService(BLE_SERVICE_UUID);

    // CMD characteristic — HP menulis ke sini
    BLECharacteristic* cmdChar = service->createCharacteristic(
        BLE_CMD_CHAR_UUID,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    cmdChar->setCallbacks(new BleCmdCallbacks());

    // Status characteristic — ESP32 notify ke HP
    _statusChar = service->createCharacteristic(
        BLE_STATUS_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    _statusChar->addDescriptor(new BLE2902());
    _statusChar->setValue("Menginisialisasi...");

    service->start();

    BLEAdvertising* adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(BLE_SERVICE_UUID);
    adv->setScanResponse(true);
    adv->setMinPreferred(0x06);
    BLEDevice::startAdvertising();

    Serial.println("[BLE] Server aktif, nama: " + String(_deviceName));
}

void BleController::setCommandCallback(CommandCallback cb) {
    _callback = cb;
}

void BleController::updateStatus(const String& status) {
    if (_statusChar) {
        _statusChar->setValue(status.c_str());
        if (_connected) _statusChar->notify();
    }
}

bool BleController::isConnected() {
    return _connected;
}
