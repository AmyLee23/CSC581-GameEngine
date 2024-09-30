#include "NetworkManager.h"
#include<iostream>
#include<string>
using namespace std;

NetworkManager::NetworkManager (bool isServer) : isServer(isServer)
{
	context = zmq_ctx_new();
	if (!context) {
		throw std::runtime_error("Failed to create ZeroMQ context");
	}
	socket = zmq_socket(context, isServer ? ZMQ_REP : ZMQ_REQ);
	pub_sub_socket = zmq_socket(context, isServer ? ZMQ_PUB : ZMQ_SUB);
}

void NetworkManager::init(int port) {
	string connectionString = (isServer ? "tcp://*:" : "tcp://localhost:") + to_string(port);
	string pubSubConnectionString = (isServer ? "tcp://*:" : "tcp://localhost:") + to_string(port+1);
	int rc;
	int rc2;
	if (isServer) {
		rc = zmq_bind(socket, connectionString.c_str());
		if (rc != 0) {
			cerr << "Failed to bind server: " << zmq_strerror(zmq_errno()) << endl;
		}
		else {
			cout << "Server bound to " << connectionString << endl;
		}

		rc2 = zmq_bind(pub_sub_socket, pubSubConnectionString.c_str());
		if (rc2 != 0) {
			cerr << "Failed to bind server: " << zmq_strerror(zmq_errno()) << endl;
		}
		else {
			cout << "Server bound to " << pubSubConnectionString << endl;
		}
	}
	else {
		rc = zmq_connect(socket, connectionString.c_str());
		if (rc != 0) {
			cerr << "Failed to connect client: " << zmq_strerror(zmq_errno()) << endl;
		}
		else {
			cout << "Client connected to " << connectionString << endl;
		}

		rc2 = zmq_connect(pub_sub_socket, pubSubConnectionString.c_str());
		zmq_setsockopt(pub_sub_socket, ZMQ_SUBSCRIBE, "", 0);
		if (rc != 0) {
			cerr << "Failed to connect client: " << zmq_strerror(zmq_errno()) << endl;
		}
		else {
			cout << "Client connected to " << pubSubConnectionString << endl;
		}
	}
}

void NetworkManager::cleanup() {
	zmq_close(socket);
	zmq_ctx_destroy(context);
}

void NetworkManager::sendMessage(const string& clientID, const string& coordinates) {
	string message = clientID + " : " + coordinates;
	int res = zmq_send(socket, message.c_str(), message.size(), 0);
	if (res == -1) {
		cerr << "Failed to send message: " << zmq_strerror(zmq_errno()) << endl;
	}
}

void NetworkManager::publishMessage(const string& clientID, const string& coordinates) {
	string message = clientID + " : " + coordinates;
	int res = zmq_send(pub_sub_socket, message.c_str(), message.size(), 0);
	if (res == -1) {
		cerr << "Failed to publish message: " << zmq_strerror(zmq_errno()) << endl;
	}
}

string NetworkManager::receiveMessage() {
	zmq_msg_t msg;
	zmq_msg_init(&msg);
	int result = zmq_msg_recv(&msg, socket, ZMQ_DONTWAIT);
	if (result == -1) {
		if (zmq_errno() == EAGAIN) {
			// No message available, not an error
			zmq_msg_close(&msg);
			return "";
		}
	}
	// Message received successfully
	string recvStr(static_cast<char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg));
	zmq_msg_close(&msg);
	return recvStr;

}

string NetworkManager::receivePubMessage() {
	zmq_msg_t msg;
	zmq_msg_init(&msg);
	int result = zmq_msg_recv(&msg, pub_sub_socket, ZMQ_DONTWAIT);
	if (result == -1) {
		zmq_msg_close(&msg);
		return "";
	}
	string recvStr(static_cast<char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg));
	zmq_msg_close(&msg);
	return recvStr;
}


NetworkManager::~NetworkManager() {
	cleanup();
}