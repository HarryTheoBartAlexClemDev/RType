//
// Created by alexandre on 04/10/23.
//

#pragma once

#include "ISystem.hpp"
#include "ComponentsType.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent2D.hpp"
#include "RectangleColliderComponent2D.hpp"
#include "AABBComponent2D.hpp"
#include "IsBullet.hpp"
#include "EventHandler.hpp"
#include "VelocityComponent.hpp"
#include "PositionComponent2D.hpp"
#include "PhysicsEngineMovementSystem2D.hpp"
#include "IsForcePod.hpp"
#include "IsPlayer.hpp"
#include "Shooter.hpp"

class ForcePodSpawn : public GameEngine::ISystem {
    void update(GameEngine::ComponentsContainer &componentsContainer,
                GameEngine::EventHandler &eventHandler) override;
public:

private:
  bool done = false;
};