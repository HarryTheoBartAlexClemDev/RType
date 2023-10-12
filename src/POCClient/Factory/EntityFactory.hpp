/*
** EPITECH PROJECT, 2023
** RType
** File description:
** EntityFactory
*/

#pragma once

#include "AABBComponent2D.hpp"
#include "DeathAnimation.hpp"
#include "EventHandler.hpp"
#include "GameEngine.hpp"
#include "IsBoss.hpp"
#include "IsBullet.hpp"
#include "IsMob.hpp"
#include "IsPlayer.hpp"
#include "IsPowerUp.hpp"
#include "MobComponents.hpp"
#include "MovementComponent2D.hpp"
#include "PositionComponent2D.hpp"
#include "RectangleColliderComponent2D.hpp"
#include "RenderEngine.hpp"
#include "SpriteAnimation.hpp"
#include "Utils.hpp"
#include "VelocityComponent.hpp"
#include "AudioComponent.hpp"
#include <cstddef>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "AudioEngineSystem.hpp"
#include "AudioComponent.hpp"
#include "PlayerUtils.hpp"
#include "IsStarship.hpp"

class EntityFactory {
public:
  static EntityFactory &getInstance() {
    static EntityFactory instance;
    return instance;
  }
  EntityFactory(const EntityFactory &) = delete;
  EntityFactory &operator=(const EntityFactory &) = delete;

  size_t spawnCancerMob(GameEngine::ComponentsContainer &container,
                        GameEngine::EventHandler &eventHandler,
                        GameEngine::Vect2 pos, bool dropPowerup);

  size_t spawnPataPataMob(GameEngine::ComponentsContainer &container,
                          GameEngine::EventHandler &eventHandler,
                          GameEngine::Vect2 pos, bool dropPowerup);

  size_t spawnBugMob(GameEngine::ComponentsContainer &container,
                     GameEngine::EventHandler &eventHandler,
                     GameEngine::Vect2 pos, bool dropPowerup);

  size_t createNewPlayer(GameEngine::ComponentsContainer &container,
                         GameEngine::EventHandler &eventHandler,
                         GameEngine::Vect2 pos, PlayerNumber numberPlayer);
  size_t createNewStarship(GameEngine::ComponentsContainer &container,
                               GameEngine::EventHandler &eventHandler,
                               GameEngine::Vect2 pos, PlayerNumber playerNumber);

  size_t createPlayerBullet(GameEngine::ComponentsContainer &container,
                                  GameEngine::EventHandler &eventHandler,
                                  GameEngine::Vect2 pos, GameEngine::Vect2 velocity, size_t typeBullet);

  size_t createBaseEnemyBullet(GameEngine::ComponentsContainer &container,
                               GameEngine::EventHandler &eventHandler,
                               GameEngine::Vect2 pos, GameEngine::Vect2 velocity);

    nlohmann::json loadConfig(const std::string& filePath);


    void registerPlayer(size_t entityId, PlayerNumber numberPlayer) {
        _playerMap[entityId] = numberPlayer;
    }

    const std::map<size_t, PlayerNumber>& getPlayerMap() const {
        return _playerMap;
    }

    void registerEntity(size_t clientEntityId, size_t serverEntityId) {
        _entityIdMap[clientEntityId] = serverEntityId;
    }

    size_t getServerId(size_t clientEntityId) const {
        auto it = _entityIdMap.find(clientEntityId);
        if (it != _entityIdMap.end()) {
            return it->second;
        }
        throw std::runtime_error("Client entity ID not found");
    }

private:
  EntityFactory() = default;
  ~EntityFactory() = default;
  size_t createBaseMob(GameEngine::ComponentsContainer &container,
                    const std::string &spriteSheetPath, int spriteSheetHeight,
                    int spriteSheetWidth, int frames, bool twoDirections, bool reverse,
                    const std::string &deathSpriteSheetPath, int deathSpriteSheetHeight,
                    int deathSpriteSheetWidth, int deathFrames, GameEngine::Vect2 pos,
                    GameEngine::Vect2 velocity, int player, float scale,
                    float rotation, GameEngine::ColorR tint, int layer);

  size_t createSharhips(GameEngine::ComponentsContainer &container,
                                   const std::string &spriteSheetPath,
                                   int spriteSheetHeight, int spriteSheetWidth,
                                   int frames, bool twoDirections, bool reverse,
                                   GameEngine::Vect2 pos,
                                   GameEngine::Vect2 velocity, int playerA, float scale, size_t entityCharge,
                                   float rotation, GameEngine::ColorR tint, int layer);

  size_t createBossMob(GameEngine::ComponentsContainer &container,
                const std::string &spriteSheetPath, int spriteSheetHeight,
                int spriteSheetWidth, int frames, bool twoDirections, bool reverse,
                const std::string &deathSpriteSheetPath, int deathSpriteSheetHeight,
                int deathSpriteSheetWidth, int deathFrames, GameEngine::Vect2 pos,
                GameEngine::Vect2 velocity, int playerA,
                float scale, float rotation, GameEngine::ColorR tint, int layer);

  size_t createPlayer(GameEngine::ComponentsContainer &container,
                                   const std::string &spriteSheetPath,
                                   int spriteSheetHeight, int spriteSheetWidth,
                                   int frames, bool twoDirections, bool reverse,
                                   GameEngine::Vect2 pos,
                                   GameEngine::Vect2 velocity, int playerA, float scale, size_t entityCharge,
                                   float rotation, GameEngine::ColorR tint, int layer);

  size_t createBullet(GameEngine::ComponentsContainer &container,
                                   const std::string &spriteSheetPath,
                                   int spriteSheetHeight, int spriteSheetWidth,
                                   int frames, bool twoDirections, bool reverse,
                                   GameEngine::Vect2 pos,
                                   GameEngine::Vect2 velocity,
                                   bool isPlayerBullet, int playerA, const std::string &pathSound, float scale,
                                   float rotation, GameEngine::ColorR tint, int layer);

  size_t createPowerUp(GameEngine::ComponentsContainer &container, const std::string &spriteSheetPath,
                                    int rectX, int rectY, int rectWidth,
                                    int rectHeight, GameEngine::Vect2 pos,
                                    GameEngine::Vect2 velocity, int playerA, float scale,
                                    float rotation, GameEngine::ColorR tint, int layer);

  static size_t createChargeAnimation(GameEngine::ComponentsContainer &container,
    const std::string &spriteSheetPath, int spriteSheetHeight,
    int spriteSheetWidth, int frames, GameEngine::Vect2 pos, GameEngine::Vect2 velocity,
    float scale, float rotation, GameEngine::ColorR tint, bool twoDirection, bool reverse, int direction, int playerA, int layer);

  size_t createBaseEntity(GameEngine::ComponentsContainer &container,
                          const std::string &spriteSheetPath,
                          int spriteSheetHeight, int spriteSheetWidth,
                          int frames, bool twoDirections, bool reverse,
                          GameEngine::Vect2 pos, GameEngine::Vect2 velocity,
                          int player, float scale,
                          float rotation,
                          GameEngine::ColorR tint, int layer);

  static std::shared_ptr<SpriteAnimation>
  initAnimation(const std::string &spriteSheetPath, int frames, int width,
                int height, bool twoDirections, bool reverse, int direction,
                int player);

  std::shared_ptr<DeathAnimation>
  initDeathAnimation(const std::string &deathSpriteSheetPath, int deathFrames,
                     int deathWidth, int deathHeight);

        std::map<size_t, size_t> _entityIdMap;
        std::map<size_t, PlayerNumber> _playerMap;
        std::map<std::string, std::shared_ptr<GameEngine::AudioComponent>> _audioMap;
};