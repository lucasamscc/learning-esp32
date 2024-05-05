
// ---- wifi -----
#include <WiFi.h>

const char* ssid     = "Claro_ Net 2.4Ghz"; // Change this to your WiFi SSID
const char* password = "3656306250"; // Change this to your WiFi password
WiFiClient esp32Client;

// -- MQTT --
#include <PubSubClient.h>
PubSubClient client(esp32Client);
const char* mqtt_Broker = "192.168.1.4";
const char* topico= "laboratorio/office/iluminacao";
const char* mqtt_ClientID = "esp32-01";
const char* mqtt_user = "curso";
const char* mqtt_pass = "laboratorio01";

// --- bluetooth ----
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5;
String dispositivosAutorizados = "dd:f8:44:df:fa:ad";
int nivelRSSI = -50;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String dispositivosEncontrados = advertisedDevice.getAddress().toString().c_str();
    if (dispositivosEncontrados == dispositivosAutorizados && advertisedDevice.getRSSI() > nivelRSSI) {
      Serial.println("Identificador DETECTADO!");
      Serial.println("RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
    }
  }
};


void setup() {
  Serial.begin(115200);
  conectaWifi();
  Serial.println("Scanning...");

  BLEDevice::init("");
}

void scanBLE() {
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  Serial.print("Devices Found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan Done");
}

void conectaWifi(){
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// -- conecta ao mqtt ---
void conectaMQTT() {
  while(!client.connected()){
    client.connect(mqtt_ClientID, mqtt_user, mqtt_pass);
  }
}


void loop () {
  if (!client.connected()) {
    conectaMQTT();
  }

  scanBLE();
  delay(2000);
}
