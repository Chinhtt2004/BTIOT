#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 4         
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);
const char* WIFI_SSID = "iphone";   
const char* WIFI_PASS = "k1k2k3k4";       
const char* BASE_URL = "https://api.thingspeak.com/update?api_key=NTLV84NV3CHBRKIZ"; 

void setup() {
  Serial.begin(9600);   
  dht.begin();          

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Dang ket noi WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nDa ket noi WiFi!");
  Serial.print("IP cua ESP32: ");
  Serial.println(WiFi.localIP()); 
}

void loop() {
  delay(15000); // Giữ khoảng cách 15 giây giữa 2 lần gửi (ThingSpeak quy định)


  float humidity = dht.readHumidity();      
  float temperature = dht.readTemperature(); 

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Khong doc duoc du lieu tu cam bien DHT!");
    return;
  }


  char params[64];
  snprintf(params, sizeof(params), "&field1=%.2f&field2=%.2f", temperature, humidity);
  String url = String(BASE_URL) + String(params);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);

    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("HTTP %d, Phan hoi: %s\n", httpCode, http.getString().c_str());
    } else {
      Serial.printf("HTTP GET that bai, code: %d\n", httpCode);
    }

    http.end();
  } else {
    Serial.println("Mat ket noi WiFi!");
  }
}
