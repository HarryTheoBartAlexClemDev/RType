//
// Created by alexandre on 30/10/23.
//

#include "PowerUpDualShoot.hpp"

void Server::PowerUpDualShoot::update(GameEngine::ComponentsContainer &componentsContainer, GameEngine::EventHandler &eventHandler)
{
    auto id = std::any_cast<int>(eventHandler.getTriggeredEvent().second);

    auto player = std::dynamic_pointer_cast<IsPlayer>(componentsContainer.getComponent(id, GameEngine::ComponentsType::getComponentType("IsPlayer")).value());
    auto posPlayer = std::dynamic_pointer_cast<PhysicsEngine::PositionComponent2D>(componentsContainer.getComponent(id, GameEngine::ComponentsType::getComponentType("PositionComponent2D")).value());
    auto shooter = std::dynamic_pointer_cast<Shooter>(componentsContainer.getComponent(id, GameEngine::ComponentsType::getComponentType("Shooter")).value());

    EntityFactory::getInstance().spawnPowersDualShoot(componentsContainer, eventHandler, PowerType::DUALSHOOT,Utils::Vect2( posPlayer->pos.x + shooter->shootPosition.x, posPlayer->pos.y + shooter->shootPosition.y + 20), Utils::Vect2( posPlayer->pos.x + shooter->shootPosition.x, posPlayer->pos.y + shooter->shootPosition.y - 10));
}