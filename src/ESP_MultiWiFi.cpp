#include "ESP_MultiWiFi.h"

/*---------------- Public Functions ----------------*/

/**
  Adds a new Access Point (AP) to the list.

  @param ssid The SSID of the new AP (Max length 32).
  @param passphrase The passphrase of the new AP (Max length 64).
  @return true if the AP is successfully added, false otherwise.
*/
bool MultiWiFi::addAp(const char *ssid, const char *passphrase)
{
  if(!isValidLength(ssid, 32) || !isValidLength(passphrase, 64)) { return false; }

  if(existsAP(ssid)) { return false; }

  WifiCredential newAp;
  strncpy(newAp.ssid, ssid, sizeof(newAp.ssid) - 1);
  newAp.ssid[sizeof(newAp.ssid) - 1] = '\0';

  strncpy(newAp.passphrase, passphrase, sizeof(newAp.passphrase) - 1);
  newAp.passphrase[sizeof(newAp.passphrase) - 1] = '\0';

  _apList.push_back(newAp);
  return true;
}

/**
 Deletes an Access Point (AP) from the list.

  @param ssid The SSID of the AP to be deleted (Max length 32)
  @return true if the AP is successfully deleted, false otherwise.
*/
bool MultiWiFi::deleteAp(const char *ssid)
{
  if(!isValidLength(ssid, 32)) { return false; }

  for(auto it = _apList.begin(); it != _apList.end(); ++it)
  {
    if(strncmp((*it).ssid, ssid, sizeof((*it).ssid)) == 0)
    {
      _apList.erase(it);
      return true;
    }
  }
  return false;
}

/**
 Checks if an Access Point (AP) exists in the list.

  @param ssid The SSID of the AP to be checked (Max length 32).
  @return true if the AP exists in the list, false otherwise.
*/
bool MultiWiFi::existsAP(const char *ssid)
{
  if(!isValidLength(ssid, 32)) { return false; }

  for(auto ap: _apList)
  {
    if(strncmp(ap.ssid, ssid, sizeof(ap.ssid)) == 0) { return true; }
  }
  return false;
}

/**
  Clears the list of Access Points (APs).
*/
void MultiWiFi::clearAPlist() { _apList.clear(); }

/**
  Returns the SSID of an Access Point (AP) given its index.

  @param index The index of the AP in the list.
  @return The SSID of the AP at the given index, or an empty string if the index is out of range.
*/
const char *MultiWiFi::getSSID(uint8_t index)
{
  if(index < _apList.size()) { return _apList[index].ssid; }
  return nullptr;
}

/**
  Returns the passphrase of an Access Point (AP) given its index.

  @param index The index of the AP in the list.
  @return The passphrase of the AP at the given index, or an empty string if the index is out of range.
*/
const char *MultiWiFi::getPassphrase(uint8_t index)
{
  if(index < _apList.size()) { return _apList[index].passphrase; }
  return nullptr;
}

/**
  Returns the number of Access Points (APs) in the list.

  @return The number of APs in the list.
*/
uint8_t MultiWiFi::getApCount() { return _apList.size(); }

/**
  - The loop function is responsible for managing WiFi connection.

  - It scans for available networks, sorts them based on signal strength,
    and attempts to connect to the networks sequentially until a connection
    is established or the timeout period is reached.

  @param connectTimeoutMs: Timeout period in milliseconds for attempting
                           to connect to a WiFi network (default is WIFI_CONNECT_TIMEOUT).

  @note This function does not block the execution of other code.
  @note This function operates only when the WiFi is in WIFI_STA (Station mode).
*/
void MultiWiFi::loop(uint32_t connectTimeoutMs)
{
  static enum class LoopState { CHECK_CONNECTION, SCAN_NETWORKS, CONNECT_TO_NETWORK } state = LoopState::CHECK_CONNECTION;
  static unsigned long lastActionTime = 0;

  // Check if WiFi mode is WIFI_STA (Station mode)
  if(WiFi.getMode() != WIFI_STA)
  {
    // Reset LoopState and variables value
    state = LoopState::CHECK_CONNECTION;
    lastActionTime = 0;
    _lastConnectionAttemptTime = 0;
    _currentNetworkIndex = 0;
    return;
  }
  
  if(millis() - lastActionTime < 1000 && state == LoopState::CHECK_CONNECTION) { return; } // Check every 1 second

  switch(state)
  {
    case LoopState::CHECK_CONNECTION:
    {
      if(WiFi.status() == WL_CONNECTED)
      {
        // Connection is already established, no need to scan or connect
        break;
      }
      else
      {
        DEBUG_MW_LN("Status: Disconnect!");
        state = LoopState::SCAN_NETWORKS;
      }
      break;
    }

    case LoopState::SCAN_NETWORKS:
    {
      WiFi.scanNetworksAsync(
        [this](int numNetworks)
        {
          if(numNetworks <= 0) { DEBUG_MW_LN("No networks found."); }
          else
          {
            // Print the number of networks found in the area
            DEBUG_MW_F("Number of networks found: %d\n", numNetworks);

            // Get the known networks and sort them based on signal strength
            uint8_t matchingNetworks = this->scanCompleted(numNetworks);
            // Print the number of networks matching the AP list
            DEBUG_MW_F("Number of networks matching with AP list: %d\n", matchingNetworks);

            DEBUG_MW_LN("Sorted networks :");
            for(size_t i = 0; i < _apListIndex.size(); i++) { DEBUG_PRINT_F("[%d] -> %s\n", i, _apList.at(_apListIndex.at(i)).ssid); }

            state = LoopState::CONNECT_TO_NETWORK;
          }
        });
      break;
    }

    case LoopState::CONNECT_TO_NETWORK:
    {
      // Attempt to connect to the sorted networks one by one
      // Implementation of connecting to networks goes here

      // If the connection is established, set state back to CHECK_CONNECTION
      if(WiFi.status() != WL_CONNECTED)
      {
        if(_currentNetworkIndex < _apListIndex.size())
        {
          const char *ssid       = _apList.at(_apListIndex[_currentNetworkIndex]).ssid;
          const char *passphrase = _apList.at(_apListIndex[_currentNetworkIndex]).passphrase;

          // Attempt to connect to selected network
          if(_lastConnectionAttemptTime == 0)
          {
            DEBUG_MW_F("Attempting to connect to SSID: %s\n", ssid);
            WiFi.begin(ssid, passphrase);

            _lastConnectionAttemptTime = millis(); // Start connection attempt timer
          }

          // Check if connection attempt has timed out
          if(millis() - _lastConnectionAttemptTime >= connectTimeoutMs)
          {
            DEBUG_MW_F("Failed to connect to SSID: %s\n", ssid);
            _currentNetworkIndex++;         // Move to the next network in the list
            _lastConnectionAttemptTime = 0; // Reset connection attempt timer
          }
        }
        else
        {
          DEBUG_MW_LN("Failed to connect to any known network.");

          _currentNetworkIndex       = 0; // Reset network index counter
          _lastConnectionAttemptTime = 0; // Reset connection attempt timer

          state = LoopState::SCAN_NETWORKS; // Retry scanning after failing all networks
        }
      }
      else
      {
        DEBUG_MW_LN("Connected to SSID: " + WiFi.SSID());
        DEBUG_MW("IP Address: ");
        DEBUG_PRINT_LN(WiFi.localIP());

        _currentNetworkIndex       = 0; // Reset network index counter
        _lastConnectionAttemptTime = 0; // Reset connection attempt timer

        state = LoopState::CHECK_CONNECTION;
      }
      break;
    }
  }
}

/*---------------- Privet Functions ----------------*/

/**
  Checks if the length of the given string is valid.
  
  @param data: The string to be checked.
  @param maxLength: The maximum allowed length for the string.

  @return True if the length of the string is less than or equal to maxLength, otherwise false.
*/
bool MultiWiFi::isValidLength(const char *data, size_t maxLength) { return (strlen(data) <= maxLength); }

/**
  - Handles the completion of WiFi scanning process.
  
  - This function is responsible for finding known WiFi networks, sorting them based on signal strength,
    and adding their indices to the _apListIndex vector.
  
  - It also ensures that the networks not detected during the scan are added to the end of _apListIndex.

  @param networksFound The number of WiFi networks found during the scan.

  @return The number of known WiFi networks found during the scan.
 */
uint8_t MultiWiFi::scanCompleted(uint8_t networksFound)
{
  int8_t        i = 0, j = 0;
  const uint8_t apListSize = _apList.size();
  uint8_t       knownNetworks[apListSize];
  uint8_t       knownNetworksCnt = 0;
  /*-------------------- Find known WiFi networks --------------------*/
  for(i = 0; i < networksFound; i++)
  {
    // Check if the WiFi network contains an entry in AP list
    for(auto entry: _apList)
    {
      // Check SSID
      if(strcmp(WiFi.SSID(i).c_str(), entry.ssid) == 0)
      {
        // Known network
        knownNetworks[knownNetworksCnt++] = i;
      }
    }
  }

  /*-------------------- Sort known WiFi networks by RSSI --------------------*/
  for(i = 0; i < knownNetworksCnt; i++)
  {
    for(j = 0; j < (knownNetworksCnt - 1) - i; j++)
    {
      if(WiFi.RSSI(knownNetworks[j]) < WiFi.RSSI(knownNetworks[j + 1]))
      {
        // Swap indices
        int8_t tmp           = knownNetworks[j];
        knownNetworks[j]     = knownNetworks[j + 1];
        knownNetworks[j + 1] = tmp;
      }
    }
  }

  /*-------------------- Add Sorted networks index to _apListIndex --------------------*/
  _apListIndex.clear(); // Clear the vector before adding new item
  _apListIndex.reserve(apListSize);
  for(i = 0; i < knownNetworksCnt; i++)
  {
    // Check if ssid of known networks is equal to ssid of apList
    for(j = 0; j < apListSize; j++)
    {
      // Check valid SSID
      if(strcmp(WiFi.SSID(knownNetworks[i]).c_str(), _apList.at(j).ssid) == 0)
      {
        /* Index of SSIDs from the strongest signal to the lowest.
         * Increase the correct value (j) by one unit (j + 1) to use in the step "Adding networks not found in the scan".
         */
        _apListIndex.push_back(j + 1);
        break;
      }
    }
  }

  /*-------------------- Add not detected networks to end of _apListIndex --------------------*/
  if(_apListIndex.size() < apListSize)
  {
    for(i = 1; i <= apListSize; i++)
    {
      bool written = false;
      for(j = 0; static_cast<uint8_t>(j) < _apListIndex.size(); j++)
      {
        if(_apListIndex.at(j) == i)
        {
          written = true;
          break;
        }
      }
      if(!written && _apListIndex.size() < apListSize)
      {
        // Adding networks not detected during the scan to the end of the list
        _apListIndex.push_back(i);
      }
    }
  }

  // Return array values to correct value
  for(i = 0; i < apListSize; i++) { _apListIndex.at(i)--; }

  // Return known networks count
  return knownNetworksCnt;
}

/*-----------------------------------------------------------------------------*/
