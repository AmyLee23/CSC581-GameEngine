#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class ReplayManager {
private:
    bool isRecording;
    bool isReplaying;
    double recordingStartTime;
    size_t currentReplayIndex;
    // Store complete game states as received from server
    std::vector<std::pair<double, std::unordered_map<std::string, std::tuple<size_t, double, double>>>> gameStates;

public:
    ReplayManager() : isRecording(false), isReplaying(false), recordingStartTime(0.0), currentReplayIndex(0) {}

    void startRecording(double currentTime) {
        isRecording = true;
        recordingStartTime = currentTime;
        gameStates.clear();
    }

    void stopRecording() {
        isRecording = false;
    }

    bool getIsRecording() const { return isRecording; }

    // Record the entire game state at each update
    void recordGameState(double time,
        const std::unordered_map<std::string, std::tuple<size_t, double, double>>& state) {
        if (isRecording) {
            gameStates.push_back({ time - recordingStartTime, state });
            
        }
    }

    bool hasRecordedStates() const {
        return !gameStates.empty();
    }

    const std::vector<std::pair<double, std::unordered_map<std::string, std::tuple<size_t, double, double>>>>&
        getGameStates() const {
        return gameStates;
    }

    size_t getStateCount() const {
        return gameStates.size();
    }

    size_t getCurrentReplayIndex() const {
        return currentReplayIndex;
    }

    void setisInReplay(bool isInReplay) {
        isReplaying = isInReplay;
    }

    void setCurrentReplayIndex(size_t currReplayIndex) {
        currentReplayIndex = currReplayIndex;
    }

    void startReplay() {
        if (!hasRecordedStates()) {
            std::cout << "No replay available to play" << std::endl;
            return;
        }
        isReplaying = true;
        currentReplayIndex = 0;
        std::cout << "Starting replay playback, states count: " << gameStates.size() << std::endl;
    }

    bool isInReplay() const { return isReplaying; }
};