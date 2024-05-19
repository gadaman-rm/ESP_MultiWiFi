#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP_MultiWiFi.h>

/*---------------------------------------------------------*/
MultiWiFi multiWifi;
uint32_t  prevMillis = 0;

/*---------------------------------------------------------*/
void setup()
{
  /*---------------- Initialization area ----------------*/
  pinMode(LED_BUILTIN, OUTPUT);

  // Serial Initialization
  Serial.begin(115200);
  Serial.println();

  // Set WiFi mode to WIFI_STA (station mode)
  WiFi.mode(WIFI_STA);
  /*---------------- End of initialization area ----------------*/

  // Adding Access points
  multiWifi.addAp("My_SSID_1", "12345678");
  multiWifi.addAp("My_SSID_2", "12345678");
  multiWifi.addAp("My_SSID_3", "12345678");
  multiWifi.addAp("My_SSID_4", "12345678");

  // Get number of access points in multiWifi list
  Serial.print("Number of access points in MultiWiFi list: ");
  Serial.println(multiWifi.getApCount());

  // Print all access points SSID
  Serial.print("Access points SSID: ");
  for(int i = 0; i < multiWifi.getApCount(); i++)
  {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(multiWifi.getSSID(i));
  }
}
void loop()
{
  // MultiWiFi loop for managing WiFi connection
  // Note: This function does not block the execution of other code.
  multiWifi.loop();

  // Blink LED every 200ms without using delay
  if(millis() - prevMillis >= 200)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    prevMillis = millis();
  }
}
