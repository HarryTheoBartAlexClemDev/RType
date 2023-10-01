/*
** EPITECH PROJECT, 2023
** RType
** File description:
** main
*/

#include "ComponentContainer.hpp"
#include "EntityComponents.hpp"
#include "EntityFactory.hpp"
#include "EntitySystems.hpp"
#include "GameEngine.hpp" // Assuming all your game engine components are in this header
#include "RenderEngineSystem.hpp"
#include <iostream>

int main() {
    GameEngine::GameEngine engine;

    auto updateSprite = std::make_shared<GameEngine::updateEntitySpriteSystem>();
    engine.addSystem("updateSpriteSystem", updateSprite);
    auto move = std::make_shared<GameEngine::updatePositionSystem>();
    engine.addSystem("updatePositionSystem", move);
    auto health = std::make_shared<GameEngine::updateHealthSystem>();
    engine.addSystem("HealthSystem", health);
    auto mobDeath = std::make_shared<GameEngine::MobDeathSystem>();
    engine.addSystem("MobDeathSystem", mobDeath);

    for (int i = 0; i < 5; i++) {
        size_t id = EntityFactory::getInstance().createBaseMob(engine, "../../../Assets/classic-mob.gif", 34, 200, 6,
                                                               true, "../../../Assets/explosion.gif", 33, 200, 6, 100,
                                                               100 + 1 * 50, 0, 1, 0, -1, 32, 32, 100, 10, 0, 0);
    }

    engine.run();

    return 0;
}