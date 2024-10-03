#include "pch.h"
#include "JServer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <zmq.hpp>
#include "PhysicsEngine.h"
#include "GameWorld.h"

// Constructor for the JServer class
JServer::JServer(int port_req_rep)
    : port_req_rep(port_req_rep), num_clients(0), context(1), rep_socket(context, ZMQ_REP) {

    // Bind the REQ-REP socket for client player number requests
    rep_socket.bind("tcp://*:" + std::to_string(port_req_rep));
}

// Destructor for the JServer class
JServer::~JServer() {
    for (auto& th : client_threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}

// Handles player number requests (REQ-REP communication)
void JServer::handlePlayerNumberRequests(GameState& gameState) {
    zmq::message_t request_msg;
    if (rep_socket.recv(request_msg, zmq::recv_flags::dontwait)) {
        std::string request(static_cast<char*>(request_msg.data()), request_msg.size());
        std::cout << "Received request: " << request << std::endl;

        // Respond with the current number of clients + port for this client
        int playerNumber = num_clients + 1;
        int clientPort = port_req_rep + playerNumber;  // Unique port per client

        std::string reply = std::to_string(playerNumber) + " " + std::to_string(clientPort);
        zmq::message_t reply_msg(reply.size());
        memcpy(reply_msg.data(), reply.c_str(), reply.size());

        // Send the reply back to the client
        rep_socket.send(reply_msg, zmq::send_flags::none);
        std::cout << "Sent player number and port: " << reply << std::endl;

        // Add the client once the reply is sent
        addClient(gameState, clientPort);
    }
}

// Adds a new client and updates the client count
void JServer::addClient(GameState& gameState, int clientPort) {
    std::lock_guard<std::mutex> lock(client_mutex);
    num_clients++;

    // Create a new REP socket for this client
    zmq::socket_t* client_socket = new zmq::socket_t(context, ZMQ_REP);
    client_socket->bind("tcp://*:" + std::to_string(clientPort));

    std::cout << "Client " << num_clients << " connected on port " << clientPort << std::endl;

    // Spawn a thread to handle this client's input with the unique socket
    client_threads.push_back(std::thread(&JServer::handleClientInput, this, std::ref(gameState), client_socket));
}

// Handles receiving client input (via the dedicated REP socket per client)
void JServer::handleClientInput(GameState& gameState, zmq::socket_t* client_socket) {
    while (true) {
        zmq::message_t request;

        // Receive the action request from the client
        if (client_socket->recv(request, zmq::recv_flags::none)) {
            std::string client_input(static_cast<char*>(request.data()), request.size());
            std::cout << "Received input: " << client_input << std::endl;

            // Deserialize the input (expected format: "name positionx positiony")
            std::istringstream iss(client_input);
            std::string sprite_name;
            double position_x, position_y;
            iss >> sprite_name >> position_x >> position_y;

            // Process the input
            auto* sprite = gameState.getGameWorld().getObjectByName(sprite_name);
            if (sprite) {
                double last_x = sprite->getPosition()->getX();
                double last_y = sprite->getPosition()->getY();
                sprite->getPosition()->setX(position_x);
                sprite->getPosition()->setY(position_y);

                // Check for collisions using the PhysicsEngine
                auto& physicsEngine = gameState.getPhysicsEngine();
                if (physicsEngine.checkCollisions(*sprite, gameState)) {
                    // Revert the position if collision detected
                    sprite->getPosition()->setX(last_x);
                    sprite->getPosition()->setY(last_y);
                }
            }

            // Get game state message
            std::string game_state_message = serializeMovingSprites(getMovingSprites(gameState));

            // Send back updated game state
            zmq::message_t reply_msg(game_state_message.size());
            memcpy(reply_msg.data(), game_state_message.c_str(), game_state_message.size());
            client_socket->send(reply_msg, zmq::send_flags::none);
        }
    }
}

// Helper function for making map of sprites that move
std::unordered_map<std::string, std::pair<double, double>> JServer::getMovingSprites(GameState& gameState) {
    std::unordered_map<std::string, std::pair<double, double>> movingSprites;
    for (const auto& object_pair : gameState.getGameWorld().getObjects()) {
        NamedSprite* object = object_pair.second;
        const std::string& name = object_pair.first;
        if ((name.find("platform") != std::string::npos || name.find("Player") != std::string::npos)) {
            movingSprites[name] = { object->getPosition()->getX(), object->getPosition()->getY() };
        }
    }
    return movingSprites;
}

// Helper function for serializing a map
std::string JServer::serializeMovingSprites(const std::unordered_map<std::string, std::pair<double, double>>& movingSprites) {
    std::string game_state_message;
    for (const auto& sprite : movingSprites) {
        const std::string& name = sprite.first;
        const std::pair<double, double>& position = sprite.second;
        game_state_message += name + " " + std::to_string(position.first) + " " + std::to_string(position.second) + ";";
    }
    return game_state_message;
}

// Returns the number of connected clients
int JServer::getNumClients() const {
    return num_clients;
}

/*
// Constructor for the JServer class
JServer::JServer(int port_pub_sub, int port_req_rep)
    : port_pub_sub(port_pub_sub), port_req_rep(port_req_rep), num_clients(0), context(1),
    pub_socket(context, ZMQ_PUB), rep_socket(context, ZMQ_REP) {

    // Bind the PUB socket for broadcasting game state
    pub_socket.bind("tcp://*:" + std::to_string(port_pub_sub));

    // Bind the REQ-REP socket for client player number requests
    rep_socket.bind("tcp://*:" + std::to_string(port_req_rep));
}

// Destructor for the JServer class
JServer::~JServer() {
    for (auto& th : client_threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}

// Handles player number requests (REQ-REP communication)
void JServer::handlePlayerNumberRequests(GameState& gameState) {
    zmq::message_t request_msg;
    if (rep_socket.recv(request_msg, zmq::recv_flags::dontwait)) {
        std::string request(static_cast<char*>(request_msg.data()), request_msg.size());
        std::cout << "Received request: " << request << std::endl;

        // Respond with the current number of clients + port for this client
        int playerNumber = num_clients + 1;
        int clientPort = port_req_rep + playerNumber;  // Unique port per client

        std::string reply = std::to_string(playerNumber) + " " + std::to_string(clientPort);
        zmq::message_t reply_msg(reply.size());
        memcpy(reply_msg.data(), reply.c_str(), reply.size());

        // Send the reply back to the client
        rep_socket.send(reply_msg, zmq::send_flags::none);
        std::cout << "Sent player number and port: " << reply << std::endl;

        // Add the client once the reply is sent
        addClient(gameState, clientPort);
    }
}

// Adds a new client and updates the client count
void JServer::addClient(GameState& gameState, int clientPort) {
    std::lock_guard<std::mutex> lock(client_mutex);
    num_clients++;

    // Create a new REP socket for this client
    zmq::socket_t* client_socket = new zmq::socket_t(context, ZMQ_REP);
    client_socket->bind("tcp://*:" + std::to_string(clientPort));

    std::cout << "Client " << num_clients << " connected on port " << clientPort << std::endl;

    // Spawn a thread to handle this client's input with the unique socket
    client_threads.push_back(std::thread(&JServer::handleClientInput, this, std::ref(gameState), client_socket));
}

// Handles receiving client input (via the dedicated REP socket per client)
void JServer::handleClientInput(GameState& gameState, zmq::socket_t* client_socket) {
    while (true) {
        zmq::message_t request;

        // Receive the action request from the client
        if (client_socket->recv(request, zmq::recv_flags::none)) {
            std::string client_input(static_cast<char*>(request.data()), request.size());
            std::cout << "Received input: " << client_input << std::endl;

            // Deserialize the input (expected format: "name positionx positiony")
            std::istringstream iss(client_input);
            std::string sprite_name;
            double position_x, position_y;
            iss >> sprite_name >> position_x >> position_y;

            // Process the input
            auto* sprite = gameState.getGameWorld().getObjectByName(sprite_name);
            std::string reply = "false";  // Default to false

            if (sprite) {
                double last_x = sprite->getPosition()->getX();
                double last_y = sprite->getPosition()->getY();
                sprite->getPosition()->setX(position_x);
                sprite->getPosition()->setY(position_y);

                // Check for collisions using the PhysicsEngine
                auto& physicsEngine = gameState.getPhysicsEngine();
                if (!physicsEngine.checkCollisions(*sprite, gameState)) {
                    reply = "true";  // Valid move
                }
                else {
                    // Revert the position if collision detected
                    sprite->getPosition()->setX(last_x);
                    sprite->getPosition()->setY(last_y);
                }
            }

            // Send back true/false depending on action validity
            zmq::message_t reply_msg(reply.size());
            memcpy(reply_msg.data(), reply.c_str(), reply.size());
            client_socket->send(reply_msg, zmq::send_flags::none);
        }
    }
}

// Broadcasts the current game state to all clients (via PUB-SUB communication)
void JServer::broadcastGameState(const std::unordered_map<std::string, std::pair<double, double>>& movingSprites) {
    std::string game_state_message;
    for (const auto& sprite : movingSprites) {
        const std::string& name = sprite.first;
        const std::pair<double, double>& position = sprite.second;
        game_state_message += name + " " + std::to_string(position.first) + " " + std::to_string(position.second) + ";";
    }

    zmq::message_t msg(game_state_message.size());
    memcpy(msg.data(), game_state_message.data(), game_state_message.size());
    pub_socket.send(msg, zmq::send_flags::none);

    std::cout << "Broadcasted game state to clients: " << game_state_message << std::endl;
}

// Returns the number of connected clients
int JServer::getNumClients() const {
    return num_clients;
}
*/

/////////////////////////////////////////////////////////////////////////////////

/*
// Constructor for the JServer class
JServer::JServer(int port_push_pull, int port_pub_sub, int port_req_rep)
    : port_push_pull(port_push_pull), port_pub_sub(port_pub_sub), port_req_rep(port_req_rep),
    num_clients(0), context(1),
    pull_socket(context, ZMQ_PULL), pub_socket(context, ZMQ_PUB), rep_socket(context, ZMQ_REP) {

    // Bind the sockets to their respective ports
    pull_socket.bind("tcp://*:" + std::to_string(port_push_pull));  // For receiving client inputs
    pub_socket.bind("tcp://*:" + std::to_string(port_pub_sub));      // For broadcasting game state
    rep_socket.bind("tcp://*:" + std::to_string(port_req_rep));      // For responding to client player number requests
}

// Destructor for the JServer class
JServer::~JServer() {
    for (auto& th : client_threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}

// Adds a new client and updates the client count
void JServer::addClient(GameState& gameState) {
    std::lock_guard<std::mutex> lock(client_mutex);
    num_clients++;
    std::cout << "Client " << num_clients << " connected." << std::endl;

    // Spawn a thread to handle client input, passing GameState by reference
    client_threads.push_back(std::thread(&JServer::handleClientInput, this, std::ref(gameState)));
}


// Handles player number requests (REQ-REP communication)
void JServer::handlePlayerNumberRequests(GameState& gameState) {
    // Non-blocking check for a client request
    zmq::message_t request_msg;
    if (rep_socket.recv(request_msg, zmq::recv_flags::dontwait)) {
        std::string request(static_cast<char*>(request_msg.data()), request_msg.size());
        std::cout << "Received request: " << request << std::endl;

        // Respond with the current number of clients + 1 (the player ID)
        std::string reply = std::to_string(num_clients + 1);
        zmq::message_t reply_msg(reply.size());
        memcpy(reply_msg.data(), reply.c_str(), reply.size());

        // Send the reply back to the client
        rep_socket.send(reply_msg, zmq::send_flags::none);
        std::cout << "Sent player number: " << reply << std::endl;

        // Add the client
        addClient(gameState);
    }
}

// Handles receiving client input (via PUSH-PULL communication)
void JServer::handleClientInput(GameState& gameState) {
    while (true) {
        zmq::message_t input_msg;

        // Receive client input (blocking call)
        if (pull_socket.recv(input_msg, zmq::recv_flags::none)) {
            std::string client_input(static_cast<char*>(input_msg.data()), input_msg.size());
            std::cout << "Received input: " << client_input << std::endl;

            // Deserialize the input (expected format: "name positionx positiony")
            std::istringstream iss(client_input);
            std::string sprite_name;
            double position_x, position_y;

            // Extract values from the input string
            iss >> sprite_name >> position_x >> position_y;

            // Get the sprite by name from GameState's GameWorld
            auto* sprite = gameState.getGameWorld().getObjectByName(sprite_name);
            if (!sprite) {
                std::cerr << "Sprite not found: " << sprite_name << std::endl;
                continue;  // Ignore this input if sprite is not found
            }

            // Store the last position of the sprite before updating
            double last_x = sprite->getPosition()->getX();
            double last_y = sprite->getPosition()->getY();

            // Update the sprite's position to the new one
            sprite->getPosition()->setX(position_x);
            sprite->getPosition()->setY(position_y);

            // Check for collisions using the PhysicsEngine
            auto& physicsEngine = gameState.getPhysicsEngine();
            if (physicsEngine.checkCollisions(*sprite, gameState)) {
                // Revert to the last position if a collision is detected
                std::cout << sprite_name << " invalid action! Reverting position." << std::endl;
                sprite->getPosition()->setX(last_x);
                sprite->getPosition()->setY(last_y);
            }
        }
    }
}


// Broadcasts the current game state to all clients (via PUB-SUB communication)
void JServer::broadcastGameState(const std::unordered_map<std::string, std::pair<double, double>>& movingSprites) {
    // Serialize the movingShapes map into a string to send to the clients
    std::string game_state_message;
    for (const auto& sprite : movingSprites) {
        const std::string& name = sprite.first;
        const std::pair<double, double>& position = sprite.second;
        game_state_message += name + " " + std::to_string(position.first) + " " + std::to_string(position.second) + ";";
    }

    // Send the game state message to all connected clients
    zmq::message_t msg(game_state_message.size());
    memcpy(msg.data(), game_state_message.data(), game_state_message.size());
    pub_socket.send(msg, zmq::send_flags::none);

    std::cout << "Broadcasted game state to clients: " << game_state_message << std::endl;
}

// Returns the number of connected clients
int JServer::getNumClients() const {
    return num_clients;
}
*/