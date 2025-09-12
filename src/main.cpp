#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// --- WiFi ---
const char* WIFI_SSID = "iphone";
const char* WIFI_PASS = "k1k2k3k4";
// --- ThingSpeak Read API ---
const char* READ_URL = "https://api.thingspeak.com/channels/3071994/feeds.json?api_key=AEKGBABBEW3LV3HQ";

// --- Mảng lưu dữ liệu ---
float temps[10];
float hums[10];
int n = 0;   // số phần tử hiện có

void setup() {
  Serial.begin(9600);
  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(READ_URL);
    int code = http.GET();

    if (code == 200) {
      String res = http.getString();

      // Parse JSON
      StaticJsonDocument<2048> doc;
      if (deserializeJson(doc, res) == DeserializationError::Ok) {
        JsonArray feeds = doc["feeds"];
        n = feeds.size();

        for (int i = 0; i < n; i++) {
          temps[i] = atof(feeds[i]["field1"]);
          hums[i]  = atof(feeds[i]["field2"]);
        }

        // In ra màn hình
        Serial.println("=== Du lieu moi nhat ===");
        for (int i = 0; i < n; i++) {
          Serial.printf("%d) Nhiet do = %.2f *C, Do am = %.2f %%\n", i+1, temps[i], hums[i]);
        }
      } else {
        Serial.println("Loi parse JSON!");
      }
    } else {
      Serial.printf("HTTP loi: %d\n", code);
    }
    http.end();
  }

  delay(15000); // đọc lại sau 15 giây
}
