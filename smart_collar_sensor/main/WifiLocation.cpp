//
//
//

#include "WifiLocation.h"

const char* googleApisHost = "www.googleapis.com";
const char* googleApiUrl = "/geolocation/v1/geolocate";

#ifdef ARDUINO_ARCH_ESP32
const char* googleCA = "-----BEGIN CERTIFICATE-----\n" \
"MIIESjCCAzKgAwIBAgINAeO0mqGNiqmBJWlQuDANBgkqhkiG9w0BAQsFADBMMSAw\n" \
"HgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEGA1UEChMKR2xvYmFs\n" \
"U2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjAeFw0xNzA2MTUwMDAwNDJaFw0yMTEy\n" \
"MTUwMDAwNDJaMEIxCzAJBgNVBAYTAlVTMR4wHAYDVQQKExVHb29nbGUgVHJ1c3Qg\n" \
"U2VydmljZXMxEzARBgNVBAMTCkdUUyBDQSAxTzEwggEiMA0GCSqGSIb3DQEBAQUA\n" \
"A4IBDwAwggEKAoIBAQDQGM9F1IvN05zkQO9+tN1pIRvJzzyOTHW5DzEZhD2ePCnv\n" \
"UA0Qk28FgICfKqC9EksC4T2fWBYk/jCfC3R3VZMdS/dN4ZKCEPZRrAzDsiKUDzRr\n" \
"mBBJ5wudgzndIMYcLe/RGGFl5yODIKgjEv/SJH/UL+dEaltN11BmsK+eQmMF++Ac\n" \
"xGNhr59qM/9il71I2dN8FGfcddwuaej4bXhp0LcQBbjxMcI7JP0aM3T4I+DsaxmK\n" \
"FsbjzaTNC9uzpFlgOIg7rR25xoynUxv8vNmkq7zdPGHXkxWY7oG9j+JkRyBABk7X\n" \
"rJfoucBZEqFJJSPk7XA0LKW0Y3z5oz2D0c1tJKwHAgMBAAGjggEzMIIBLzAOBgNV\n" \
"HQ8BAf8EBAMCAYYwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIGA1Ud\n" \
"EwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFJjR+G4Q68+b7GCfGJAboOt9Cf0rMB8G\n" \
"A1UdIwQYMBaAFJviB1dnHB7AagbeWbSaLd/cGYYuMDUGCCsGAQUFBwEBBCkwJzAl\n" \
"BggrBgEFBQcwAYYZaHR0cDovL29jc3AucGtpLmdvb2cvZ3NyMjAyBgNVHR8EKzAp\n" \
"MCegJaAjhiFodHRwOi8vY3JsLnBraS5nb29nL2dzcjIvZ3NyMi5jcmwwPwYDVR0g\n" \
"BDgwNjA0BgZngQwBAgIwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly9wa2kuZ29vZy9y\n" \
"ZXBvc2l0b3J5LzANBgkqhkiG9w0BAQsFAAOCAQEAGoA+Nnn78y6pRjd9XlQWNa7H\n" \
"TgiZ/r3RNGkmUmYHPQq6Scti9PEajvwRT2iWTHQr02fesqOqBY2ETUwgZQ+lltoN\n" \
"FvhsO9tvBCOIazpswWC9aJ9xju4tWDQH8NVU6YZZ/XteDSGU9YzJqPjY8q3MDxrz\n" \
"mqepBCf5o8mw/wJ4a2G6xzUr6Fb6T8McDO22PLRL6u3M4Tzs3A2M1j6bykJYi8wW\n" \
"IRdAvKLWZu/axBVbzYmqmwkm5zLSDW5nIAJbELCQCZwMH56t2Dvqofxs6BBcCFIZ\n" \
"USpxu6x6td0V7SvJCCosirSmIatj/9dSSVDQibet8q/7UK4v4ZUN80atnZz1yg==\n" \
"-----END CERTIFICATE-----\n";
#endif

String WifiLocation::MACtoString(uint8_t* macAddress) {
    uint8_t mac[6];
    char macStr[18] = { 0 };
#ifdef ARDUINO_ARCH_SAMD
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[5], macAddress[4], macAddress[3], macAddress[2], macAddress[1], macAddress[0]);
#elif defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
#endif
    return  String(macStr);
}

WifiLocation::WifiLocation(String googleKey) {
    _googleApiKey = googleKey;
}

// Function to get a list of surrounding WiFi signals in JSON format to get location via Google Location API
String WifiLocation::getSurroundingWiFiJson() {
    String wifiArray = "[\n";

    int8_t numWifi = WiFi.scanNetworks();
#ifdef DEBUG_WIFI_LOCATION
    Serial.println(String(numWifi) + " WiFi networks found");
#endif // DEBUG_WIFI_LOCATION
    for (uint8_t i = 0; i < numWifi; i++) {//numWifi; i++) {
      //Serial.print("WiFi.BSSID(i) = ");
      //Serial.println((char *)WiFi.BSSID(i));
#ifdef ARDUINO_ARCH_SAMD
        byte bssid[6];

        wifiArray += "{\"macAddress\":\"" + MACtoString(WiFi.BSSID(i,bssid)) + "\",";
#elif defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
        wifiArray += "{\"macAddress\":\"" + MACtoString(WiFi.BSSID(i)) + "\",";
#else
#error Only ESP8266 and SAMD platforms are supported
#endif
        wifiArray += "\"signalStrength\":" + String(WiFi.RSSI(i)) + ",";
        wifiArray += "\"channel\":" + String(WiFi.channel(i)) + "}";
        if (i < (numWifi - 1)) {
            wifiArray += ",\n";
        }
    }
#if defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
    WiFi.scanDelete();
#endif
    wifiArray += "]";
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("WiFi list :\n" + wifiArray);
#endif // DEBUG_WIFI_LOCATION
    return wifiArray;
}

// Calls Google Location API to get current location using surrounding WiFi signals inf
location_t WifiLocation::getGeoFromWiFi() {

    location_t location;
    String response = "";
#ifdef ARDUINO_ARCH_ESP32
    _client.setCACert(googleCA);
#endif
    if (_client.connect(googleApisHost, 443)) {
#ifdef DEBUG_WIFI_LOCATION
        Serial.println("Connected to API endpoint");
#endif // DEBUG_WIFI_LOCATION
    } else {
#ifdef DEBUG_WIFI_LOCATION
        Serial.println("HTTPS error");
#endif // DEBUG_WIFI_LOCATION
        return location;
    }

    String body = "{\"wifiAccessPoints\":" + getSurroundingWiFiJson() + "}";
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("requesting URL: " + String(googleApiUrl) + "?key=" + _googleApiKey);
#endif // DEBUG_WIFI_LOCATION
    String request = String("POST ") + String(googleApiUrl);
    if (_googleApiKey != "")
        request += "?key=" + _googleApiKey;
    request += " HTTP/1.1\r\n";
    request += "Host: " + String(googleApisHost) + "\r\n";
    request += "User-Agent: ESP8266\r\n";
    request += "Content-Type:application/json\r\n";
    request += "Content-Length:" + String(body.length()) + "\r\n";
    request += "Connection: keep-alive\r\n\r\n";
    request += body;
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("request: \n" + request);
#endif // DEBUG_WIFI_LOCATION

    _client.println(request);
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("request sent");
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
#endif // DEBUG_WIFI_LOCATION
    int timer = millis();

    // Wait for response
    while (millis() - timer < MAX_CONNECTION_TIMEOUT) {
        if (_client.available())
            break;
        yield ();
    }
    while (_client.available()) {
#ifdef DEBUG_WIFI_LOCATION
        Serial.print(".");
#endif // DEBUG_WIFI_LOCATION
        response += _client.readString();
        yield ();
    }
#ifdef DEBUG_WIFI_LOCATION
    Serial.println();
#endif // DEBUG_WIFI_LOCATION
    if (response != "") {
#ifdef DEBUG_WIFI_LOCATION
        Serial.println("Got response:");
        Serial.println(response);
#endif // DEBUG_WIFI_LOCATION

        int index = response.indexOf("\"lat\":");
        if (index != -1) {
            String tempStr = response.substring(index);
            //Serial.println(tempStr);
            tempStr = tempStr.substring(tempStr.indexOf(":") + 1, tempStr.indexOf(","));
            //Serial.println(tempStr);
            location.lat = tempStr.toFloat();
#ifdef DEBUG_WIFI_LOCATION
            Serial.println("\nLat: " + String(location.lat, 7));
#endif // DEBUG_WIFI_LOCATION
        }

        index = response.indexOf("\"lng\":");
        if (index != -1) {
            String tempStr = response.substring(index);
            //Serial.println(tempStr);
            tempStr = tempStr.substring(tempStr.indexOf(":") + 1, tempStr.indexOf(","));
            //Serial.println(tempStr);
            location.lon = tempStr.toFloat();
#ifdef DEBUG_WIFI_LOCATION
            Serial.println("Lon: " + String(location.lon, 7));
#endif // DEBUG_WIFI_LOCATION
        }

        index = response.indexOf("\"accuracy\":");
        if (index != -1) {
            String tempStr = response.substring(index);
            //Serial.println(tempStr);
            tempStr = tempStr.substring(tempStr.indexOf(":") + 1, tempStr.indexOf("\n"));
            //Serial.println(tempStr);
            location.accuracy = tempStr.toFloat();
#ifdef DEBUG_WIFI_LOCATION
            Serial.println("Accuracy: " + String(location.accuracy) + "\n");
#endif // DEBUG_WIFI_LOCATION
        }
    }

    return location;
}
