#pragma once
#include <unordered_map>
#include <any>
#include <string>

class Blackboard
{
public:
    Blackboard();

    template <class T>
    T getValue(std::string key) {
        return std::any_cast<T>(blackboard_[key]);
    }

    bool containsKey(std::string key) {
        return blackboard_.contains(key);
    }

    void addValue(std::string key, std::any value) {
        blackboard_[key] = value;
    }

    void removeValue(std::string key) {
        blackboard_.erase(key);
    }

private:
    std::unordered_map<std::string, std::any> blackboard_;
};


namespace BTree{
    inline Blackboard blackboard;
}

