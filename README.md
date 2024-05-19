# ESP_MultiWiFi Library

[![GitHub license](https://img.shields.io/github/license/gadaman-rm/ESP_MultiWiFi.svg)](https://github.com/gadaman-rm/ESP_MultiWiFi/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/gadaman-rm/ESP_MultiWiFi.svg)](https://github.com/gadaman-rm/ESP_MultiWiFi/releases/)
[![GitHub issues](https://img.shields.io/github/issues/gadaman-rm/ESP_MultiWiFi.svg)](https://github.com/gadaman-rm/ESP_MultiWiFi/issues)

## Description

`ESP_MultiWiFi` is a library for managing multiple WiFi connections on ESP8266 devices. This library automatically scans for available networks, sorts them based on signal strength, and attempts to connect to the strongest network available. If the connection fails, it moves to the next network in the list until a successful connection is established or all options are exhausted.

## Features

- Automatically scans for WiFi networks.
- Sorts networks by signal strength.
- Attempts to connect to known networks in order of strength.
- Handles reconnection in case of disconnection.
- Compatible with ESP8266.

## Installation

### Arduino IDE

1. Download the latest release from the [GitHub releases page](https://github.com/gadaman-rm/ESP_MultiWiFi/releases).
2. In the Arduino IDE, go to `Sketch` -> `Include Library` -> `Add .ZIP Library...` and select the downloaded ZIP file.
3. Alternatively, you can clone this repository into your Arduino libraries folder.

### PlatformIO

1. Add the following line to your `platformio.ini` file:
    ```ini
    lib_deps =
        https://github.com/gadaman-rm/ESP_MultiWiFi.git
    ```

## Usage

Include the library in your sketch and initialize it. Here's a basic example:

```cpp
#include <ESP_MultiWiFi.h>

// Create an instance of MultiWiFi
MultiWiFi multiWiFi;

void setup()
{
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.mode(WIFI_STA);

  // Add known networks
  multiWiFi.addAP("SSID1", "password1");
  multiWiFi.addAP("SSID2", "password2");
}

void loop()
{
  // Call the loop function to manage WiFi connections
  multiWiFi.loop();
}
```

## API
### Methods

- `void addAP(const char* ssid, const char* passphrase)`: Adds a new Access Point (AP) to the list.
- `bool deleteAp(const char *ssid)`: Deletes an Access Point (AP) from the list.
- `bool existsAP(const char *ssid)`: Checks if an Access Point (AP) exists in the list.
- `void clearAPlist()`Clears the list of Access Points (APs).

- `uint8_t getApCount()`:Returns the number of Access Points (APs) in the list.
- `const char* getSSID(size_t index)`: Returns the SSID of the network at the specified index.
- `const char* getPassphrase(size_t index);`: Returns the passphrase of the network at the specified index.

- `void loop(uint32_t connectTimeoutMs = WIFI_CONNECT_TIMEOUT)`: Manages the WiFi connection process, should be called in the main loop of your sketch.

## Debugging

This library provides debug output to help with troubleshooting. To enable debug messages, uncomment the following line in the library header file (`ESP_MultiWiFi.h`):

```cpp
// #define ENABLE_MULTI_WIFI_DEBUG // Uncomment to enable debug output
```
If debug mode is enabled, ensure that you configure the serial communication in the setup function:

```cpp
void setup()
{
  Serial.begin(115200);
  // other setup code
}

```
Once debug mode is enabled, you can use the following macros to handle the debug messages:

- `DEBUG_MW_F(...)`: Print messages with the library prefix using `printf`.
- `DEBUG_MW(...)`: Print messages with the library prefix using `print`.
- `DEBUG_MW_LN(...)`: Print messages with the library prefix using `println`.
- `DEBUG_PRINT_F(...)`: Print messages using `printf`.
- `DEBUG_PRINT(...)`: Print messages using `print`.
- `DEBUG_PRINT_LN(...)`: Print messages using `println`.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Maintainer

Maintained by Mahdi Ghasemi ([GitHub Profile](https://github.com/giminotron5)).

## Contributions

Contributions are welcome! Please fork this repository and submit a pull request with your changes.