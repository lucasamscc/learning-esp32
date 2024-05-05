#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <esp_bt.h>

int scanTime = 5; // In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // Obtém o RSSI e calcula a distância aproximada usando uma fórmula empírica
        int rssi = advertisedDevice.getRSSI();
        float distance = pow(10, ((-69 - rssi) / (10 * 2.0)));

        Serial.print("Dispositivo anunciado: ");
        Serial.print(advertisedDevice.toString().c_str());
        Serial.print(" | RSSI: ");
        Serial.print(rssi);
        Serial.print(" dBm | Distância aproximada: ");
        Serial.print(distance, 2);
        Serial.println(" metros");
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Escaneando...");

    // Inicializa o BLE
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
}

void loop() {
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

    Serial.print("Dispositivos encontrados: ");
    Serial.println(foundDevices.getCount());
    Serial.println("Escaneamento concluído!");

    pBLEScan->clearResults();
    delay(2000);
}
