#include "MQTTHandler.h"

MQTTHandler::MQTTHandler()
{
	// TODO:
	// If Deviot/PlatformIO support C++11, initialize values 
	// of shared members variables here and call MQTTHandle()
	// on the other constructors
}

MQTTHandler::MQTTHandler(const char* mqttServer)
{
	this->connectTryoutMax = 5;
	this->numTopics = 0;
	this->mqttServer = mqttServer;
	this->MQTTclient.setClient(this->WifiClient);
	this->port = 1883; // default public port
	this->hasAuth = false;

	MQTTclient.setServer(this->mqttServer, this->port);
}

MQTTHandler::MQTTHandler(const char* mqttServer, int port)
{
	this->connectTryoutMax = 5;
	this->numTopics = 0;
	this->mqttServer = mqttServer;
	this->MQTTclient.setClient(this->WifiClient);
	this->port = port; // default public port

	MQTTclient.setServer(this->mqttServer, this->port);
}

MQTTHandler::MQTTHandler(const char* mqttServer, int port, 
		        const char* user, const char* password)
{
	this->connectTryoutMax = 5;
	this->numTopics = 0;
	this->hasAuth = true;
	this->mqttServer = mqttServer;
	this->MQTTclient.setClient(this->WifiClient);
	this->port = port; // default public port

	this->password = password;
	this->user = user;

	MQTTclient.setServer(this->mqttServer, this->port);
}

MQTTHandler::~MQTTHandler()
{
	this->MQTTclient.disconnect();
}

void MQTTHandler::SetClient(WiFiClient WifiClient)
{
	this->WifiClient = WifiClient;
	this->MQTTclient.setClient(this->WifiClient);
}

bool MQTTHandler::KeepAlive()
{
	if (!MQTTclient.connected()) {
		if (!Reconnect())
			return false;
	}

	return MQTTclient.loop();
	// TODO: check whats up with loop() thats causing disconnection
	// return MQTTclient.loop();
}

bool MQTTHandler::Reconnect()
{
	int connectTries = 0;

	while (!this->MQTTclient.connected()) 
	{
		Serial.println("Attempting MQTT connection...");

		if (Connect()) 
		{
			return true;
		} 
		else 
		{
			Serial.print("failed, rc=");
			Serial.print(this->MQTTclient.state());
			Serial.println(" try again in 5 seconds");
			connectTries++;
			// Wait 5 seconds before retrying
			delay(5000);
		}
		// TODO: take action if it can't reconnect
		if (connectTries > this->connectTryoutMax) 
		{
			Serial.print("Connection failed after ");
			Serial.print(this->connectTryoutMax);
			Serial.println("! Please reboot system...");

			return false;
		}
	}
}

bool MQTTHandler::Connect()
{
	if(this->MQTTclient.connect(this->clientId))
	{
		Serial.print("Connected to ");
		Serial.print(this->mqttServer);
		Serial.print(":");
		Serial.println(this->port);

		for(int i=0; i<numTopics; i++)
		{
			this->MQTTclient.subscribe(topicList[i]);
		}
		PrintSubscribedTopics();

		return true;
	}
	return false;
}

void MQTTHandler::SetCallback(MQTT_CALLBACK_SIGNATURE)
{
	this->MQTTclient.setCallback(callback);
}

bool MQTTHandler::Connect(const char* clientId)
{
	this->clientId = clientId;

	return Connect();
}

bool MQTTHandler::Publish(const char* topic, const char* msg)
{
	return this->MQTTclient.publish(topic, msg);
}

bool MQTTHandler::Publish(const char* topic, int msg)
{
	char payload[10];
	sprintf(payload, "%d", msg);

	return this->MQTTclient.publish(topic, payload);
}

bool MQTTHandler::Subscribe(const char* topic)
{
	topicList[numTopics] = topic;
	numTopics++;
	if (!MQTTclient.connected())
		if(this->MQTTclient.subscribe(topic))
		{
			Serial.print("Subscribed to ");
			Serial.println(topic);
			return true;
		}
		else
		{
			Serial.print("Could not subscribed to ");
			Serial.print(topic);
			Serial.println(", adding it to topic list.");
			return false;
		}
	else
	{
		Serial.print(topic);
		Serial.println(" added to topic list.");
		return true;
	}
	// TODO: think of a better way to tell the user that the
	// topic was only added to the topic list when the client 
	// is not connected instead of returning 'true'
}

void MQTTHandler::PrintSubscribedTopics()
{
	for(int i=0; i<numTopics; i++){
		Serial.println("Subscribed topics:");
		Serial.print("- ");
		Serial.println(topicList[i]);	    
	}
}