#pragma once
#include "EventHandler.h"
#include "ReplayManager.h"
#include "GameState.h"
#include "GameWorld.h"
#include "PlayReplayEvent.h"
#include <iostream>

//class PlayReplayEventHandler : public EventHandler {
//private:
//    ReplayManager& replayManager;
//    GameState& gameState;
//    size_t currentReplayIndex;
//    bool isReplaying;
//
//public:
//    PlayReplayEventHandler(ReplayManager& manager, GameState& gs)
//        : replayManager(manager),
//        gameState(gs),
//        currentReplayIndex(0),
//        isReplaying(false) {}
//
//    void onEvent(std::shared_ptr<Event> event) override {
//        auto playEvent = std::static_pointer_cast<PlayReplayEvent>(event);
//
//        if (!replayManager.hasRecordedStates()) {
//            std::cout << "No replay available to play" << std::endl;
//            return;
//        }
//
//        isReplaying = true;
//        currentReplayIndex = 0;
//
//        // Store initial game state before replay
//        auto& gameStates = replayManager.getGameStates();
//        if (!gameStates.empty()) {
//            std::cout << "Starting replay playback" << std::endl;
//            // Set initial state
//            applyGameState(gameStates[0].second);
//        }
//    }
//
//    void update() {
//        if (!isReplaying) return;
//
//        auto& gameStates = replayManager.getGameStates();
//        if (currentReplayIndex >= gameStates.size()) {
//            isReplaying = false;
//            std::cout << "Replay finished" << std::endl;
//            return;
//        }
//
//        // Apply the next state
//        applyGameState(gameStates[currentReplayIndex].second);
//        currentReplayIndex++;
//    }
//
//private:
//    void applyGameState(const std::unordered_map<std::string, std::tuple<size_t, double, double>>& state) {
//        for (const auto& pair : state) {
//            const std::string& name = pair.first;
//            const auto& data = pair.second;
//
//            if (NamedSprite* sprite = gameState.getGameWorld().getObjectByName(name)) {
//                sprite->getPosition()->setX(std::get<1>(data));
//                sprite->getPosition()->setY(std::get<2>(data));
//            }
//        }
//    }
//};

class PlayReplayEventHandler : public EventHandler {
private:
    ReplayManager& replayManager;
    GameState& gameState;
    size_t currentReplayIndex;
    bool isReplaying;
    double replayStartTime;

public:
    PlayReplayEventHandler(ReplayManager& manager, GameState& gs)
        : replayManager(manager),
        gameState(gs),
        currentReplayIndex(0),
        isReplaying(false),
        replayStartTime(0) {}

    void onEvent(std::shared_ptr<Event> event) override {
        /*std::cout << "PlayReplayEventHandler::onEvent called" << std::endl;
        auto playEvent = std::static_pointer_cast<PlayReplayEvent>(event);

        if (!replayManager.hasRecordedStates()) {
            std::cout << "No replay available to play" << std::endl;
            return;
        }

        isReplaying = true;
        currentReplayIndex = 0;
        replayStartTime = gameState.getTimeline().getTime();
        std::cout << "Starting replay playback, states count: "
            << replayManager.getStateCount() << std::endl;*/
        std::cout << "PlayReplayEventHandler::onEvent called" << std::endl;
        replayManager.startReplay();
    }

    bool isInReplay() const { return isReplaying; }

    void update() {
        if (!isReplaying) return;

        auto& gameStates = replayManager.getGameStates();
        if (currentReplayIndex >= gameStates.size()) {
            isReplaying = false;
            std::cout << "Replay finished" << std::endl;
            return;
        }

        // Apply the current state
        applyGameState(gameStates[currentReplayIndex].second);
        currentReplayIndex++;
    }

private:
    void applyGameState(const std::unordered_map<std::string, std::tuple<size_t, double, double>>& state) {
        for (const auto& pair : state) {
            const std::string& name = pair.first;
            const auto& data = pair.second;

            if (auto* sprite = gameState.getGameWorld().getObjectByName(name)) {
                sprite->getPosition()->setX(std::get<1>(data));
                sprite->getPosition()->setY(std::get<2>(data));
            }
        }
    }
};