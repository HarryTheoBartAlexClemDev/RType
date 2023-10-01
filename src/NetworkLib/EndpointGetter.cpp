//
// Created by Clément Lagasse on 01/10/2023.
//

#include "EndpointGetter.hpp"
#include "InterfaceNetwork.hpp"

unsigned int Network::EndpointGetter::getIdByEndpoint(const asio::ip::udp::endpoint& endpoint, const std::vector<std::shared_ptr<Network::Interface> >* _clients) {
    if (_clients == nullptr)
        return 0;
    for (auto& client : *_clients) {
        if (client->getEndpoint() == endpoint)
            return client->getId();
    }
    return 0;
}

asio::ip::udp::endpoint Network::EndpointGetter::getEndpointById(unsigned int id, const std::vector<std::shared_ptr<Network::Interface> >* _clients) {
    if (_clients == nullptr)
        return asio::ip::udp::endpoint();
    for (auto& client : *_clients) {
        if (client->getId() == id)
            return client->getEndpoint();
    }
    return {};
}