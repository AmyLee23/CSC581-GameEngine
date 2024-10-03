#include "pch.h"
#include "JClient.h"
#include <iostream>
#include <zmq.hpp>
#include <string>
#include <unordered_map>

// Constructor for the JClient class
JClient::JClient(const std::string& server_ip, int port_req_rep)
    : server_ip(server_ip), port_req_rep(port_req_rep),
    context(1), req_socket(context, ZMQ_REQ), action_socket(nullptr) {

    // Connect the REQ-REP socket to request player number and port for further communication
    req_socket.connect("tcp://" + server_ip + ":" + std::to_string(port_req_rep));
}

// Destructor for the JClient class
JClient::~JClient() {
    if (action_socket) {
        delete action_socket;
    }
}

// Function to connect to the server and ask for the player number and action socket port
void JClient::connectToServer() {
    // Send a request to get the player number and port
    zmq::message_t request(5);
    memcpy(request.data(), "READY", 5);
    req_socket.send(request, zmq::send_flags::none);

    // Receive the reply from the server with the player number and port
    zmq::message_t reply;
    req_socket.recv(reply, zmq::recv_flags::none);

    // Parse the reply to get the player number and port
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    std::istringstream iss(reply_str);
    std::string player_num, port_str;
    iss >> player_num >> port_str;

    player_name = "Player" + player_num;
    int action_port = std::stoi(port_str);

    std::cout << "Connected to the server as " << player_name << " on port " << action_port << std::endl;

    // Create a new REP socket for sending actions and receiving validation (true/false)
    action_socket = new zmq::socket_t(context, ZMQ_REQ);
    action_socket->connect("tcp://" + server_ip + ":" + std::to_string(action_port));
}

std::unordered_map<std::string, std::pair<double, double>> JClient::deserializeMovingSprites(std::string game_state_message) {
    std::unordered_map<std::string, std::pair<double, double>> movingSprites;
    // Split the game state update by semicolons
    std::istringstream iss(game_state_message);
    std::string sprite_info;

    // Process each sprite's info
    while (std::getline(iss, sprite_info, ';')) {
        if (sprite_info.empty()) continue;  // Skip any empty entries

        std::istringstream sprite_stream(sprite_info);
        std::string name;
        double x, y;

        // Parse the name, x, and y values from each sprite entry
        sprite_stream >> name >> x >> y;

        if (!name.empty()) {  // Make sure we have a valid name
            movingSprites[name] = { x, y };  // Store the updated position in the map
        }
    }
    return movingSprites;
}

// Function to send player input (REQ-REP) to the server and wait for validation
std::unordered_map<std::string, std::pair<double, double>> JClient::sendInput(const std::string& name, const std::pair<double, double>& position) {
    if (!action_socket) {
        std::cerr << "Action socket is not initialized!" << std::endl;
        return std::unordered_map<std::string, std::pair<double, double>>{};
    }

    // Serialize the player's input (player name and position)
    std::string message = name + " " + std::to_string(position.first) + " " + std::to_string(position.second);

    // Send the serialized message to the server
    zmq::message_t input_msg(message.size());
    memcpy(input_msg.data(), message.c_str(), message.size());
    action_socket->send(input_msg, zmq::send_flags::none);

    std::cout << "Sent player input to the server: " << message << std::endl;

    // Wait for the server's reply
    zmq::message_t reply_msg;
    action_socket->recv(reply_msg, zmq::recv_flags::none);
    std::string reply(static_cast<char*>(reply_msg.data()), reply_msg.size());

    std::cout << "Received game state update from the server: " << reply << std::endl;

    // return reply
    std::unordered_map<std::string, std::pair<double, double>> movingSprites = deserializeMovingSprites(reply);
    return movingSprites;
}

// Getter function to retrieve the player's name
std::string JClient::getPlayerName() const {
    return player_name;
}



/*
// Constructor for the JClient class
JClient::JClient(const std::string& server_ip, int port_pub_sub, int port_req_rep)
    : server_ip(server_ip), port_pub_sub(port_pub_sub), port_req_rep(port_req_rep),
    context(1), sub_socket(context, ZMQ_SUB), req_socket(context, ZMQ_REQ), action_socket(nullptr) {

    // Connect the PUB-SUB socket to receive game state updates
    sub_socket.connect("tcp://" + server_ip + ":" + std::to_string(port_pub_sub));
    sub_socket.set(zmq::sockopt::subscribe, "");  // Subscribe to all messages for PUB-SUB updates

    // Connect the REQ-REP socket to request player number and port for further communication
    req_socket.connect("tcp://" + server_ip + ":" + std::to_string(port_req_rep));
}

// Destructor for the JClient class
JClient::~JClient() {
    if (action_socket) {
        delete action_socket;
    }
}

// Function to connect to the server and ask for the player number and action socket port
void JClient::connectToServer() {
    // Send a request to get the player number and port
    zmq::message_t request(5);
    memcpy(request.data(), "READY", 5);
    req_socket.send(request, zmq::send_flags::none);

    // Receive the reply from the server with the player number and port
    zmq::message_t reply;
    req_socket.recv(reply, zmq::recv_flags::none);

    // Parse the reply to get the player number and port
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    std::istringstream iss(reply_str);
    std::string player_num, port_str;
    iss >> player_num >> port_str;

    player_name = "Player" + player_num;
    int action_port = std::stoi(port_str);

    std::cout << "Connected to the server as " << player_name << " on port " << action_port << std::endl;

    // Create a new REP socket for sending actions and receiving validation (true/false)
    action_socket = new zmq::socket_t(context, ZMQ_REQ);
    action_socket->connect("tcp://" + server_ip + ":" + std::to_string(action_port));
}

// Function to receive the game state from the server (PUB-SUB)
void JClient::receiveGameState(std::unordered_map<std::string, std::pair<double, double>>& movingSprites) {
    zmq::message_t update_msg;
    if (sub_socket.recv(update_msg, zmq::recv_flags::none)) {
        std::string game_state_update(static_cast<char*>(update_msg.data()), update_msg.size());

        // Split the game state update by semicolons
        std::istringstream iss(game_state_update);
        std::string sprite_info;

        // Process each sprite's info
        while (std::getline(iss, sprite_info, ';')) {
            if (sprite_info.empty()) continue;  // Skip any empty entries

            std::istringstream sprite_stream(sprite_info);
            std::string name;
            double x, y;

            // Parse the name, x, and y values from each sprite entry
            sprite_stream >> name >> x >> y;

            if (!name.empty()) {  // Make sure we have a valid name
                movingSprites[name] = { x, y };  // Store the updated position in the map
            }
        }

        std::cout << "Received game state update from the server: " << game_state_update << std::endl;
    }
}

// Function to send player input (REQ-REP) to the server and wait for validation
bool JClient::sendInput(const std::string& name, const std::pair<double, double>& position) {
    if (!action_socket) {
        std::cerr << "Action socket is not initialized!" << std::endl;
        return false;
    }

    // Serialize the player's input (player name and position)
    std::string message = name + " " + std::to_string(position.first) + " " + std::to_string(position.second);

    // Send the serialized message to the server
    zmq::message_t input_msg(message.size());
    memcpy(input_msg.data(), message.c_str(), message.size());
    action_socket->send(input_msg, zmq::send_flags::none);

    std::cout << "Sent player input to the server: " << message << std::endl;

    // Wait for the server's reply (true or false)
    zmq::message_t reply_msg;
    action_socket->recv(reply_msg, zmq::recv_flags::none);
    std::string reply(static_cast<char*>(reply_msg.data()), reply_msg.size());

    // return reply
    return (reply == "true");
}

// Getter function to retrieve the player's name
std::string JClient::getPlayerName() const {
    return player_name;
} */

////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
// Constructor for the JClient class
JClient::JClient(const std::string& server_ip, int port_push_pull, int port_pub_sub, int port_req_rep)
    : server_ip(server_ip), port_push_pull(port_push_pull), port_pub_sub(port_pub_sub), port_req_rep(port_req_rep),
    context(1), push_socket(context, ZMQ_PUSH), sub_socket(context, ZMQ_SUB), req_socket(context, ZMQ_REQ) {

    // Connect the sockets to their respective server ports
    push_socket.connect("tcp://" + server_ip + ":" + std::to_string(port_push_pull));
    sub_socket.connect("tcp://" + server_ip + ":" + std::to_string(port_pub_sub));
    req_socket.connect("tcp://" + server_ip + ":" + std::to_string(port_req_rep));

    // Subscribe to all messages for PUB-SUB updates
    sub_socket.set(zmq::sockopt::subscribe, "");
}

// Destructor for the JClient class
JClient::~JClient() {}

// Function to connect to the server and ask for the player number
void JClient::connectToServer() {
    // Send a request to get the number of connected clients (player number)
    zmq::message_t request(5);
    memcpy(request.data(), "READY", 5);
    req_socket.send(request, zmq::send_flags::none);

    // Receive the reply from the server with the player number
    zmq::message_t reply;
    req_socket.recv(reply, zmq::recv_flags::none);

    // Set the player name based on the number of connected clients (PlayerX)
    std::string player_num(static_cast<char*>(reply.data()), reply.size());
    player_name = "Player" + player_num;

    std::cout << "Connected to the server as " << player_name << std::endl;
}

// Function to receive the game state from the server (PUB-SUB)
void JClient::receiveGameState(std::unordered_map<std::string, std::pair<double, double>>& movingSprites) {
    zmq::message_t update_msg;
    if (sub_socket.recv(update_msg, zmq::recv_flags::none)) {
        std::string game_state_update(static_cast<char*>(update_msg.data()), update_msg.size());

        // Split the game state update by semicolons
        std::istringstream iss(game_state_update);
        std::string sprite_info;

        // Process each sprite's info
        while (std::getline(iss, sprite_info, ';')) {
            if (sprite_info.empty()) continue;  // Skip any empty entries

            std::istringstream sprite_stream(sprite_info);
            std::string name;
            double x, y;

            // Parse the name, x, and y values from each sprite entry
            sprite_stream >> name >> x >> y;

            if (!name.empty()) {  // Make sure we have a valid name
                movingSprites[name] = { x, y };  // Store the updated position in the map
            }
        }

        std::cout << "Received game state update from the server: " << game_state_update << std::endl;
    }
}


// Function to send player input (PUSH-PULL) to the server
void JClient::sendInput(const std::string& name, const std::pair<double, double>& position) {
    // Serialize the player's input (player name and position)
    std::string message = name + " " + std::to_string(position.first) + " " + std::to_string(position.second);

    // Send the serialized message to the server
    zmq::message_t input_msg(message.size());
    memcpy(input_msg.data(), message.c_str(), message.size());
    push_socket.send(input_msg, zmq::send_flags::none);

    std::cout << "Sent player input to the server: " << message << std::endl;
}

// Getter function to retrieve the player's name
std::string JClient::getPlayerName() const {
    return player_name;
}
*/