//
// Created by Clément Lagasse on 05/10/2023.
//

#include "NetworkClientConnection.hpp"
#include "UserGameMode.hpp"

namespace Server {

    void NetworkClientConnection::update(GameEngine::ComponentsContainer &componentsContainer, GameEngine::EventHandler &eventHandler)
    {
        Network::TSQueue<unsigned int> &queue = Network::Server::getInstance().getConnectedClients();
        if (queue.empty())
            return;
        unsigned int netInterfaceId = queue.popBack();
        auto &factory = EntityFactory::getInstance();
        Utils::Vect2 pos = {0, 0};
        PlayerNumber nbr = factory.getNextPlayerNumber();
        size_t entityId = 0;
        std::vector<std::any> args = {static_cast<int>(nbr)};
        std::vector<size_t> ids = {entityId};
        std::shared_ptr<Utils::UserGameMode> userGameMode = nullptr;
        if (nbr == PlayerNumber::Spectator)  {
            entityId = componentsContainer.createEntity();
            userGameMode = std::make_shared<Utils::UserGameMode>(Utils::UserGameMode::SPECTATOR);
            args.emplace_back(static_cast<int>(userGameMode->_state));
        } else {
            entityId = factory.createNewPlayer(componentsContainer, eventHandler, pos, nbr);
            userGameMode = std::make_shared<Utils::UserGameMode>(Utils::UserGameMode::PLAYER);
            args.emplace_back(static_cast<int>(userGameMode->_state));
            auto positionType = GameEngine::ComponentsType::getComponentType("PositionComponent2D");
            auto mayComp = componentsContainer.getComponent(entityId, positionType);
            if (!mayComp.has_value())
                return;
            auto comp = std::static_pointer_cast<PhysicsEngine::PositionComponent2D>(mayComp.value());
            args.emplace_back(static_cast<int>(comp->pos.x * 1000));
            args.emplace_back(static_cast<int>(comp->pos.y * 1000));
            args.emplace_back(0);
            args.emplace_back(0);
            std::shared_ptr<Network::Message> message = std::make_shared<Network::Message>("CREATED_USER", ids, "INT", args, true);
            std::shared_ptr<Network::NotUserMessage> notMessage = std::make_shared<Network::NotUserMessage>(netInterfaceId, message);
            eventHandler.queueEvent("SEND_NETWORK", notMessage);
        }
        auto netComp = std::make_shared<NetworkClientId>(netInterfaceId);
        componentsContainer.bindComponentToEntity(entityId, netComp);
        componentsContainer.bindComponentToEntity(entityId, userGameMode);

        std::shared_ptr<Network::Message> message2 = std::make_shared<Network::Message>("ACCEPTED", ids, "INT", args, true);
        std::shared_ptr<Network::UserMessage> userMessage = std::make_shared<Network::UserMessage>(netInterfaceId, message2);
        eventHandler.queueEvent("SEND_NETWORK", userMessage);
        eventHandler.queueEvent("CREATE_WORLD", entityId);
    }
}