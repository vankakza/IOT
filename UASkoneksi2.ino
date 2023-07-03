#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h> //mengirim data ke server

const char* ssid = "wipoll";                //Nama wifi
const char* password = "vanka123";          //pw wifi
const char* server = "te20.project-anemo.app"; //server
const char* serverUrl = "https://te20.project-anemo.app/api/device/status"; 

const String deviceId = "esp32";            //nama device
unsigned long lastUpdateTime = 0;           //inisialisasi nilai awal
const unsigned long updateInterval = 30000; //akan mengupdate koneksi dalam waktu 30 detik

const int anemometerPin = 32;               //pin anemo
volatile int interruptCounter = 0;          //counter untuk nilai kecepatan angin
float windSpeed = 0.0;                      //Kecepatan angin (m/s)

const int dhtPin = 25;                      //pin DH22
const int dhtType = DHT22;                  //Tipe sensor dht

const int buzzerPin = 26;                   //Pin buzzer

WiFiClient client;                          
DHT dht(dhtPin, dhtType);

void IRAM_ATTR anemometerInterrupt() {
  interruptCounter++;                       //menghitung jumlah interupt (perubahan nilai)
}

void setup() {
  Serial.begin(115200);

  pinMode(anemometerPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(anemometerPin), anemometerInterrupt, FALLING);

  dht.begin();

  pinMode(buzzerPin, OUTPUT);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password, 0, NULL, true);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // mengecek apakah sudah waktunya mengirim status update
  if (millis() - lastUpdateTime >= updateInterval) {
    sendStatusUpdate();
    lastUpdateTime = millis();
  }
<<<<<<< HEAD

  //jika tidak ada perubahan nilai pada anemo, maka tuliskan kecepatan = 0
=======
>>>>>>> 6162c003505b5411ba1486abc0e1bbf9d5093c7c
  if (interruptCounter == 0){
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    windSpeed = 0;

    Serial.print("Wind Speed: ");
    Serial.print(windSpeed);

    if (windSpeed == 0 ) {
      digitalWrite(buzzerPin, HIGH);
      delay(500);
      digitalWrite(buzzerPin, LOW);
    }
    sendData(temperature, humidity, windSpeed);
<<<<<<< HEAD
    //jika ada perubahan nilai pada anemo, masukan nilai kecepatan angin
=======
>>>>>>> 6162c003505b5411ba1486abc0e1bbf9d5093c7c
  }else if (interruptCounter > 0) {
    windSpeed = (float)interruptCounter / 2.0;
    interruptCounter = 0;

    Serial.print("Wind Speed: ");
    Serial.print(windSpeed);
    Serial.println(" m/s");

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Nyalakan buzzer jika kecepatan angin lebih dari 5 m/s
    if (windSpeed > 3.5) {
      digitalWrite(buzzerPin, HIGH);
      delay(500);
      digitalWrite(buzzerPin, LOW);
    }

    sendData(temperature, humidity, windSpeed);
  }

  delay(1000);
}
<<<<<<< HEAD

//fungsi untuk mengirim data output sensor
=======
>>>>>>> 6162c003505b5411ba1486abc0e1bbf9d5093c7c
void sendData(float temperature, float humidity, float windSpeed) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://" + String(server) + "/inputdata/" + String(temperature) + "/" + String(humidity) + "/" + String(windSpeed);
    
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      Serial.println("Data successfully sent to server");
    } else {
      Serial.print("Error sending data to server. Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi connection lost. Cannot send data to server.");
  }
}

//mengirim konektivitas esp32 online/tidak
void sendStatusUpdate() {
if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://" + String(server) + "/inputstatus/" + String(deviceId);
    
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      Serial.println("Status successfully sent to server");
    } else {
      Serial.print("Error sending status to server. Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi connection lost. Cannot send data to server.");
  }
}
