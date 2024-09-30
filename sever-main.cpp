//#include "NetworkManager.h"
//#include <unordered_map>
//#include <string>
//#include <iostream>
//#include <thread>
//#include <unordered_set>
//#include <utility> // for std::pair
//#include <stdexcept> // for std::invalid_argument
//
//// Assume Position struct to store coordinates
//struct Position {
//    int x, y;
//
//    bool operator==(const Position& other) const {
//        return x == other.x && y == other.y;
//    }
//
//    std::string toString() const {
//        return std::to_string(x) + "," + std::to_string(y);
//    }
//};
//
//struct PositionHasher {
//    std::size_t operator()(const Position& p) const {
//        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
//    }
//};
//
//std::unordered_map<std::string, Position> clientPositions;
//std::unordered_set<Position, PositionHasher> occupiedPositions;
//
///**
// * Parses a position from a string formatted as "x,y".
// * @param positionStr The string representing the position.
// * @return A pair of integers representing the x and y coordinates.
// * @throws std::invalid_argument if the format is incorrect.
// */
//Position parsePosition(const std::string& positionStr) {
//    size_t commaPos = positionStr.find(',');
//    if (commaPos == std::string::npos || commaPos == 0 || commaPos == positionStr.length() - 1) {
//        //throw std::invalid_argument("Invalid position format. Expected format 'x,y'.");
//    }
//
//    try {
//        int x = std::stoi(positionStr.substr(0, commaPos));
//        int y = std::stoi(positionStr.substr(commaPos + 1));
//        Position pos;
//        pos.x = x, pos.y = y;
//        return pos;
//    }
//    catch (const std::exception& e) {
//        // Catch exceptions thrown by std::stoi if the conversion fails
//        //throw std::invalid_argument("Invalid numeric values in the position string.");
//    }
//}
//
////int main() {
////    NetworkManager networkManager(true);
////    networkManager.init(5555);
////
////
////    while (true) {
////		
////        std::string clientIdAndPosition = networkManager.receiveMessage();
////        if (!clientIdAndPosition.empty()) {
////            std::cout << "Received message: " << clientIdAndPosition << std::endl;
////
////            auto delimPos = clientIdAndPosition.find(':');
////            std::string clientId = clientIdAndPosition.substr(0, delimPos);
////            std::cout << "Client ID: " << clientId << endl;
////            Position newPos = parsePosition(clientIdAndPosition.substr(delimPos + 1));
////            std::cout << "newPosition: " << newPos.x << ", " << newPos.y << endl;
////
////            //          // Update positions
////            Position& currentPos = clientPositions[clientId];
////            occupiedPositions.erase(currentPos);
////            currentPos = newPos;
////            occupiedPositions.insert(newPos);
////
////            networkManager.sendMessage(clientId, newPos.toString());
////            networkManager.publishMessage("Update - " + clientId, newPos.toString());
////        }
////
////        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // Frame rate control
////    }
////
////    return 0;
////}
//
//int main() {
//    NetworkManager networkManager(true);
//    networkManager.init(5555);
//
//    while (true) {
//        std::string clientIdAndPosition = networkManager.receiveMessage();
//        if (!clientIdAndPosition.empty()) {
//            std::cout << "2. Received message: " << clientIdAndPosition << std::endl;
//
//            auto delimPos = clientIdAndPosition.find(':');
//            std::string clientId = clientIdAndPosition.substr(0, delimPos);
//            Position newPos = parsePosition(clientIdAndPosition.substr(delimPos + 1));
//
//            bool isNewClient = (clientPositions.find(clientId) == clientPositions.end());
//
//            // Update positions
//            if (!isNewClient) {
//                occupiedPositions.erase(clientPositions[clientId]);
//            }
//            clientPositions[clientId] = newPos;
//            occupiedPositions.insert(newPos);
//
//            if (isNewClient) {
//                // Send back positions of all clients
//                std::string allPositions;
//                for (const auto& pair : clientPositions) {
//                    allPositions += pair.first + ":" + pair.second.toString() + ";";
//                }
//                cout << "3. All positions:" << allPositions << endl;
//                networkManager.sendMessage(clientId, allPositions);
//            }
//            else {
//                // Send back acknowledgment or the client's own position
//                networkManager.sendMessage(clientId, newPos.toString());
//            }
//
//            // Publish update to all clients
//            networkManager.publishMessage("Update - " + clientId, newPos.toString());
//        }
//
//        //std::this_thread::sleep_for(std::chrono::milliseconds(16));  // Frame rate control
//    }
//
//    return 0;
//}
