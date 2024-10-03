#pragma once

#include <zmq.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include "GameState.h"

class JServer {
public:
    JServer(int port_req_rep);
    ~JServer();

    // Handles player number requests (REQ-REP communication)
    void handlePlayerNumberRequests(GameState& gameState);

    // Adds a new client with a unique REP socket for each client
    void addClient(GameState& gameState, int clientPort);

    // Handles receiving client input (via the dedicated REP socket per client)
    void handleClientInput(GameState& gameState, zmq::socket_t* client_socket);

    // Returns the number of connected clients
    int getNumClients() const;

private:
    int port_req_rep;
    int num_clients;

    zmq::context_t context;
    zmq::socket_t rep_socket;

    std::vector<std::thread> client_threads;
    std::mutex client_mutex;

    std::unordered_map<std::string, std::pair<double, double>> getMovingSprites(GameState& gameState);
    std::string serializeMovingSprites(const std::unordered_map<std::string, std::pair<double, double>>& movingSprites);
};

/*
class JServer {
public:
    JServer(int port_pub_sub, int port_req_rep);
    ~JServer();

    // Handles player number requests (REQ-REP communication)
    void handlePlayerNumberRequests(GameState& gameState);

    // Adds a new client with a unique REP socket for each client
    void addClient(GameState& gameState, int clientPort);

    // Handles receiving client input (via the dedicated REP socket per client)
    void handleClientInput(GameState& gameState, zmq::socket_t* client_socket);

    // Broadcasts the current game state to all clients (via PUB-SUB communication)
    void broadcastGameState(const std::unordered_map<std::string, std::pair<double, double>>& movingSprites);

    // Returns the number of connected clients
    int getNumClients() const;

private:
    int port_pub_sub;
    int port_req_rep;
    int num_clients;

    zmq::context_t context;
    zmq::socket_t pub_socket;
    zmq::socket_t rep_socket;

    std::vector<std::thread> client_threads;
    std::mutex client_mutex;
};
*/

/////////////////////////////////////////////////////////////////////////////////////////

/*
class JServer {
public:
    // Constructor: Initializes the server with PUSH-PULL, PUB-SUB, and REQ-REP ports
    JServer(int port_push_pull, int port_pub_sub, int port_req_rep);

    // Destructor: Ensures that all client threads are joined upon cleanup
    ~JServer();

    // Adds a new client and increments the client count
    void addClient(GameState& gameState);

    // Handles player number requests (REQ-REP communication)
    void handlePlayerNumberRequests(GameState& gameState);

    // Handles receiving client input (via PUSH-PULL) and modifies the game state
    void handleClientInput(GameState& gameState);

    // Broadcasts the current game state to all clients (via PUB-SUB communication)
    void broadcastGameState(const std::unordered_map<std::string, std::pair<double, double>>& movingShapes);

    // Returns the number of connected clients
    int getNumClients() const;

private:
    int port_push_pull;
    int port_pub_sub;
    int port_req_rep;
    int num_clients;

    // ZeroMQ context and sockets
    zmq::context_t context;
    zmq::socket_t pull_socket;  // For receiving client inputs (PUSH-PULL)
    zmq::socket_t pub_socket;   // For broadcasting game state (PUB-SUB)
    zmq::socket_t rep_socket;   // For responding to client player number requests (REQ-REP)

    // Thread handling for clients
    std::vector<std::thread> client_threads;
    std::mutex client_mutex;
};
*/