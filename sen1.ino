#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* ssid = "KSMJ";
const char* password = "07212005";
const char* serverUrl = "http://192.168.18.27/k_integrative/api.php";

#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        float temp = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (isnan(temp) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        // JSON payload
        String payload = "{\"name\":\"DHT Sensor\", \"temperature\":" + String(temp, 1) + ", \"humidity\":" + String(humidity, 0) + "}";
        
        Serial.print("Payload: ");
        Serial.println(payload); // Debugging output

        int httpResponseCode = http.POST(payload);
        
        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            Serial.println(http.getString());
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
    }

    delay(5000);
}
