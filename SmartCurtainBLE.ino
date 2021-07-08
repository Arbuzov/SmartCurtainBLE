/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h> 

#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  2        //Time ESP32 will go to sleep (in seconds)

RTC_DATA_ATTR int direction = 0;
RTC_DATA_ATTR int position = 0;

int connections_count = 0;
int try_count = 0;

#include "CustomStepper.h"

#include "BatteryService.h"
#include "CurtainService.h"

void setup() {
  Serial.begin(115200);

  Serial.println("Boot device");

  BLEDevice::init("SmartCurtain");

  InitCurtainBLE();
  
  InitBateryBLE();
  UpdateBateryBLE();
}

void loop() {
  UpdateBateryBLE();
  /*
  if (connections_count == 0) {
    try_count++;
    if (try_count > 20) {
      Serial.println("Sleep by timeout");
      esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
      esp_deep_sleep_start();
    }
  }
  */
  delay(100);
}
