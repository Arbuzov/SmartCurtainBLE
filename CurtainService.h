#pragma once

#define SERVICE_UUID                 "7ad39eeb-a1bd-11eb-bcbc-0242ac130002"
#define POSITION_CHARACTERISTIC_UUID "POSITION-a1bd-11eb-bcbc-0242ac130002"
#define DIRECION_CHARACTERISTIC_UUID "DIRECION-a1bd-11eb-bcbc-0242ac130002"
#define DEBUG_CHARACTERISTIC_UUID    "DEBUG000-a1bd-11eb-bcbc-0242ac130002"

class CurtainServerCallbacks: public BLEServerCallbacks {
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

class DirectionCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.print("direction to: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);
        Serial.println();

        if (value == "on") {
          Serial.println("on command");
        } else if (value == "off") {
          Serial.println("off command");
        } else {
          Serial.println("unknow command");
        }
      }
    }
};

class PositionCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.print("position to: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);
        Serial.println();

        if (value == "on") {
          Serial.println("on command");
        } else if (value == "off") {
          Serial.println("off command");
        } else {
          Serial.println("unknow command");
        }
      }
    }
};

class DebugCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.print("debug to: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        if (value == "on") {
          Serial.println("on command");
        } else if (value == "off") {
          Serial.println("off command");
        } else {
          Serial.println("unknow command");
        }
      }
    }
};

void InitCurtainBLE() {
	  BLEServer *pServer = BLEDevice::createServer();
	  pServer->setCallbacks(new CurtainServerCallbacks());

	  BLEService *pService = pServer->createService(SERVICE_UUID);

	  BLECharacteristic *pCharacteristicDirection = pService->createCharacteristic(
	        DIRECION_CHARACTERISTIC_UUID,
	        BLECharacteristic::PROPERTY_READ |
	        BLECharacteristic::PROPERTY_WRITE
	      );

	  BLECharacteristic *pCharacteristicDebug = pService->createCharacteristic(
	        DEBUG_CHARACTERISTIC_UUID,
	        BLECharacteristic::PROPERTY_READ |
	        BLECharacteristic::PROPERTY_WRITE
	      );

	  BLECharacteristic *pCharacteristicPosition = pService->createCharacteristic(
	        POSITION_CHARACTERISTIC_UUID,
	        BLECharacteristic::PROPERTY_READ |
	        BLECharacteristic::PROPERTY_WRITE
	      );

	  pCharacteristicDirection->setCallbacks(new DirectionCallback());
	  pCharacteristicDebug->setCallbacks(new DebugCallback());
	  pCharacteristicPosition->setCallbacks(new PositionCallback());

	  pCharacteristicDirection->setValue(direction);
	  pCharacteristicPosition->setValue(position);
	  pService->start();

	  BLEAdvertising *pAdvertising = pServer->getAdvertising();
	  pAdvertising->start();
}
