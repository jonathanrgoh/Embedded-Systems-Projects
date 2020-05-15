#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>

//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_ACCELERATION               "2713"
#define BLE_UUID_COUNTER                    "1A3AC130-31EE-758A-BC50-54A61958EF81"
#define BLE_UUID_RESET_COUNTER              "FE4E19FF-B132-0099-5E94-3FFB2CF07940"

#define BLE_UUID_PROXIMITY                  "5555"

//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEFloatCharacteristic accelerationCharacteristic( BLE_UUID_ACCELERATION, BLERead | BLENotify );
    BLEIntCharacteristic proximityCharacteristic( BLE_UUID_PROXIMITY, BLERead | BLENotify);
BLEUnsignedLongCharacteristic counterCharacteristic( BLE_UUID_COUNTER, BLERead | BLENotify );
BLEBoolCharacteristic resetCounterCharacteristic( BLE_UUID_RESET_COUNTER, BLEWriteWithoutResponse );

const int BLE_LED_PIN = LED_BUILTIN;
//const int LED_PWR = 2;
const int RSSI_LED_PIN = LED_PWR;

void setup()
{
  Serial.begin( 9600 );
  //  while ( !Serial );

  pinMode( BLE_LED_PIN, OUTPUT );
  pinMode( RSSI_LED_PIN, OUTPUT );

  if ( !IMU.begin() )
  {
    Serial.println( "Failed to initialize IMU!" );
    while ( 1 );
  }
  Serial.print( "Accelerometer sample rate = " );
  Serial.print( IMU.accelerationSampleRate() );
  Serial.println( " Hz" );

    if ( !APDS.begin() )
    {
      Serial.println( "Failed to initialize APDS!" );
      while ( 1 );
    }


  if( setupBleMode() )
  {
    digitalWrite( BLE_LED_PIN, HIGH );
  }
} // setup

void loop()
{
  static unsigned long counter = 0;
  static long previousMillis = 0;

  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  if ( central )
  {
    Serial.print( "Connected to central: " );
    Serial.println( central.address() );
    digitalWrite(LED_PWR,HIGH);
    delay(100);
    digitalWrite(LED_PWR,LOW);
    
    while ( central.connected() )
    {
      delay(1000);
      
      
      if ( IMU.accelerationAvailable() )
      {
        float accelerationX, accelerationY, accelerationZ;
        IMU.readAcceleration( accelerationX, accelerationY, accelerationZ );
        Serial.println(accelerationX);
        accelerationCharacteristic.writeValue( accelerationX );
      }
   
          if ( APDS.proximityAvailable() )
          {
            int prox;
            prox = APDS.readProximity();
            Serial.println(prox);
            proximityCharacteristic.writeValue(prox);
            
          }

      
      counter++;
      counterCharacteristic.writeValue( counter );
    } // while connected

    Serial.print( F( "Disconnected from central: " ) );
    Serial.println( central.address() );
  } // if central
} // loop

bool setupBleMode()
{
  if ( !BLE.begin() )
  {
    return false;
  }

  // set advertised local name and service UUID:
  BLE.setDeviceName( "Jon Arduino Nano 33 BLE" );
  BLE.setLocalName( "Jon Arduino Nano 33 BLE" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( accelerationCharacteristic );
  testService.addCharacteristic( counterCharacteristic );
  testService.addCharacteristic( resetCounterCharacteristic );
    testService.addCharacteristic( proximityCharacteristic);

  // add service
  BLE.addService( testService );

  // set the initial value for the characeristic:
  
  accelerationCharacteristic.writeValue( 0.0 );
    proximityCharacteristic.writeValue( 0 );
  counterCharacteristic.writeValue( 0 );

  // start advertising
  BLE.advertise();

  return true;
}

 
