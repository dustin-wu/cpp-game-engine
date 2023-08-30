#pragma once

#include "Engine/component.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <typeinfo>

typedef unsigned long long GameObjectID;

class GameObject
{
public:
    GameObject();
    GameObjectID ID();
    template <class T>
    void addComponent(const std::shared_ptr<T> component) {
        auto castedComponent = std::dynamic_pointer_cast<Component>(component);
//        std::cout << "Adding component to key: " << typeid(T).name() << std::endl;
        components[typeid(T)].push_back(castedComponent);
    }

    template <class T>
    void removeComponent() {
        components[typeid(T)].pop_back();
    }

    template <class T>
    void removeComponents() {
        components[typeid(T)].clear();
    }

    template <class T>
    void removeComponentByIndex(int index) {
        components[typeid(T)].erase(components[typeid(T)].begin() + index);
    }

    template <class T>
    std::shared_ptr<T> getComponent() {
        return dynamic_pointer_cast<T>(components[typeid(T)][0]);
    }

    template <class T>
    std::vector<std::shared_ptr<T>> getComponents() {
        std::vector<std::shared_ptr<T>> componentsOfType;
        for (auto &component : components[typeid(T)]) {
            auto castedComponent = std::dynamic_pointer_cast<T>(component);
            componentsOfType.push_back(castedComponent);
        }
        return componentsOfType;
    }

    std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> getAllComponents() {
        return components;
    }

    template <class T>
    std::shared_ptr<T> getComponentByIndex(int index) {
        return components[typeid(T)][index];
    }

    bool hasComponentOfType(std::type_index type) {
        if (!components.count(type)) return false;
        return components[type].size() > 0;
    }

private:
    // Map from type --> Component
    std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> components;

    GameObjectID ID_;

    // The ID to assign to the newest object; ++ when a new object is created
    static GameObjectID nextID_;
};
