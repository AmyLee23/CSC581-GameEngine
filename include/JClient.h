#pragma once

#include <string>
#include <unordered_map>
#include <zmq.hpp>

class JClient {
public:
    JClient(const std::string& server_ip, int port_req_rep);
    ~JClient();

    // Connects to the server and retrieves the player number and action port
    void connectToServer();

    // Sends player input (REQ-REP) to the server and waits for validation
    std::unordered_map<std::string, std::pair<double, double>> sendInput(const std::string& name, const std::pair<double, double>& position);

    // Getter function to retrieve the player's name
    std::string getPlayerName() const;

private:
    std::string server_ip;
    int port_req_rep;
    std::string player_name;

    zmq::context_t context;
    zmq::socket_t req_socket;
    zmq::socket_t* action_socket;

    std::unordered_map<std::string, std::pair<double, double>> deserializeMovingSprites(std::string game_state_message);
};

/*
class JClient {
public:
    JClient(const std::string& server_ip, int port_pub_sub, int port_req_rep);
    ~JClient();

    // Connects to the server and retrieves the player number and action port
    void connectToServer();

    // Receives the game state from the server (PUB-SUB)
    void receiveGameState(std::unordered_map<std::string, std::pair<double, double>>& movingSprites);

    // Sends player input (REQ-REP) to the server and waits for validation
    bool sendInput(const std::string& name, const std::pair<double, double>& position);

    // Getter function to retrieve the player's name
    std::string getPlayerName() const;

private:
    std::string server_ip;
    int port_pub_sub;
    int port_req_rep;
    std::string player_name;

    zmq::context_t context;
    zmq::socket_t sub_socket;
    zmq::socket_t req_socket;
    zmq::socket_t* action_socket;
}; */

//////////////////////////////////////////////////////////////////////////////

/*
class JClient {
public:
    // Constructor for the Client class
    JClient(const std::string& server_ip, int port_push_pull, int port_pub_sub, int port_req_rep);

    // Destructor for the Client class
    ~JClient();

    // Function to connect to the server and ask for the player number
    void connectToServer();

    // Function to receive the game state from the server (PUB-SUB)
    void receiveGameState(std::unordered_map<std::string, std::pair<double, double>>& movingShapes);

    // Function to send player input (PUSH-PULL) to the server
    void sendInput(const std::string& name, const std::pair<double, double>& position);

    // Getter function to retrieve the player's name
    std::string getPlayerName() const;

private:
    // Server details
    std::string server_ip;
    int port_push_pull;
    int port_pub_sub;
    int port_req_rep;

    // Player information
    std::string player_name;

    // ZeroMQ context and sockets
    zmq::context_t context;
    zmq::socket_t push_socket;
    zmq::socket_t sub_socket;
    zmq::socket_t req_socket;
}; 
*/