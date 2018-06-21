/* 
TODO:
 Right now, this is using 'WiFiClient' class, so it only works
with WiFiClients
*/
#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MAX_TOPICS 30

class MQTTHandler
{
private:
	const char* clientId;

	const char* mqttServer;

	int port; 
	const char* user;
	const char* password;
	bool hasAuth;
	
	int connectTryoutMax;

	int numTopics;

	const char* topicList[MAX_TOPICS];

	PubSubClient MQTTclient;
	WiFiClient WifiClient;

	MQTT_CALLBACK_SIGNATURE;

	bool Connect();
	bool Reconnect();
public:
	MQTTHandler();
	MQTTHandler(const char* mqttServer);
	MQTTHandler(const char* mqttServer, int port);
	MQTTHandler(const char* mqttServer, int port, 
		        const char* user, const char* password);
	~MQTTHandler();

	void SetClient(WiFiClient WifiClient);

	bool KeepAlive();

	void SetCallback(MQTT_CALLBACK_SIGNATURE);

	bool Connect(const char* clientId);

	bool Publish(const char* topic, const char* msg);
	bool Publish(const char* topic, int msg);

	bool Subscribe(const char* topic);
	void PrintSubscribedTopics();
};

#endif