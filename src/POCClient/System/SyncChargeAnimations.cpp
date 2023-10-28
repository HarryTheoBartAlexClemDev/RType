/*
** EPITECH PROJECT, 2023
** RType
** File description:
** SyncChargeAnimations
*/

#include "SyncChargeAnimations.hpp"
#include "SpriteComponent.hpp"
#include <iostream>

void Client::SyncChargeAnimations::update(GameEngine::ComponentsContainer &componentsContainer,
                                          GameEngine::EventHandler &eventHandler) {

    auto players = componentsContainer.getEntitiesWithComponent(GameEngine::ComponentsType::getComponentType("IsStarship"));
    auto thisPlayer = componentsContainer.getEntityWithUniqueComponent(GameEngine::ComponentsType::getComponentType("IsPlayer"));
    

    players.push_back(thisPlayer);

    for (const auto& playerID : players) {
        
        auto starshipOpt = componentsContainer.getComponent(playerID, GameEngine::ComponentsType::getComponentType("IsStarship"));
        auto playerOpt = componentsContainer.getComponent(playerID, GameEngine::ComponentsType::getComponentType("IsPlayer"));

        if (!starshipOpt.has_value() && !playerOpt.has_value())
            continue;
        if (playerOpt.has_value()) {
            auto player = std::dynamic_pointer_cast<IsPlayer>(playerOpt.value());
            if (player->entityIdChargeAnimation == 0)
                continue;
            // auto spriteOpt = componentsContainer.getComponent(player->entityIdChargeAnimation, GameEngine::ComponentsType::getComponentType("SpriteComponent"));
            // if (spriteOpt.has_value()) {
            //     auto sprite = std::dynamic_pointer_cast<RenderEngine::SpriteComponent>(spriteOpt.value());
            //     std::cout << "entityID: " << player->entityIdChargeAnimation << std::endl;
            //     std::cout << "rect1: " << sprite->rect1.x << " " << sprite->rect1.y << std::endl;
            //     std::cout << "rectDim: " << sprite->rect1.w << " " << sprite->rect1.h << std::endl;
            // }
            updateAnimationPos(player->entityIdChargeAnimation, playerID, componentsContainer);
        }
        if (starshipOpt.has_value()) {
            auto starship = std::dynamic_pointer_cast<IsStarship>(starshipOpt.value());
            if (starship->entityIdChargeAnimation == 0)
                continue;
            // auto spriteOpt = componentsContainer.getComponent(starship->entityIdChargeAnimation, GameEngine::ComponentsType::getComponentType("SpriteComponent"));
            // if (spriteOpt.has_value()) {
            //     auto sprite = std::dynamic_pointer_cast<RenderEngine::SpriteComponent>(spriteOpt.value());
            //     std::cout << "entityID: " << starship->entityIdChargeAnimation << std::endl;
            //     std::cout << "rect1: " << sprite->rect1.x << " " << sprite->rect1.y << std::endl;
            //     std::cout << "rectDim: " << sprite->rect1.w << " " << sprite->rect1.h << std::endl;
            // }
            updateAnimationPos(starship->entityIdChargeAnimation, playerID, componentsContainer);
        }   
    }
}

void Client::SyncChargeAnimations::updateAnimationPos(size_t chargeAnimationID, size_t playerID, GameEngine::ComponentsContainer &componentsContainer) {
        auto chargeAnimationOpt = componentsContainer.getComponent(chargeAnimationID, GameEngine::ComponentsType::getComponentType("PositionComponent2D"));
        auto shooterOpt = componentsContainer.getComponent(playerID, GameEngine::ComponentsType::getComponentType("Shooter"));
        auto playerPosOpt = componentsContainer.getComponent(playerID, GameEngine::ComponentsType::getComponentType("PositionComponent2D"));

        if (!chargeAnimationOpt.has_value() || !shooterOpt.has_value() || !playerPosOpt.has_value())
            return;

        auto chargeAnimation = std::dynamic_pointer_cast<PhysicsEngine::PositionComponent2D>(chargeAnimationOpt.value());
        auto shooter = std::dynamic_pointer_cast<Shooter>(shooterOpt.value());
        auto playerPos = std::dynamic_pointer_cast<PhysicsEngine::PositionComponent2D>(playerPosOpt.value());

        chargeAnimation->pos.x = playerPos->pos.x + shooter->shootPosition.x;
        chargeAnimation->pos.y = playerPos->pos.y + shooter->shootPosition.y - 25;
}