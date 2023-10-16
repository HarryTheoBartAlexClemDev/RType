/*
** EPITECH PROJECT, 2023
** RType
** File description:
** MobSpawn
*/

#include "EntityFactory.hpp"

size_t EntityFactory::spawnCancerMob(GameEngine::ComponentsContainer &container,
                                     GameEngine::EventHandler &eventHandler,
                                     GameEngine::Vect2 pos, bool dropPowerup) {

nlohmann::json config = loadConfig("config/Entity/createCancerMob.json");

size_t entityId = createBaseMob(
    container,
    config["createCancerMob"]["spriteSheetPath"].get<std::string>(),
    config["createCancerMob"]["spriteSheetHeight"].get<int>(),
    config["createCancerMob"]["spriteSheetWidth"].get<int>(),
    config["createCancerMob"]["frames"].get<int>(),
    config["createCancerMob"]["twoDirections"].get<bool>(),
    config["createCancerMob"]["reverse"].get<bool>(),
    config["createCancerMob"]["deathSpriteSheetPath"].get<std::string>(),
    config["createCancerMob"]["deathSpriteSheetHeight"].get<int>(),
    config["createCancerMob"]["deathSpriteSheetWidth"].get<int>(),
    config["createCancerMob"]["deathFrames"].get<int>(),
    pos,
    GameEngine::Vect2(
        config["createCancerMob"]["velocity"]["x"].get<float>(),
        config["createCancerMob"]["velocity"]["y"].get<float>()
    ),
    config["createCancerMob"]["maxHealth"].get<int>(),
    config["createCancerMob"]["damageValue"].get<int>(),
    config["createCancerMob"]["player"].get<int>(),
    config["createCancerMob"]["scale"].get<float>(),
    config["createCancerMob"]["rotation"].get<float>(),
    GameEngine::ColorR(
        config["createCancerMob"]["tint"]["r"].get<int>(),
        config["createCancerMob"]["tint"]["g"].get<int>(),
        config["createCancerMob"]["tint"]["b"].get<int>(),
        config["createCancerMob"]["tint"]["a"].get<int>()
    ),
    config["createCancerMob"]["layer"].get<int>(),
    config["createCancerMob"]["dropPowerup"].get<bool>()
);

  auto shooterComp = std::make_shared<Shooter>(GameEngine::Vect2(config["shootingPos"]["x"].get<float>(), config["shootingPos"]["y"].get<float>()), config["typeBullet"].get<int>());
  container.bindComponentToEntity(entityId, std::make_shared<Cancer>());
  container.bindComponentToEntity(entityId, shooterComp);
  eventHandler.scheduleEvent("animate", 30, std::make_tuple(std::string("Cancer"), entityId));
  auto IdCharge = std::make_tuple(entityId, 0);
  eventHandler.scheduleEvent("ShootSystem", 300, IdCharge);
  return entityId;
}

size_t
EntityFactory::spawnPataPataMob(GameEngine::ComponentsContainer &container,
                                GameEngine::EventHandler &eventHandler,
                                GameEngine::Vect2 pos, bool dropPowerup) {

    nlohmann::json config = loadConfig("config/Entity/createPatapataMob.json");

    size_t entityId = createBaseMob(
        container,
        config["createPatapataMob"]["spriteSheetPath"].get<std::string>(),
        config["createPatapataMob"]["spriteSheetHeight"].get<int>(),
        config["createPatapataMob"]["spriteSheetWidth"].get<int>(),
        config["createPatapataMob"]["frames"].get<int>(),
        config["createPatapataMob"]["twoDirections"].get<bool>(),
        config["createPatapataMob"]["reverse"].get<bool>(),
        config["createPatapataMob"]["deathSpriteSheetPath"].get<std::string>(),
        config["createPatapataMob"]["deathSpriteSheetHeight"].get<int>(),
        config["createPatapataMob"]["deathSpriteSheetWidth"].get<int>(),
        config["createPatapataMob"]["deathFrames"].get<int>(),
        pos,
        GameEngine::Vect2(
            config["createPatapataMob"]["velocity"]["x"].get<float>(),
            config["createPatapataMob"]["velocity"]["y"].get<float>()
        ),
        config["createPatapataMob"]["maxHealth"].get<int>(),
        config["createPatapataMob"]["damageValue"].get<int>(),
        config["createPatapataMob"]["player"].get<int>(),
        config["createPatapataMob"]["scale"].get<float>(),
        config["createPatapataMob"]["rotation"].get<float>(),
        GameEngine::ColorR(
            config["createPatapataMob"]["tint"]["r"].get<int>(),
            config["createPatapataMob"]["tint"]["g"].get<int>(),
            config["createPatapataMob"]["tint"]["b"].get<int>(),
            config["createPatapataMob"]["tint"]["a"].get<int>()
        ),
        config["createPatapataMob"]["layer"].get<int>(),
        config["createPatapataMob"]["dropPowerup"].get<bool>()
    );
  container.bindComponentToEntity(entityId, std::make_shared<PataPata>());
  container.bindComponentToEntity(
      entityId, std::make_shared<HeightVariation>(config["heightVarience"].get<float>(), config["maxVar"].get<float>(), pos.y));

  eventHandler.scheduleEvent(
      "animate", 10, std::make_tuple(std::string("PataPata"), entityId));
  return entityId;
}

size_t EntityFactory::spawnBugMob(GameEngine::ComponentsContainer &container,
                                  GameEngine::EventHandler &eventHandler,
                                  GameEngine::Vect2 pos, bool dropPowerup) {

nlohmann::json config = loadConfig("config/Entity/createBugMob.json");

size_t entityId = createBaseMob(
    container,
    config["createBugMob"]["spriteSheetPath"].get<std::string>(),
    config["createBugMob"]["spriteSheetHeight"].get<int>(),
    config["createBugMob"]["spriteSheetWidth"].get<int>(),
    config["createBugMob"]["frames"].get<int>(),
    config["createBugMob"]["twoDirections"].get<bool>(),
    config["createBugMob"]["reverse"].get<bool>(),
    config["createBugMob"]["deathSpriteSheetPath"].get<std::string>(),
    config["createBugMob"]["deathSpriteSheetHeight"].get<int>(),
    config["createBugMob"]["deathSpriteSheetWidth"].get<int>(),
    config["createBugMob"]["deathFrames"].get<int>(),
    pos,
    GameEngine::Vect2(
        config["createBugMob"]["velocity"]["x"].get<float>(),
        config["createBugMob"]["velocity"]["y"].get<float>()
    ),
    config["createBugMob"]["maxHealth"].get<int>(),
    config["createBugMob"]["damageValue"].get<int>(),
    config["createBugMob"]["player"].get<int>(),
    config["createBugMob"]["scale"].get<float>(),
    config["createBugMob"]["rotation"].get<float>(),
    GameEngine::ColorR(
        config["createBugMob"]["tint"]["r"].get<int>(),
        config["createBugMob"]["tint"]["g"].get<int>(),
        config["createBugMob"]["tint"]["b"].get<int>(),
        config["createBugMob"]["tint"]["a"].get<int>()
    ),
    config["createBugMob"]["layer"].get<int>(),
    config["createBugMob"]["dropPowerup"].get<bool>()
);


  container.bindComponentToEntity(entityId, std::make_shared<Bug>());
  return entityId;
}
