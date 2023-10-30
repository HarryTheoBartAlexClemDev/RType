#include "AnimateDeath.hpp"
#include "AnimateOnMove.hpp"
#include "AudioComponent.hpp"
#include "AudioEngineSystem.hpp"
#include "ChangeDirPlayer.hpp"
#include "ChargingBar.hpp"
#include "Client.hpp"
#include "CollisionHandler.hpp"
#include "Component/DeathAnimation.hpp"
#include "ComponentContainer.hpp"
#include "CreatePlayer.hpp"
#include "DeleteEntities.hpp"
#include "Endpoint.hpp"
#include "EntityFactory.hpp"
#include "ForcePodSpawn.hpp"
#include "ISystem.hpp"
#include "InitParallax.hpp"
#include "IsChargingBar.hpp"
#include "MobHit.hpp"
#include "NetworkInput.hpp"
#include "NetworkOutput.hpp"
#include "Parallax.hpp"
#include "ParallaxPlanet.hpp"
#include "PhysicsEngineCollisionSystem2D.hpp"
#include "PhysicsEngineMovementSystem2D.hpp"
#include "PlayerHit.hpp"
#include "PlayerHitMob.hpp"
#include "RemoveHealth.hpp"
#include "RenderEngineSystem.hpp"
#include "ResetDirPlayer.hpp"
#include "RollBackBorder.hpp"
#include "Score.hpp"
#include "Shoot.hpp"
#include "Shooter.hpp"
#include "SpawnMob.hpp"
#include "SpriteComponent.hpp"
#include "SyncPosSprite.hpp"
#include "System/AnimateOnMove.hpp"
#include "TestInput.hpp"
#include "TextComponent.hpp"
#include "ToggleFullScreen.hpp"
#include "UpdateEntitySprite.hpp"
#include "UpdateScore.hpp"
#include "Vect2.hpp"
#include "ColorR.hpp"
#include "rect.hpp"
#include "VelocityComponent.hpp"
#include "WiggleMob.hpp"
#include "WindowInfoComponent.hpp"
#include "isHealthBar.hpp"
#include <iostream>
#include <memory>
#include "SpawnPowerUp.hpp"
#include "ButtonComponent.hpp"
#include "PhysicsEngineGravitySystem.hpp"


void initScene(GameEngine::GameEngine &engine) {
    auto collision = std::make_shared<PhysicsEngine::PhysicsEngineCollisionSystem2D>();
  auto movement = std::make_shared<PhysicsEngine::PhysicsEngineMovementSystem2D>();
  auto paralax = std::make_shared<Parallax>();
  auto paralaxPlanet = std::make_shared<ParallaxPlanet>();
  auto move = std::make_shared<ChangeDirPlayer>();
  auto reset = std::make_shared<ResetDirPlayer>();
  auto shoot = std::make_shared<Shoot>();
  auto sync = std::make_shared<SyncPosSprite>();

  Utils::rect rect2(0, 0, 1920, 1080);
  Utils::Vect2 pos2(0, 0);
  Utils::Vect2 pos3(1920, 0);

     Utils::ColorR tint = {255,255,255,255};
    float scale = 1.0f;
    float rotation = 0.0f;

  auto paralaxEntity = engine.createEntity();
  auto isParalaxComponent = std::make_shared<IsParallax>();
  engine.bindComponentToEntity(paralaxEntity, isParalaxComponent);
  auto velocityComponent = std::make_shared<PhysicsEngine::VelocityComponent>(
      Utils::Vect2(1.0f, 0.0f));
  engine.bindComponentToEntity(paralaxEntity, velocityComponent);
  auto spritecompoennt2 = std::make_shared<RenderEngine::SpriteComponent>(
      "assets/background_1.png", pos2, rect2, 2, scale, rotation, tint);
  engine.bindComponentToEntity(paralaxEntity, spritecompoennt2);

  auto paralaxEntity2 = engine.createEntity();
  auto isParalaxComponent1 =
      std::make_shared<IsParallax>();
  engine.bindComponentToEntity(paralaxEntity2, isParalaxComponent1);
  auto spritecompoennt3 = std::make_shared<RenderEngine::SpriteComponent>(
      "assets/background_1.png", pos3, rect2, 2, scale, rotation, tint);
  engine.bindComponentToEntity(paralaxEntity2, spritecompoennt3);

  auto animateOnMove = std::make_shared<AnimateOnMove>();
  auto forcePod = std::make_shared<ForcePodSpawn>();
  auto testInput = std::make_shared<TestInput>();
  auto render = std::make_shared<RenderEngine::RenderEngineSystem>("POC Engine", engine);
  auto deleteShoot = std::make_shared<DeleteEntities>();
  auto initParallax = std::make_shared<InitParallax>();
  auto toggleFullScreen = std::make_shared<RenderEngine::ToggleFullScreen>();
  auto PlayerHit1 = std::make_shared<PlayerHit>();
  auto MobHit1 = std::make_shared<MobHit>();
  auto PlayerHitMob1 = std::make_shared<PlayerHitMob>();
  auto borderStop = std::make_shared<RollBackBorder>();
  auto spawnPowerUp = std::make_shared<SpawnPowerUp>();
  auto gravitySystem = std::make_shared<GameEngine::PhysicsEngineGravitySystem>();

  auto window = engine.createEntity();

  engine.addEvent("SpawnPowerUp", spawnPowerUp);
  engine.addEvent("PlayerHit", PlayerHit1);
  engine.addEvent("MobHit", MobHit1);
  engine.addEvent("PlayerHitMob", PlayerHitMob1);
  engine.addEvent("InitParallax", initParallax);
  engine.queueEvent("InitParallax");
  engine.addEvent("toggleFullScreen", toggleFullScreen);
    engine.addSystem("GravitySystem", gravitySystem);
  engine.addSystem("CollisionSystem", collision);
  engine.addSystem("RollBackBorder", borderStop);
  engine.addSystem("MovementSystem", movement, 2);
  engine.addSystem("ParallaxSystem", paralax);
  engine.addSystem("ParallaxPlanetSystem", paralaxPlanet);
  engine.addSystem("SyncPosSPrite", sync, 3);
  engine.addSystem("RenderEngineSystem", render, 4);
  engine.addEvent("UP_KEY_PRESSED", move);
  engine.addEvent("UP_KEY_RELEASED", reset);
  engine.setContinuousEvent("UP_KEY_PRESSED", "UP_KEY_RELEASED");
  engine.addEvent("DOWN_KEY_PRESSED", move);
  engine.addEvent("DOWN_KEY_RELEASED", reset);
  engine.setContinuousEvent("DOWN_KEY_PRESSED", "DOWN_KEY_RELEASED");
  engine.addEvent("LEFT_KEY_PRESSED", move);
  engine.addEvent("LEFT_KEY_RELEASED", reset);
  engine.setContinuousEvent("LEFT_KEY_PRESSED", "LEFT_KEY_RELEASED");
  engine.addEvent("RIGHT_KEY_PRESSED", move);
  engine.addEvent("RIGHT_KEY_RELEASED", reset);
  engine.setContinuousEvent("RIGHT_KEY_PRESSED", "RIGHT_KEY_RELEASED");

  engine.addEvent("animatePlayer", animateOnMove);

  engine.addEvent("ShootSystem", shoot);

  engine.setContinuousEvent("SPACE_KEY_PRESSED", "SPACE_KEY_RELEASED");

  engine.setContinuousEvent("SPACE_KEY_RELEASED", "STOP_UNCHARGING");
  auto chargingBarEntityLayer1 = engine.createEntity();
  auto isChargingBarComponent = std::make_shared<IsChargingBar>();
  engine.bindComponentToEntity(chargingBarEntityLayer1, isChargingBarComponent);
  auto spritecompoennt5 = std::make_shared<RenderEngine::SpriteComponent>(
      "assets/HUD/BlueBar.png", Utils::Vect2(752, 1028),
      Utils::rect(0, 0, 0, 26), 100, 2.0f, rotation, tint);
  engine.bindComponentToEntity(chargingBarEntityLayer1, spritecompoennt5);

  auto chargingBarEntityLayer2 = engine.createEntity();
  auto spritecompoennt6 = std::make_shared<RenderEngine::SpriteComponent>(
      "assets/HUD/EmptyBar.png", Utils::Vect2(752, 1028),
      Utils::rect(0, 0, 208, 26), 99, 2.0f, rotation, tint);
  engine.bindComponentToEntity(chargingBarEntityLayer2, spritecompoennt6);

   auto buttonTest = engine.createEntity();
  auto button = std::make_shared<RenderEngine::ButtonComponent>(
      "assets/HUD/EmptyBar.png", Utils::Vect2(0, 0),
      Utils::rect(0, 0, 208, 26), 99, 2.0f, rotation, tint);
  engine.bindComponentToEntity(buttonTest, button);

  auto chargingBar = std::make_shared<ChargingBar>();

  engine.addEvent("SPACE_KEY_PRESSED", chargingBar);
  engine.addEvent("SPACE_KEY_RELEASED", chargingBar);

Utils::Vect2 pos;
  pos.x = 100;
  pos.y = 100;

  Utils::rect rect1;
  rect1.w = 144;
  rect1.h = 59;
  rect1.x = 0;
  rect1.y = 0;
  Utils::ColorR color;
  color.r = 0;
  color.g = 0;
  color.b = 255;
  color.a = 255;

  auto emptyHealthBarEntity = engine.createEntity();
  auto spritecompoennt7 = std::make_shared<RenderEngine::SpriteComponent>("assets/HUD/HealthBar.png", Utils::Vect2(0,1040), Utils::rect(0, 0, 24, 10), 99, 4.0f, rotation, tint);
  engine.bindComponentToEntity(emptyHealthBarEntity, spritecompoennt7);

  auto healthBarEntity = engine.createEntity();
  auto spritecompoennt8 = std::make_shared<RenderEngine::SpriteComponent>("assets/HUD/FullHealthBar.png", Utils::Vect2(0,1040), Utils::rect(0, 0, 24, 10), 100, 4.0f, rotation, tint);
  engine.bindComponentToEntity(healthBarEntity, spritecompoennt8);
  auto isHealthBarComponent = std::make_shared<isHealthBar>();
  engine.bindComponentToEntity(healthBarEntity, isHealthBarComponent);


  auto healthSystem = std::make_shared<RemoveHealth>();
  engine.addEvent("DAMAGE", healthSystem);
  auto scoreEntity = engine.createEntity();
    auto scoreComponent = std::make_shared<Score>();
    engine.bindComponentToEntity(scoreEntity, scoreComponent);
    auto scoreTextComponent = std::make_shared<RenderEngine::TextComponent>("Score: 0", Utils::Vect2(800, 0), 64, 100, Utils::ColorR{255, 255, 255, 255});
    engine.bindComponentToEntity(scoreEntity, scoreTextComponent);

    auto updateScore = std::make_shared<UpdateScore>();

    engine.addEvent("UpdateScore", updateScore);

    engine.scheduleEvent("UpdateScore", 30, 70, 10);

    engine.scheduleEvent("UpdateScore", 60, 100, 10);

    engine.unscheduleEvent("UpdateScore", 100);


  //   GameEngineUtils::Vect2 pos;
  //   pos.x = 100;
  //   pos.y = 100;

  //   GameEngineUtils::rect rect1;
  //   rect1.w = 144;
  //   rect1.h = 59;
  //   rect1.x = 0;
  //   rect1.y = 0;
  //   GameEngineUtils::ColorR color;
  //   color.r = 0;
  //   color.g = 0;
  //   color.b = 255;
  //   color.a = 255;

  //   auto Player = engine.createEntity();
  //   auto spritecompoennt = std::make_shared<RenderEngine::SpriteComponent>(
  //       "assets/spaceship.png", pos, rect1, 4, scale, rotation, tint);
  //   auto isPLayerComponent = std::make_shared<IsPlayer>();
  //   auto movementComponent =
  //   std::make_shared<GameEngine::MovementComponent>(); auto
  //   positionComponent =
  //   std::make_shared<GameEngine::PositionComponent2D>(
  //       GameEngineUtils::Vect2(pos.x, pos.y));
  //   auto velocity =
  //       std::make_shared<GameEngine::VelocityComponent>(GameEngineUtils::Vect2(0,
  //       0));
  //   engine.bindComponentToEntity(Player, spritecompoennt);
  //   engine.bindComponentToEntity(Player, isPLayerComponent);
  //   engine.bindComponentToEntity(Player, movementComponent);
  //   engine.bindComponentToEntity(Player, positionComponent);
  //   engine.bindComponentToEntity(Player, velocity);

  auto mobDeath = std::make_shared<AnimateDeath>();
  engine.addEvent("MobDeath", mobDeath);

  auto createPlayer = std::make_shared<CreatePlayer>();
  engine.addEvent("createPlayer", createPlayer);
  engine.queueEvent("createPlayer");

  auto spawnMob = std::make_shared<SpawnMob>("config/map");
  engine.addSystem("spawnMob", spawnMob);
  //engine.queueEvent("spawnMob", 1);

  auto updateSprite = std::make_shared<updateEntitySprite>();
  engine.addEvent("animate", updateSprite);

  auto wigglePata = std::make_shared<WiggleMob>();
  engine.addSystem("wiggleMob", wigglePata);

  engine.addEvent("CONTROL_KEY_PRESSED", testInput);
  engine.addEvent("ENTER_KEY_PRESSED", testInput);
  engine.addEvent("ForcePodSpawn", forcePod);
  engine.addEvent("ForcePodStop", forcePod);
  engine.addEvent("ForcePodFix", forcePod);
  engine.addSystem("deleteShoot", deleteShoot);

  auto collisionHandler = std::make_shared<CollisionHandler>();

  engine.addEvent("Collision", collisionHandler);
}


int main() {
  GameEngine::GameEngine engine;
  engine.bindSceneInitiation("Scene1", initScene);
  std::string sceneName = "Scene1";
  engine.queueEvent("gameEngineChangeScene", sceneName);
  engine.run();

  return 0;
}


