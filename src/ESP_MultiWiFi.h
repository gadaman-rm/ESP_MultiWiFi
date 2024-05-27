#ifndef MultiWiFi_h
#define MultiWiFi_h

#include <Arduino.h>
#include <vector>
#include <ESP8266WiFi.h>

/*---------------- Definitions ----------------*/
// #define ENABLE_MULTI_WIFI_DEBUG // Uncomment to enable debug output

// Serial port definition
#ifdef ENABLE_MULTI_WIFI_DEBUG
  #define MULTI_WIFI_DEBUG_PORT Serial // Debug port
#endif
// Definitions debug macros
#ifdef MULTI_WIFI_DEBUG_PORT
  #define DEBUG_MW_F(...)     MULTI_WIFI_DEBUG_PORT.printf("[MWiFi] " __VA_ARGS__)
  #define DEBUG_MW(...)       MULTI_WIFI_DEBUG_PORT.print("[MWiFi] " __VA_ARGS__)
  #define DEBUG_MW_LN(...)    MULTI_WIFI_DEBUG_PORT.println("[MWiFi] " __VA_ARGS__)
  #define DEBUG_PRINT_F(...)  MULTI_WIFI_DEBUG_PORT.printf(__VA_ARGS__)
  #define DEBUG_PRINT(...)    MULTI_WIFI_DEBUG_PORT.print(__VA_ARGS__)
  #define DEBUG_PRINT_LN(...) MULTI_WIFI_DEBUG_PORT.println(__VA_ARGS__)
#else
  #define DEBUG_MW_F(...)
  #define DEBUG_MW(...)
  #define DEBUG_MW_LN(...)
  #define DEBUG_PRINT_F(...)
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINT_LN(...)
#endif

#define WIFI_CONNECT_TIMEOUT 10000 // Default WiFi connection timeout in ms

/*-----------------------------------------------------------------------------*/
class MultiWiFi
{
private:

  /*---------------- Variables ----------------*/

  struct WifiCredential
  {
    char ssid[32];
    char passphrase[64];
  };

  std::vector<WifiCredential> _apList;
  std::vector<uint8_t>        _apListIndex;

  uint32_t _lastConnectionAttemptTime;
  uint8_t  _currentNetworkIndex;

public:

  /*---------------- Functions ----------------*/

  bool addAp(const char *ssid, const char *passphrase);
  bool deleteAp(const char *ssid);
  bool existsAP(const char *ssid);
  void clearAPlist();

  uint8_t     getApCount();
  const char *getSSID(uint8_t index);
  const char *getPassphrase(uint8_t index);

  void loop(uint32_t connectTimeoutMs = WIFI_CONNECT_TIMEOUT);

private:

  /*---------------- Private Functions ----------------*/

  bool    isValidLength(const char *data, size_t maxLength);
  uint8_t scanCompleted(uint8_t networksFound);
};
/*-----------------------------------------------------------------------------*/
#endif
