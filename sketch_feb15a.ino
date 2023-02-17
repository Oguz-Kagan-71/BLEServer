#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define UUIDs for the original service and characteristic
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Define UUIDs for the new service and characteristic
#define NEW_SERVICE_UUID "b5a8a4a4-ad25-11ed-afa1-0242ac120002" //uid generatörden generate edilen  custom uniqie ID 
#define NEW_CHARACTERISTIC_UUID "c12038c4-ad25-11ed-afa1-0242ac120002" //uid generatörden generate edilen  custom uniqie ID 


BLEServer *pServer;  //server ve characteristic tanımlamaları
BLEService *pService; 
BLECharacteristic *pCharacteristic; 
BLEService *pNewService; 
BLECharacteristic *pNewCharacteristic; 

int hexValue = 0x71;  // başlangıçtaki değişken değeri
int lastInput = 0; //son inputu saklamak için kullanılan değişken
void setup()
{
  Serial.begin(115200);  //serial vereceğim inputlar için 
  Serial.println("Starting BLE Server!"); 
  
  // Initialize the BLE device and create the server
  BLEDevice::init("ESP32-BLE-Server"); //device ismi 
  pServer = BLEDevice::createServer(); //kütüphane fonksiyonu kullanılarak server başlatıluıyor

  // Create the original service and characteristic
  pService = pServer->createService(SERVICE_UUID);  // service oluşturuldu
  pCharacteristic = pService->createCharacteristic(  //oluşturulan servis altında characteristic oluşturuldu 
                                         CHARACTERISTIC_UUID,  // unique ıd verildi
                                         BLECharacteristic::PROPERTY_READ | //read özelliği verildi
                                         BLECharacteristic::PROPERTY_WRITE  //write özelliği verildi
                                       );
  pCharacteristic->setValue("Hello, World!");  
  pService->start();  //konfigürasyonları yapılan service başlatılıyor

  // Create the new service and characteristic
  pNewService = pServer->createService(NEW_SERVICE_UUID); //  service oluşturuldu
  pNewCharacteristic = pNewService->createCharacteristic( //oluşturulan servis altında characteristic oluşturuldu 
                                          NEW_CHARACTERISTIC_UUID,  // unique ıd verildi
                                          BLECharacteristic::PROPERTY_READ | //read özelliği verildi
                                          BLECharacteristic::PROPERTY_WRITE  //write özelliği verildi
                                       );
  pNewCharacteristic->setValue(hexValue);  //Değişken değeri atandı
  pNewService->start();

  //advertising konfigürasyonları 
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();  //BLEDevice::getAdvertising() çağrısı, reklam verilerini yönetmek için bir BLEAdvertising nesnesi oluşturur
  pAdvertising->addServiceUUID(SERVICE_UUID);  
  pAdvertising->addServiceUUID(NEW_SERVICE_UUID);
  pAdvertising->setScanResponse(true); //scanlenebilir bir advertise oluşturuldu
  pAdvertising->setMinPreferred(0x06);  //pAdvertising->setMinPreferred() metodu, reklam paketlerinin iki farklı türde boyutu belirleyen bir yöntemdir. 
  pAdvertising->setMinPreferred(0x12); //setMinPreferred() metoduna iki parametre verilmelidir, bunlar minimum ve maksimum boyutlar (byte cinsinden) sırasıyla.
  BLEDevice::startAdvertising();   

  Serial.println("BLE server setup complete!");
}

void loop(){ //loop kısmında 2. servicenin  serialden verilen değişkeni saklaması için yazılmıştır.
  // Wait for user input
  while (Serial.available() <= 0) { //serialden gelen ver boyutu 0'dan büyük ise 
    delay(50);
  }

  // Read user input and set it as the value of the new characteristic
  String inputString = Serial.readStringUntil('\n');  
  pNewCharacteristic->setValue(inputString.c_str()); 

  Serial.print("The new value is: ");
  Serial.println(inputString);
}

  // Read the value of the original characteristic
  //std::string value = pCharacteristic->getValue();
  //Serial.print("The original characteristic value is: ");
  //Serial.println(value.c_str());

  // Read the value of the new characteristic
  //std::string newValue = pNewCharacteristic->getValue();
  //Serial.print("The new characteristic value is: ");
  //Serial.println(newValue.c_str());

  // Check if there is any incoming data from the serial port
  //if (Serial.available() > 0) {
    // Read the incoming byte and set the new characteristic value
    //int incomingByte = Serial.read();
    //lastInput = incomingByte;
    //pNewCharacteristic->setValue(lastInput);
     // set lastInput to incomingByte
    //Serial.print("New characteristic value set to: ");
    //Serial.println(lastInput, HEX);
   // delay(1000);}}
    
