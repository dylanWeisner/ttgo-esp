

#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36
#include <SPI.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

TFT_eSPI tft = TFT_eSPI();
const char* ssid = "";//add your stuff in the quotes
const char* password = "";


void setup() {

  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  tft.begin();
  tft.setRotation(1);


  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.println("Connecting...");

  }

  Serial.println("Connected");
  Serial.println(WiFi.localIP());

}

void loop() {

  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient client;
    client.begin("https://chain.so/api/v2/get_price/DOGE/USD");
    int httpCode = client.GET();

    if (httpCode > 0) {
      String payload = client.getString();
      Serial.println("______________________");
      Serial.println(httpCode);
      Serial.println(payload);
      Serial.println("______________________");
      ///////////////////////////////////////////////////////

      // char* input;
      // size_t inputLength; (optional)
      // char* input = "{\"status\":\"success\",\"data\":{\"network\":\"DOGE\",\"prices\":[{\"price\":\"0.294\",\"price_base\":\"USD\",\"exchange\":\"gemini\",\"time\": 1622317926}]}}}";

      StaticJsonDocument<400> doc;

      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      const char* status = doc["status"]; // "success"

      const char* data_network = doc["data"]["network"]; // "DOGE"

      for (JsonObject elem : doc["data"]["prices"].as<JsonArray>()) {

        const char* price = elem["price"]; // "0.2945", "0.29459"
        const char* price_base = elem["price_base"]; // "USD", "USD"
        const char* exchange = elem["exchange"]; // "gemini", "binance"
        long time = elem["time"]; // 1622322213, 1622322199

        /////////////////////////////////////////////////////////////////////////////
        //////////////DIPLAY////////////////////////////////////////////////////
        tft.loadFont(AA_FONT_LARGE);
        tft.setCursor(0, 0);
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.println("DOGE PRICE");
        tft.setTextColor(TFT_PINK, TFT_BLACK);
        tft.println(price);
      }

    } else {
      Serial.println("Error in Http");
    }
    client.end();
  }
  delay(10000);
}
