//#include "WindowManager.h"
//#include "PhysicsEngine.h"
//#include "GameWorld.h"
//#include "Timeline.h"
//#include "GameState.h"
//#include "NamedSprite.h"
//#include "StandardPosition.h"
//#include "BouncingMovement.h"
//#include "ControllableFlyer.h"
//#include "SimpleRectDrawer.h"
//#include "ControllableJumper.h"
//#include "NetworkManager.h"
//#include <sstream>
//
//#include <thread>
//
//double adjustTimeScale(double currentScale, double adjustment, double minScale, double maxScale) {
//    double newScale = currentScale + adjustment;
//    if (newScale > maxScale) {
//        return maxScale;
//    }
//    if (newScale < minScale) {
//        return minScale;
//    }
//    return newScale;
//}
//
//std::string serializePosition(int x, int y) {
//    return std::to_string(x) + "," + std::to_string(y);
//}
//
//std::pair<int, int> deserializePosition(const std::string& str) {
//    size_t commaPos = str.find(',');
//    int x = std::stoi(str.substr(0, commaPos));
//    int y = std::stoi(str.substr(commaPos + 1));
//    return { x, y };
//}
//
//
//int main(int argc, char* args[]) {
//    WindowManager windowManager;
//    PhysicsEngine physics;
//    Timeline globalTimeline(nullptr, 1);
//    GameWorld gameWorld(800, 600);
//    GameState gameState(globalTimeline, gameWorld, physics, windowManager);
//    
//    int count = 0;
//    string localClientID = "2";
//    cout << "0. Hello World I am client:" << localClientID << endl;
//    //client
//    NetworkManager client(false);
//    //client.init(5553);
//    client.init(5555);
//
//    gameState.getPhysicsEngine().setPixelsPerMeter(16.0);
//
//    if (!windowManager.init(800, 600)) {
//        std::cerr << "Failed to initialize!" << std::endl;
//        return -1;
//    }
//
//    SDL_Color green = { 0, 255, 0, 255 };
//    // fixed shape
//    PositionComponent* fp = new StandardPosition(0.0, static_cast<double>(gameWorld.getWorldHeight() - 50), gameWorld.getWorldWidth(), 50);
//    DrawerComponent* fd = new SimpleRectDrawer(green);
//    Timeline localTimeline1(&globalTimeline, 64);
//    NamedSprite* ground = new NamedSprite("ground", fp, nullptr, nullptr, fd, localTimeline1);
//    gameWorld.addObject(ground);
//
//    // bouncing shape
//    PositionComponent* bp = new StandardPosition(500.0, 200.0, 80, 60);
//    MovementComponent* bm = new BouncingMovement(30.0 / 100.0, 0);
//    DrawerComponent* bd = new SimpleRectDrawer(green);
//    Timeline localTimeline2(&globalTimeline, 10);
//    NamedSprite* platform = new NamedSprite("platform", bp, bm, nullptr, bd, localTimeline2);
//    gameWorld.addObject(platform);
//
//    SDL_Color red = { 255, 0, 0, 255 };
//
//    // controllable jumper
//    gameState.getPhysicsEngine().setGravity(5.0 / 100.0);
//    PositionComponent* jp = new StandardPosition(100.0, 100.0, 50, 50);
//    ControllableJumper* jmi = new ControllableJumper(45.0 / 100.0, 0.0, 1.5);  // Handles both movement and input
//    DrawerComponent* jd = new SimpleRectDrawer(red);
//    Timeline localTimeline4(&globalTimeline, 10);
//    NamedSprite* jumper = new NamedSprite("jumper" + localClientID, jp, jmi, jmi, jd, localTimeline4);
//    gameWorld.addObject(jumper);
//    gameWorld.addJumper(localClientID, jumper);
//    //gameWorld.addJumper(localClientID, jumper);
//    //PositionComponepos = jumper->getPosition(); // Assume getPosition returns a pair<int, int>
//    std::string positionUpdate = serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY());
//    //client.sendMessage("1", positionUpdate);
//    client.sendMessage(localClientID, positionUpdate);
//    std::cout << "1. Sending msg to server:" << positionUpdate << endl;
//    
//    string serverReply = client.receiveMessage();
//    while (serverReply.empty()) {
//        serverReply = client.receiveMessage();
//    }
//    if (!serverReply.empty()) {
//        std::cout << "4. server reply =" << serverReply<<endl;
//        size_t colonPos = serverReply.find(':');
//        serverReply = serverReply.substr(colonPos+2);
//        if (serverReply.find(';') != std::string::npos) {
//            // Received positions of all clients
//            std::istringstream iss(serverReply);
//            std::string clientPosStr;
//            while (std::getline(iss, clientPosStr, ';')) {
//                cout << "5. client posittion string =" << clientPosStr << endl;
//                if (clientPosStr.empty()) continue;
//                size_t colonPos = clientPosStr.find(':');
//                std::string clientId = clientPosStr.substr(0, colonPos-1);
//                cout << "6. client id in new clients part =" << clientId << endl;
//                std::string posStr = clientPosStr.substr(colonPos + 1);
//                auto position = deserializePosition(posStr);
//
//                if (clientId != localClientID) {
//                    std::cout << "7. New client in my world with ClientID:"<<localClientID<<"," << clientId <<"." << endl;
//                    auto otherJumper = gameWorld.getJumper(clientId);
//                    if (!otherJumper) {
//                        // Create a new jumper for the new client
//                        cout << "8. Creating new jumper for clientid:" << clientId<< endl;
//                        PositionComponent* jp = new StandardPosition(position.first, position.second, 50, 50);
//                        //ControllableJumper* jmi = new ControllableJumper(45.0 / 100.0, 0.0, 1.5);
//                        DrawerComponent* jd = new SimpleRectDrawer(red);
//                        Timeline localTimeline(&globalTimeline, 10);
//                        //jumper = new NamedSprite("jumper" + clientId, jp, jmi, jmi, jd, localTimeline);
//                        otherJumper = new NamedSprite("jumper" + clientId, jp, nullptr, nullptr, jd, localTimeline);
//                        gameWorld.addObject(otherJumper);
//                        gameWorld.addJumper(clientId, otherJumper);
//                    }
//            
//                }
//            }
//        }
//    }
//
//    bool quit = false;
//    SDL_Event e;
//    const Uint8* keystate = SDL_GetKeyboardState(NULL);
//
//    int targetFPS = 120;
//    int64_t timePerFrame = 1000 / targetFPS;
//    cout << "9. After recinvg the reply for server for the first time:"<< endl;
//    std::string currpos;
//    currpos= serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY());
//    while (!quit) {
//        if (currpos != serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY())) {
//            cout << "10. Position changed, sending message to server:" << endl;
//            currpos = serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY());
//            client.sendMessage(localClientID, currpos);
//        }
//        int64_t frameStartTime = globalTimeline.getTime();
//        //positionUpdate = serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY());
//        //client.sendMessage(localClientID, positionUpdate);
//
//    // Check for position updates and send messages if needed
//        //const int MIN_UPDATE_INTERVAL = 100;  // Minimum time between updates in milliseconds
//        //const double MIN_DISTANCE_UPDATE = 5.0;  // Minimum distance to trigger an update
//        //static int64_t lastUpdateTime = 0;
//        //static std::pair<int, int> lastSentPosition = { jumper->getPosition()->getX(), jumper->getPosition()->getY() };
//
//        /*int64_t currentTime = globalTimeline.getTime();
//        double currentDistance = distance(
//            { jumper->getPosition()->getX(), jumper->getPosition()->getY() },
//            lastSentPosition
//        );
//
//        if (currentDistance > MIN_DISTANCE_UPDATE) {
//            if (currentTime - lastUpdateTime > MIN_UPDATE_INTERVAL) {
//                cout << "10. Significant position change, sending message to server:" << endl;
//                std::string currpos = serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY());
//                client.sendMessage(localClientID, currpos);
//                lastUpdateTime = currentTime;
//                lastSentPosition = { jumper->getPosition()->getX(), jumper->getPosition()->getY() };
//            }
//        }*/
//
//
//        while (SDL_PollEvent(&e) != 0) {
//            // q to quit
//            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
//                quit = true;
//            }
//            // window resize
//            if (e.type == SDL_WINDOWEVENT) {
//                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
//                    int windowWidth = e.window.data1;
//                    int windowHeight = e.window.data2;
//                    windowManager.updateWindowSize(windowWidth, windowHeight, gameState);
//                }
//            }
//            // s to scale shapes, p to pause/unpause
//            if (e.type == SDL_KEYDOWN) {
//                //pair<int, int> pos = { jmi->getDx(), jmi->getDy() }; // Assume getPosition returns a pair<int, int>
//                
//                //std::string positionUpdate = serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY());
//                //client.sendMessage(localClientID, positionUpdate);
//               /* if (currpos != serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY())) {
//                    cout << "10. Position changed, sending message to server:" << endl;
//                    currpos = serializePosition(jumper->getPosition()->getX(), jumper->getPosition()->getY());
//                    client.sendMessage(localClientID, currpos);
//                }*/
//                if (e.key.keysym.sym == SDLK_s) {
//                    gameState.getWindowManager().toggleProportionalMode();
//                }
//                else if (e.key.keysym.sym == SDLK_p) {
//                    if (globalTimeline.isPaused()) {
//                        globalTimeline.unpause();
//                    }
//                    else {
//                        globalTimeline.pause();
//                    }
//                }
//            }
//            // - to slow down, + to speed up
//            if (e.type == SDL_KEYDOWN) {
//                double adjustment = 0.0;
//                if (e.key.keysym.sym == SDLK_EQUALS || e.key.keysym.sym == SDLK_PLUS) {
//                    adjustment = 0.1;
//                }
//                else if (e.key.keysym.sym == SDLK_MINUS) {
//                    adjustment = -0.1;
//                }
//                if (adjustment != 0.0) {
//                    double newGlobalTimeScale = adjustTimeScale(globalTimeline.getTimeScale(), adjustment, 0.5, 2.0);
//                    globalTimeline.setTimeScale(newGlobalTimeScale);
//                    for (auto& pair : gameWorld.getObjects()) {
//                        NamedSprite* sprite = pair.second;
//                        Timeline& spriteTimeline = sprite->getTimeline();
//                        double newSpriteTimeScale = adjustTimeScale(spriteTimeline.getTimeScale(), adjustment, 0.5, 2.0);
//                        spriteTimeline.setTimeScale(newSpriteTimeScale);
//                    }
//                }
//            }
//        }
//
//        std::string serverUpdate = client.receiveMessage();
//
//        if (!serverUpdate.empty()) {
//            cout << "11. Server update: " << serverUpdate << endl;
//        }
//        std::string pubUpdate = client.receivePubMessage();
//        
//        if (!pubUpdate.empty() && pubUpdate.find("Update - ") == 0) {
//            cout << "12. Received message from publisher:" << pubUpdate<<endl;
//            auto clientInfoEndPos = pubUpdate.find(" : ");
//            if (clientInfoEndPos != string::npos) {
//                std::string clientId = pubUpdate.substr(9, clientInfoEndPos - 10);
//                cout << "13. Client ID here:" << clientId << endl;
//                std::string posData = pubUpdate.substr(clientInfoEndPos + 3);
//                auto position = deserializePosition(posData);
//                PositionComponent* jp = new StandardPosition(position.first, position.second, 50, 50);
//                if (clientId != localClientID) { 
//                    cout << "14. Updating position for the following client:"<<clientId<< endl;
//                    auto otherJumper = gameWorld.getJumper(clientId);
//                    //auto otherJumper = gameWorld.
//                    if (!otherJumper) {
//                        // Create a new jumper for the new client
//                        cout  << "15. Creating new jumper for following client:" << clientId << endl;
//                        //ControllableJumper* jmi = new ControllableJumper(45.0 / 100.0, 0.0, 1.5); 
//                        DrawerComponent* jd = new SimpleRectDrawer(red);
//                        Timeline localTimeline(&globalTimeline, 10);
//                        otherJumper = new NamedSprite("jumper" + clientId, jp, nullptr, nullptr, jd, localTimeline);
//                        gameWorld.addJumper(clientId, otherJumper);
//                        gameWorld.addObject(otherJumper);
//                        unordered_map<std::string, NamedSprite*> objs = gameWorld.getObjects();
//                        cout << "===========================================================" << endl;
//                        cout << "16. Current map objects:"<<endl;
//                        for (auto x : objs) {
//                            cout << x.first << endl;
//                        }
//                        cout << "===========================================================" << endl;
//                    }
//                    else {
//                        cout  << "17. Existing jumper updating pos for following client:" << clientId << endl;
//
//                        // Update existing jumper's position
//                        otherJumper->setPosition(jp);
//                    }
//                }
//            }
//        }
//
//        gameState.step(keystate);
//        windowManager.updateWindow(gameState);
//
//        int64_t frameEndTime = globalTimeline.getTime();
//        int64_t frameDuration = frameEndTime - frameStartTime;
//        int64_t sleepTime = timePerFrame - frameDuration;
//
//        if (sleepTime > 0) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
//        }
//    }
//
//    windowManager.close();
//
//    /*double distance(const std::pair<int, int>& pos1, const std::pair<int, int>& pos2) {
//        return sqrt(pow(pos1.first - pos2.first, 2) + pow(pos1.second - pos2.second, 2));
//    }*/
//    return 0;
//}