//
// Created by Theophilus Homawoo on 08/10/2023.
//

#pragma once

#include "ISystem.hpp"
#include "EventHandler.hpp"
#include "ComponentsType.hpp"

class CollisionHandler : public GameEngine::ISystem {
public:
    void update(GameEngine::ComponentsContainer &componentsContainer, GameEngine::EventHandler &eventHandler) override;
    int call = 0;
};