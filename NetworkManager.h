#pragma once
#include<iostream>
#include<zmq.h>
using namespace std;

class NetworkManager {
public:
	NetworkManager(bool isServer);
	~NetworkManager();

	void init(int port);
	
	void cleanup();
	void sendMessage(const string& clientID, const string& coordinates);
	void publishMessage(const string& clientID, const string& coordinates);
	string receiveMessage();
	string receivePubMessage();

private:
	void* context;
	void* socket;
	void* pub_sub_socket;
	bool isServer;
};