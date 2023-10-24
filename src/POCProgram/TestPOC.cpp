//
// Created by Theophilus Homawoo on 24/09/2023.
//

#include <iostream>
#include "GameEngine.hpp"
#include "ComponentsType.hpp"


namespace GameEngine {

    class TransformComponent : public IComponent {
    public:
        float x, y;

        TransformComponent(float x, float y) : x(x), y(y) {}

        size_t getComponentType() override {
            return 0;
        }
    };

    class RenderSystem : public ISystem {
    public:
        void update(ComponentsContainer& componentsContainer, EventHandler& eventHandler) override {

            std::cout << "Rendering entities" << std::endl;
        }
    };

    class PhysicsSystem : public ISystem {
    public:
        void update(ComponentsContainer& componentsContainer, EventHandler& eventHandler) override {
            // Update positions here
            std::cout << "Updating physics" << std::endl;
        }
    };
} // namespace GameEngine

int main() {
    GameEngine::GameEngine engine;

    // Create entities
    size_t entity1 = engine.createEntity();
    size_t entity2 = engine.createEntity();

    engine.bindComponentToEntity(entity1, std::make_optional(std::make_shared<GameEngine::TransformComponent>(1.0f, 2.0f)));
    engine.bindComponentToEntity(entity2, std::make_optional(std::make_shared<GameEngine::TransformComponent>(3.0f, 4.0f)));

    auto renderSystem = std::make_shared<GameEngine::RenderSystem>();
    auto physicsSystem = std::make_shared<GameEngine::PhysicsSystem>();

    engine.addSystem("RenderSystem", renderSystem);
    engine.addSystem("PhysicsSystem", physicsSystem);

    engine.scheduleEvent("gameEngineStop", 600);

    engine.run();

    return 0;
}
