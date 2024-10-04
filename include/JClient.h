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

    // Sends player input (REQ-REP) to the server and waits for latest game state
    std::unordered_map<std::string, std::pair<double, double>> sendInput(const std::string& name, const std::pair<double, double>& position);

    // Retrieve the player's name
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
