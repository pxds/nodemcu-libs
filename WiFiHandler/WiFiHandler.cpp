#include "WiFiHandler.h"
#include <ESP8266WiFi.h>

WiFiHandler::WiFiHandler()
{
	this->connectTryoutMax = 10;
	this->OTAenabled = false;
}

WiFiHandler::~WiFiHandler()
{
	WiFi.disconnect();
}

bool WiFiHandler::Connect(const char* ssid, const char* password)
{
	this->ssid = ssid;
	this->password = password;

	Serial.println("");
	Serial.println("Starting WiFi client");
	WiFi.mode(WIFI_STA);
	int connectTries = 0;

	while (WiFi.status()!= WL_CONNECTED) 
	{
		delay(2000);
		// only do WiFi.begin if not already connected
		if (connectTries == 0) {
			WiFi.begin(ssid, password); 
		}
		connectTries++;
		if(connectTries > 1){
			Serial.println("Retrying connection..");
		}
		if (connectTries > this->connectTryoutMax) 
		{
			Serial.print("Connection failed after ");
			Serial.print(this->connectTryoutMax);
			Serial.println("! Rebooting ESP...");
			delay(500);
			ESP.restart();
		}
	}
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	return true;
}

void WiFiHandler::StaticIPConfig(const char* ip, const char* gateway, const char* subnet)
{
	this->staticIP.fromString(ip);
	this->gateway.fromString(gateway);
	this->subnet.fromString(subnet);
	
	IPAddress googleDNS(8, 8, 8, 8);

 	WiFi.config(this->staticIP, googleDNS, this->gateway, this->subnet);

	Serial.println("Static IP configuration: ");
	Serial.print("IP: ");
	Serial.println(this->staticIP);
	Serial.print("Gateway: ");
	Serial.println(this->gateway);
	Serial.print("SubnetMask ");
	Serial.println(this->subnet);
	return;
}

IPAddress WiFiHandler::GetIP()
{
	return WiFi.localIP();
}

IPAddress WiFiHandler::GetSubnetMask()
{
	return WiFi.subnetMask();
}

IPAddress WiFiHandler::GetGatewayIP()
{
	return WiFi.gatewayIP();
}

void WiFiHandler::EnableOTA()
{
	this->OTAenabled = true;
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
	Serial.println("OTA enabled");
	return;
}

void WiFiHandler::OTAsetPort(int port)
{
	ArduinoOTA.setPort(port);
}

void WiFiHandler::OTAsetHostName(const char* hostName)
{
	ArduinoOTA.setHostname(hostName);
}

void WiFiHandler::OTAsetPassword(const char* password)
{
	ArduinoOTA.setPassword(password);
}

void WiFiHandler::OTAbootloader()
{
	ArduinoOTA.handle();
}
