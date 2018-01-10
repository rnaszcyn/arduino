
/*
 * Bluetooh officisting signal device
 * v0.01
 * 
 * This code allows a bluetooth device Adafruit NRF32 feather to behave as bluetooth
 * signaling device. The device connected to this device should use the signals to drive
 * behavior of an officiating timing program.
 * 
 */

#include <bluefruit.h>

#define ADV_TIMEOUT   60 // seconds

BLEDis            bledis; // DIS (Device Information Service) helper class instance
/**
 * Using custom UUIDS generated at https://www.uuidgenerator.net/version1
 * All Bluetooth SIG defined UUIDs use a common base UUID: 0x0000xxxx-0000-1000-8000-00805F9B34FB
 * The unique 16-bit UUID that replaces the x’s in the common base UUID described above i used for services and characterisitc
 * 
 * block of 10 uuid's
 * 3be266d0-ef68-11e7-8c3f-9a214cf093ae   0x66D0
 * 3be26978-ef68-11e7-8c3f-9a214cf093ae   0x6978
 * 3be26ad6-ef68-11e7-8c3f-9a214cf093ae
 * 3be27080-ef68-11e7-8c3f-9a214cf093ae
 * 3be271e8-ef68-11e7-8c3f-9a214cf093ae
 * 3be2735a-ef68-11e7-8c3f-9a214cf093ae
 * 3be275bc-ef68-11e7-8c3f-9a214cf093ae
 * 3be277f6-ef68-11e7-8c3f-9a214cf093ae
 * 3be27954-ef68-11e7-8c3f-9a214cf093ae
 * 3be27d46-ef68-11e7-8c3f-9a214cf093ae
 */
BLEService        bleButtonService; // = BLEService(0x66D0); // need to figure out how to use 128 bit UUID
BLECharacteristic bleButtonServiceC1; // = BLECharacteristic(0x6978); // need to figure out how to use 128 bit UUID



// function prototypes
void bleConnected(uint16_t conn_handle);
void bleDisconnected(uint16_t conn_handle, uint8_t reason);
void startAdvertising(uint8_t info);
void setupButtonService(void);
 
void setup() {
  // set up and start serial debugging info
  Serial.begin(115200);
  Serial.println("BLE Officiating Signaler");
  Serial.println("--------------------------\n");

  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Officiating BLE GeeWizMo");
  Bluefruit.setConnectCallback( bleConnected );
  Bluefruit.setDisconnectCallback( bleDisconnected );

  // Set up and start advertising
  startAdvertising();

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("Richard Naszcyniec");
  bledis.setModel("BLE Lacrosse Signaler");
  bledis.setHardwareRev("0.1");
  bledis.setSoftwareRev("0.01");
  bledis.begin();
  
  //configure and start the button service
  setupButtonService();
}

void bleDisconnected(uint16_t conn_handle, uint8_t reason) {
  Serial.println("bluetooth disconnected");
}

void bleConnected(uint16_t conn_handle) {
  Serial.println("bluetooth connected");
}

void startAdvertising(void){   
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html
   */
  // Bluefruit.Advertising.setStopCallback(adv_stop_callback);
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in units of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(ADV_TIMEOUT);      // Stop advertising entirely after ADV_TIMEOUT seconds 
}

void setupButtonService(void) {
  // Configure the bluetooth service for button status
  // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml
  // Supported Characteristics:
  // Name                         UUID    Requirement Properties
  // ---------------------------- ------  ----------- ----------
  // All button status            0x6978  Manadatory  Notify
  // Heart Rate Measurement       0x2A37  Mandatory   Notify
  // Body Sensor Location         0x2A38  Optional    Read
  // Heart Rate Control Point     0x2A39  Conditional Write       <-- Not used here
  
  bleButtonService = BLEService(0x66D0);
  bleButtonService.begin(); // must begin service before adding a characteritic

  // Configure the button status characteristic
  // Permission = Notify
  // Min Len    = 1
  // Max Len    = 2
  //    B0      = UINT8  - Flag (MANDATORY)
  //      b7    = button 7 state
  //      b6    = button 6 state
  //      b4    = button 5 state
  //      b4    = button 4 state
  //      b3    = button 3 state
  //      b2    = button 2 state
  //      b1    = button 1 state
  //      b0    = button 0 state
  //    B1      = reserved
  bleButtonServiceC1 = BLECharacteristic(0x6978);
  bleButtonServiceC1.setProperties(CHR_PROPS_NOTIFY);
  bleButtonServiceC1.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  bleButtonServiceC1.setFixedLen(2);
  bleButtonServiceC1.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
