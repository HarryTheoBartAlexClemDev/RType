//
// Created by alexandre on 24/10/23.
//

#include "EntityFactory.hpp"

size_t Client::EntityFactory::spawnPowerUp(GameEngine::ComponentsContainer &container, GameEngine::EventHandler &eventHandler, PowerUpType type)
{
    try {
        LoadConfig::ConfigData data = LoadConfig::LoadConfig::getInstance().loadConfig("config/Entity/createPowerUp.json");

        int size = data.getSize("/powers");
        for (int i = 0; i < size; i++) {
            PowerUpType typeTab = static_cast<PowerUpType>(data.getInt("/powers/" + std::to_string(i) + "/type"));
            if (typeTab == type) {
                Utils::Vect2 pos = {0, 0};
                Utils::Vect2 velocity = {0, 0};
                size_t entityId = createBaseEntity(
                        container,
                        data.getString("/powers/" + std::to_string(i) + "/path"),
                        data.getInt("/powers/" + std::to_string(i) + "/rect/height"),
                        data.getInt("/powers/" + std::to_string(i) + "/rect/width"),
                        data.getInt("/powers/" + std::to_string(i) + "/frames"),
                        data.getBool("/powers/" + std::to_string(i) + "/twoDirections"),
                        data.getBool("/powers/" + std::to_string(i) + "/reverse"),
                        pos,
                        velocity,
                        0,
                        data.getFloat("/powers/" + std::to_string(i) + "/scale"),
                        data.getFloat("/powers/" + std::to_string(i) + "/rotation"),
                        Utils::ColorR(
                                data.getInt("/powers/" + std::to_string(i) + "/tint/r"),
                                data.getInt("/powers/" + std::to_string(i) + "/tint/g"),
                                data.getInt("/powers/" + std::to_string(i) + "/tint/b"),
                                data.getInt("/powers/" + std::to_string(i) + "/tint/a")
                        ),
                        data.getInt("/powers/" + std::to_string(i) + "/layer")
                        );
                eventHandler.scheduleEvent("animate", 30, std::make_tuple(std::string("PowerUp"), entityId));
                return entityId;
            }
        }
        return 0;
    } catch (std::bad_any_cast &e) {
        std::cerr << "Error from UpdatePosition System " << e.what() << std::endl;
        exit(84);
    }
}
