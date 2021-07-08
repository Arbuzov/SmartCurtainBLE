#pragma once

#define BatteryService BLEUUID((uint16_t)0x180F)

int sensorValue = 0;
int sensorPin = 34;
float vMax = 3.5;
int valMax = 4095;
float kDiv = 2.1;
float vMaxBat = 4.30;
int percentage = 0;


BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901));

class BatteryServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      connections_count++;
      Serial.println("Connected");
    };
    void onDisconnect(BLEServer* pServer) {
      connections_count--;
      if (connections_count < 0) {
        connections_count = 0;
      }
      Serial.println("Disconnected");
    }
};

void InitBateryBLE() {
  BLEDevice::init("BLE Battery");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BatteryServerCallbacks());

  // Create the BLE Service
  BLEService *pBattery = pServer->createService(BatteryService);

  pBattery->addCharacteristic(&BatteryLevelCharacteristic);
  BatteryLevelDescriptor.setValue("Percentage 0 - 100");
  BatteryLevelCharacteristic.addDescriptor(&BatteryLevelDescriptor);
  BatteryLevelCharacteristic.addDescriptor(new BLE2902());

  pServer->getAdvertising()->addServiceUUID(BatteryService);

  pBattery->start();
  pServer->getAdvertising()->start();
}

float valueToVolt(int value){
  return kDiv*value*vMax/valMax;
}

int calcPercenage(float vreal, float vmax) {
  return round(100*vreal/vmax);
}

void UpdateBateryBLE() {
  
  sensorValue = analogRead(sensorPin);
  percentage = calcPercenage(valueToVolt(sensorValue), vMaxBat);
  //Serial.println("sensor "+ String(sensorValue) +" volts " + String(valueToVolt(sensorValue)));
  //Serial.println(percentage);
  BatteryLevelCharacteristic.setValue(percentage);
  BatteryLevelCharacteristic.notify();
}
