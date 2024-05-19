# ESP_MultiWiFi

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

- `void addAP(const char* ssid, const char* passphrase);`

  Adds a known WiFi network to the list of networks.

- `void loop(uint32_t connectTimeoutMs = WIFI_CONNECT_TIMEOUT);`

  Manages the WiFi connection process, should be called in the main loop of your sketch.

- `size_t getAPCount() const;`

  Returns the number of known WiFi networks added.

- `const char* getSSID(size_t index) const;`

  Returns the SSID of the network at the specified index.

- `const char* getPassphrase(size_t index) const;`

  Returns the passphrase of the network at the specified index.

## Debugging

This library provides debug output to help with troubleshooting. Ensure you have a debug macro defined in your project to enable debug messages.

```cpp
#define DEBUG_MW_LN(...) Serial.println(__VA_ARGS__)
#define DEBUG_MW_F(...) Serial.printf(__VA_ARGS__)
#define DEBUG_PRINT_LN(...) Serial.println(__VA_ARGS__)
#define DEBUG_PRINT_F(...) Serial.printf(__VA_ARGS__)
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Maintainer

Maintained by Mahdi Ghasemi ([GitHub Profile](https://github.com/giminotron5)).

## Contributions

Contributions are welcome! Please fork this repository and submit a pull request with your changes.