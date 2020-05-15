#include <PubSubClient.h>
#include <WiFi.h>

#include "BLEDevice.h"
//#include "BLEScan.h"


#define WIFISSID "Jonnyg" // Put your WifiSSID here
#define PASSWORD "12345" // Put your wifi password here
#define TOKEN "BBFF-Hw3Drg5Q6P30IqKvBSeSVDPoJW4xeV" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "jonathangoh123" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
                                           //it should be a random and unique ascii string and different from all other devices

/****************************************
 * Define Constants
 ****************************************/
#define VARIABLE_LABEL "IMU" // Assing the variable label/////////////////////////
#define VARIABLE_LABEL2 "Proximity" // Assing the variable label/////////////////////////
#define DEVICE_LABEL "esp32" // Assig the device label

#define SENSOR 12 // Set the GPIO12 as SENSOR

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char payload2[100];
char topic[150];
char topic2[150];
// Space to store values to send
char str_sensor[10];
char str_sensor2[10];

float IMUData;
int ProxData;

/****************************************
 * Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, reconnect=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}


/****************************************
 * OLD STUFF
 ****************************************/


// The remote service we wish to connect to.
static BLEUUID serviceUUID("9A48ECBA-2E92-082F-C079-9E75AAE428B1");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("2713");
static BLEUUID    charUUID2("5555");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLERemoteCharacteristic* pRemoteCharacteristic2;
static BLEAdvertisedDevice* myDevice;


static void IMUNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,uint8_t* pData,size_t length,bool isNotify) {
    Serial.print("Notify callback for IMU: ");
    float* number = (float*)pData;
    IMUData = *number; 
    Serial.println(*number);

//          float sensor = IMUData; //////////////////////////
//      sprintf(payload, "%s", "");
//      sprintf(payload, "{\"%s\":", VARIABLE_LABEL);
//      /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
//      dtostrf(sensor, 4, 2, str_sensor);
//      //Serial.println("str_sensor: ");
//      //Serial.println(str_sensor);
//      sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
//      Serial.println("Publishing IMU data to Ubidots Cloud");
//      client.publish(topic, payload);
}
static void proxNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,uint8_t* pData,size_t length,bool isNotify) {
    Serial.print("Notify callback for Proximity: ");
    int* number2 = (int*)pData;
    ProxData = *number2;
    Serial.println(*number2);

      int sensor2 = ProxData; //////////////////////////
      sprintf(payload2, "%s", "");
      sprintf(payload2, "{\"%s\":", VARIABLE_LABEL2);
      /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
      dtostrf(sensor2, 4, 2, str_sensor2);
      //Serial.println("str_sensor: ");
      //Serial.println(str_sensor);
      sprintf(payload2, "%s {\"value\": %s}}", payload2, str_sensor2); // Adds the value
      Serial.println("Publishing Proximity data to Ubidots Cloud");
      client.publish(topic2, payload2);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");

///////////////////////////////////////////////////////////////////////////////////////////////

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find IMU UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found IMU characteristic");

        pRemoteCharacteristic2 = pRemoteService->getCharacteristic(charUUID2);
    if (pRemoteCharacteristic2 == nullptr) {
      Serial.print("Failed to find Proximity UUID: ");
      Serial.println(charUUID2.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found Proximity characteristic");
    
///////////////////////////////////////////////////////////////////////////////////////////////

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value1 = pRemoteCharacteristic->readValue();
      Serial.print("The IMU value was: ");
      //Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic2->canRead()) {
      std::string value2 = pRemoteCharacteristic2->readValue();
      Serial.print("The Proximity value was: ");
      //Serial.println(value.c_str());
    }
    
///////////////////////////////////////////////////////////////////////////////////////////////

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(IMUNotifyCallback);

    if(pRemoteCharacteristic2->canNotify())
      pRemoteCharacteristic2->registerForNotify(proxNotifyCallback);

    connected = true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

/****************************************
 * 
 ****************************************/

  WiFi.begin(WIFISSID, PASSWORD);
  // Assign the pin as INPUT 
  pinMode(SENSOR, INPUT);/////////////////////////////////

  Serial.println();
  Serial.print("Wait for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(700);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

/****************************************
 * 
 ****************************************/

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(15, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {
/****************************************
 * 
 ****************************************/
  if (!client.connected()) {
    reconnect();
  }

  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
  


/****************************************
 * 
 ****************************************/

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  }
  
  else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }

//  if (doConnect == true) {
//      float sensor = IMUData; //////////////////////////
//      sprintf(payload, "%s", "");
//      sprintf(payload, "{\"%s\":", VARIABLE_LABEL);
//      /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
//      dtostrf(sensor, 4, 2, str_sensor);
//      //Serial.println("str_sensor: ");
//      //Serial.println(str_sensor);
//      sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
//      Serial.println("Publishing data to Ubidots Cloud");
//      client.publish(topic, payload);
//      //client.loop();
//      //delay(1000);
//  }
//  
//  client.loop();
  delay(1000); // Delay a second between loops.
} // End of loop
