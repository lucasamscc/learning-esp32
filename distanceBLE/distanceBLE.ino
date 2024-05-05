#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <math.h> // Inclua a biblioteca math.h para a função log

int scanTime = 5; // Em segundos
float A = -50.0; // Valor de RSSI a 1 metro
float n = 4.0; // Constante de perda de trajeto
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.printf("Advertised Device: %s, RSSI: %d, Distance: %.2f meters\n", advertisedDevice.toString().c_str(), advertisedDevice.getRSSI(), calculateDistance(advertisedDevice.getRSSI()));
    }

    float calculateDistance(int RSSI) {
        float distance = pow(10, ((A - RSSI) / (10 * n))); // Fórmula de estimativa de distância
        // Limitar a distância a um mínimo de 0 metros
        if (distance < 0) {
            distance = 0.0;
        }
        return distance;
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Scanning...");

    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); // Criar nova varredura
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // A varredura ativa consome mais energia, mas obtém resultados mais rapidamente
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // menos ou igual ao valor setInterval
}

void loop() {
    // Coloque seu código principal aqui, para ser executado repetidamente:
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("Scan done!");
    pBLEScan->clearResults();   // deletar resultados do buffer de BLEScan para liberar memória
    delay(2000);
}
