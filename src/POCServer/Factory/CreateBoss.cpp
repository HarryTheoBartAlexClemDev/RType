/*
** EPITECH PROJECT, 2023
** RType
** File description:
** CreateBoss
*/

#include "EntityFactory.hpp"
#include <cmath>
#include <cstddef>

namespace Server {
size_t
EntityFactory::createBellmiteBoss(GameEngine::ComponentsContainer &container,
                                  GameEngine::EventHandler &eventHandler,
                                  Utils::Vect2 pos) {
  nlohmann::json config = loadConfig("config/Entity/createBossBellmite.json");
  Utils::Vect2 velocity(
      config["createBellmiteBoss"]["velocity"]["x"].get<float>(),
      config["createBellmiteBoss"]["velocity"]["y"].get<float>());
  size_t entityId = createBossMob(
      container, config["createBellmiteBoss"]["hitboxHeight"].get<int>(),
      config["createBellmiteBoss"]["hitboxWidth"].get<int>(), pos, velocity,
      config["createBellmiteBoss"]["maxHealth"].get<int>(),
      config["createBellmiteBoss"]["damageValue"].get<int>(),
      config["createBellmiteBoss"]["stageValue"].get<int>(),
      config["createBellmiteBoss"]["scale"].get<float>());

  auto bossCore = std::make_shared<isBossCore>();
  container.bindComponentToEntity(entityId, bossCore);
  container.unbindComponentFromEntity(
      entityId, GameEngine::ComponentsType::getComponentType("Health"));
  std::vector<size_t> ids = {entityId};
  std::vector<std::any> args = {MobType::BELLMITE};
  std::shared_ptr<Network::Message> message =
      std::make_shared<Network::Message>("CREATED_MOB", ids, "", args);
  std::shared_ptr<Network::AllUsersMessage> allUserMsg =
      std::make_shared<Network::AllUsersMessage>(message);
  eventHandler.queueEvent("SEND_NETWORK", allUserMsg);
  EntityFactory::updateEntityNetwork(eventHandler, entityId, pos, velocity);
  return entityId;
}

size_t
EntityFactory::createBellmitePod(GameEngine::ComponentsContainer &container,
                                 GameEngine::EventHandler &eventHandler,
                                 Utils::Vect2 pos) {
  nlohmann::json config = loadConfig("config/Entity/createBossBellmite.json");
  Utils::Vect2 velocity(
      config["createBellmitePod"]["velocity"]["x"].get<float>(),
      config["createBellmitePod"]["velocity"]["y"].get<float>());

  size_t entityId = createBossMob(
      container, config["createBellmitePod"]["hitboxHeight"].get<int>(),
      config["createBellmitePod"]["hitboxWidth"].get<int>(), pos, velocity,
      config["createBellmitePod"]["maxHealth"].get<int>(),
      config["createBellmitePod"]["damageValue"].get<int>(),
      config["createBellmitePod"]["stageValue"].get<int>(),
      config["createBellmitePod"]["scale"].get<float>());

  auto bossPod = std::make_shared<isBossPod>();
  container.bindComponentToEntity(entityId, bossPod);
  eventHandler.scheduleEvent("animate", 8,
                             std::make_tuple(std::string("Pods"), entityId));
  std::vector<size_t> ids = {entityId};
  std::vector<std::any> args = {MobType::BELLMITE};
  std::shared_ptr<Network::Message> message =
      std::make_shared<Network::Message>("CREATED_MOB", ids, "", args);
  std::shared_ptr<Network::AllUsersMessage> allUserMsg =
      std::make_shared<Network::AllUsersMessage>(message);
  eventHandler.queueEvent("SEND_NETWORK", allUserMsg);
  return entityId;
}

size_t EntityFactory::createBellmite(GameEngine::ComponentsContainer &container,
                                     GameEngine::EventHandler &eventHandler,
                                     Utils::Vect2 pos) {

  const int totalPods = 21;
  const float podDiameter = 64.0 * 1.5f;
  const float maxRadius = 800.0 / 2;
  const float radiusIncrement = podDiameter * 0.80;

  Utils::Vect2 velocity = Utils::Vect2(0, 0);
  createBellmiteBoss(container, eventHandler, pos);

  float currentRadius = 0;
  int podsInCurrentRadius = 1;

  for (int i = 0; i < totalPods;) {
    float angleIncrement = 2 * M_PI / podsInCurrentRadius;

    for (int j = 0; j < podsInCurrentRadius && i < totalPods; j++, i++) {
      Utils::Vect2 podPos;
      podPos.x = pos.x + currentRadius * cos(j * angleIncrement);
      podPos.y = pos.y + currentRadius * sin(j * angleIncrement);

      createBellmitePod(container, eventHandler, podPos);
    }

    currentRadius += radiusIncrement;
    podsInCurrentRadius += 6;
  }

  eventHandler.scheduleEvent("bounceBoss", 2);

  return 0;
}
} // namespace Server

// size_t
// EntityFactory::createBellmiteBoss(GameEngine::ComponentsContainer &container,
//                                   GameEngine::EventHandler &eventHandler,
//                                   GameEngine::Vect2 pos,
//                                   GameEngine::Vect2 velocity) {
//   size_t entityId = createBossMob(container, "assets/bellmite-core.png", 64,
//   64,
//                                   1, false, false, "assets/boss-explode.png",
//                                   64, 320, 5, pos, velocity, 100, 10,
//                                   1, 2.0f);
//   auto bossCore = std::make_shared<isBossCore>();
//   container.bindComponentToEntity(entityId, bossCore);
//   container.unbindComponentFromEntity(
//       entityId, GameEngine::ComponentsType::getComponentType("Health"));
//   return entityId;
// }

// size_t
// EntityFactory::createBellmitePod(GameEngine::ComponentsContainer &container,
//                                  GameEngine::EventHandler &eventHandler,
//                                  GameEngine::Vect2 pos,
//                                  GameEngine::Vect2 velocity) {
//   size_t entityId = createBossMob(
//       container, "assets/bellmite-entities.png", 29, 124, 4, false, false,
//       "assets/explode-mob.gif", 33, 200, 6, pos, velocity, 100, 50, 1, 3.0f);
//   auto bossPod = std::make_shared<isBossPod>();
//   container.bindComponentToEntity(entityId, bossPod);
//   eventHandler.scheduleEvent("animate", 8,
//                              std::make_tuple(std::string("Pods"), entityId));
//   return entityId;
// }