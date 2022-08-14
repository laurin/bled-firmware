#include <Arduino.h>
#include "BLEDevice.h"

#include "ledcontroller.hpp"
LEDController *leds = new LEDController();

BLECharacteristic *pCharacteristic;

bool deviceConnected = false;

char currentColor[3] = {0, 0, 0};

#define USER_DESCRIPTION_DESCRIPTOR_UUID BLEUUID((uint16_t)0x2901)
#define CLIENT_CHARACTERISTIC_CONFIG_DESCRIPTOR_UUID BLEUUID((uint16_t)0x2902)

#define LED_SERVICE_UUID "c7564aae-99ee-4874-848b-8a01f00d71bd"
#define LED_COLOR_CHARACTERISTIC_UUID "88db6efe-6abe-477f-bced-b5b0f5984320"

BLECharacteristic ledColorCharacteristic(LED_COLOR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                                                                            BLECharacteristic::PROPERTY_WRITE |
                                                                            BLECharacteristic::PROPERTY_WRITE_NR |
                                                                            BLECharacteristic::PROPERTY_NOTIFY |
                                                                            BLECharacteristic::PROPERTY_INDICATE |
                                                                            BLECharacteristic::PROPERTY_BROADCAST);

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *customCharacteristic)
  {
    std::string rcvString = customCharacteristic->getValue();
    for (int i = 0; i < 3; ++i)
    {
      currentColor[i] = rcvString[i];
    }
    customCharacteristic->setValue((uint8_t*) &currentColor, 3);
    leds->set(currentColor[0], currentColor[1], currentColor[2]);
    customCharacteristic->notify(true);
  }
};

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *server)
  {
    Serial.println("client connected");
    server->getAdvertising()->start();
  };

  void onDisconnect(BLEServer *server)
  {
    Serial.println("client disconnected");
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Started");
  leds->test();

  // Create the BLE Device
  BLEDevice::init("STF-Eis");

  // Create the BLE Server
  BLEServer *gattServer = BLEDevice::createServer();
  gattServer->setCallbacks(new ServerCallbacks());

  // Create the BLE Service
  BLEService *ledService = gattServer->createService(LED_SERVICE_UUID);

  // Create a BLE Characteristic
  BLEDescriptor *userDescriptionDescriptor = new BLEDescriptor(USER_DESCRIPTION_DESCRIPTOR_UUID);
  BLEDescriptor *clientCharacteristicConfigDescriptor = new BLEDescriptor(CLIENT_CHARACTERISTIC_CONFIG_DESCRIPTOR_UUID);
  userDescriptionDescriptor->setValue("Color of the STF-Eis");
  userDescriptionDescriptor->setValue({1, 1});
  clientCharacteristicConfigDescriptor->setAccessPermissions(ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE);

  ledColorCharacteristic.addDescriptor(userDescriptionDescriptor);
  ledColorCharacteristic.addDescriptor(clientCharacteristicConfigDescriptor);

  ledService->addCharacteristic(&ledColorCharacteristic);

  ledColorCharacteristic.setCallbacks(new MyCharacteristicCallbacks());
  ledColorCharacteristic.setValue((char *)&currentColor);

  gattServer->getAdvertising()->addServiceUUID(LED_SERVICE_UUID);

  // Start the service
  ledService->start();

  // Start advertising
  gattServer->getAdvertising()->start();
}

void loop()
{
  delay(1000);
  // Serial.println("aaaaaaaaaaa");
  // customCharacteristic.setValue((char *)&currentColor);
  // customCharacteristic.notify();
}