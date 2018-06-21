/*
	Simple WiFi handler to decouple WiFi complexity from main
*/

#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include "IPAddress.h"
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

inline void OTAsetup()
{
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

inline void OTAloop()
{
	ArduinoOTA.handle();
}

class WiFiHandler
{
private:
	IPAddress staticIP;
	IPAddress gateway;
	IPAddress subnet;

	const char* ssid;
	const char* password;

	bool OTAenabled;
	int connectTryoutMax;

public:
	WiFiHandler();
	~WiFiHandler();

	bool Connect(const char* ssid, const char* password);
	void StaticIPConfig(const char* ip, const char* gateway, const char* subnet);
	
	IPAddress GetIP();
	IPAddress GetSubnetMask();
	IPAddress GetGatewayIP();

	void EnableOTA();
	void OTAsetPort(int port);
	void OTAsetHostName(const char* hostName);
	void OTAsetPassword(const char* password);
	void OTAbootloader();
};

#endif