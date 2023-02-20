#ifndef WiFi_h
#define WiFi_h

#include <ESP8266WiFi.h>

class WiFiManager {
  private:
    const char* ssid;
    const char* password;

  public:
    WiFiManager(const char* ap_ssid, const char* ap_password) {
      ssid = ap_ssid;
      password = ap_password;
    }

    void begin() {
      Serial.print("Creating access point: ");
      Serial.println(ssid);

      WiFi.softAP(ssid, password);
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(myIP);
    }

    bool connect(const char* network_ssid, const char* network_password) {
      WiFi.begin(network_ssid, network_password);
      Serial.print("Connecting to ");
      Serial.print(network_ssid);

      int i = 0;
      while (WiFi.status() != WL_CONNECTED && i < 20) {
        delay(500);
        Serial.print(".");
        i++;
      }

      Serial.println("");
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected!");
        return true;
      } else {
        Serial.println("Connection failed.");
        return false;
      }
    }
};

#endif
void WiFiLibrary::connectToWifi() {
  Serial.println("Connecting to WiFi");

  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiLibrary::startAccessPoint() {
  Serial.println("Starting access point");

  WiFi.softAP(apName.c_str(), apPassword.c_str());

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void WiFiLibrary::setup() {
  if (shouldStartAccessPoint()) {
    startAccessPoint();
  } else {
    connectToWifi();
  }
}

bool WiFiLibrary::shouldStartAccessPoint() {
  return ssid == "" || password == "";
}
