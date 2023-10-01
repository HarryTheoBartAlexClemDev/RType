//
// Created by Clément Lagasse on 28/09/2023.
//

#include <iostream>
#include <typeindex>
#include "Message.hpp"

std::vector <std::uint8_t> Network::Serializer::serialize(const std::vector <std::any> &data)
{
    std::vector<std::uint8_t> result;

    for (const auto& item : data) {
        std::vector<std::uint8_t> serializedItem = serializeItem(item);
        result.insert(result.end(), serializedItem.begin(), serializedItem.end());
    }

    return result;
}

std::vector<std::uint8_t> Network::Serializer::serializeItem(const std::any &item)
{
    std::stringstream ss;

    static const std::map<std::type_index, void(*)(const std::any&, std::stringstream&)> serializers = {
        { typeid(int), [](const std::any& item, std::stringstream& ss) { ss << std::any_cast<int>(item); } },
        { typeid(float), [](const std::any& item, std::stringstream& ss) { ss << std::fixed << std::any_cast<float>(item); } },
        { typeid(std::string), [](const std::any& item, std::stringstream& ss) { ss << std::any_cast<std::string>(item); } },
        { typeid(const std::string&), [](const std::any& item, std::stringstream& ss) { ss << std::any_cast<const std::string&>(item); } },
        { typeid(char), [](const std::any& item, std::stringstream& ss) { ss << std::any_cast<char>(item); } }
    };

    auto it = serializers.find(item.type());
    if (it != serializers.end()) {
        it->second(item, ss);
    } else {
        std::cout << item.type().name() << std::endl;
        throw std::runtime_error("Unsupported data type for serialization check Message.hpp file");
    }

    std::string serialized = ss.str();
    return {serialized.begin(), serialized.end()};
}



Network::Message::Message(std::vector <std::uint8_t> &message): _message(message), _ArgType(), _args()
{
    getDataMessage();
}

Network::Message::Message(const std::string &action, std::vector<unsigned int> IDs, const std::string &typeArg, std::vector<std::any> args)
: _action(action), _ArgType(typeArg), _args(args), _IDs(IDs), _NbrArgs(args.size())
{
    std::vector<std::uint8_t> serializedArgs = Serializer::serialize(_args);
    initializeMessage(IDs, serializedArgs);
}

Network::Message::Message(const std::string &action, std::vector<unsigned int> IDs, const std::string &typeArg, std::any arg)
        : _action(action), _ArgType(typeArg), _args(), _IDs(IDs), _NbrArgs(1), _NbrId(IDs.size())
{
    std::vector<std::uint8_t> serializedArgs = Serializer::serializeItem(arg);
    initializeMessage(IDs, serializedArgs);
}

std::vector<std::uint8_t> &Network::Message::getMessage()
{
    return _message;
}

unsigned int Network::Message::getSize() {
    return _message.size();
}

std::string Network::Message::getActionByCode(uint8_t code)
{
    for (auto& item : actionToCodeMap) {
        if (item.second == code) {
            return item.first;
        }
    }
    throw std::runtime_error("Unknown action code");
}

std::string Network::Message::getTypeByCode(uint8_t code)
{
    for (auto& item : typeToCodeMap) {
        if (item.second == code) {
            return item.first;
        }
    }
    throw std::runtime_error("Unknown type code");
}

uint8_t Network::Message::getCodeByAction(const std::string &action)
{
    for (auto& item : actionToCodeMap) {
        if (item.first == action) {
            return item.second;
        }
    }
    throw std::runtime_error("Unknown action");
}

uint8_t Network::Message::getCodeByType(const std::string &type)
{
    for (auto& item : typeToCodeMap) {
        if (item.first == type) {
            return item.second;
        }
    }
    throw std::runtime_error("Unknown type");
}

uint8_t Network::Message::getSizeByType(uint8_t code)
{
    for (auto& item : typeToSizeMap) {
        if (item.first == code) {
            return item.second;
        }
    }
    throw std::runtime_error("Unknown type");
}

void Network::Message::getDataMessage()
{
    _action = getActionByCode(_message.front());
    memcpy(&_NbrId, _message.data() + 1, sizeof(_NbrId));
    _NbrId = ntohs(_NbrId);
    _IDs.insert(_IDs.end(), _message.begin() + 2, _message.begin() + 2 + _NbrId);
    _ArgTypeCode = _message[3 + _NbrId];
    _NbrArgs = _message[4 + _NbrId];

    _ArgType = getTypeByCode(_ArgTypeCode);
    _sizeArg = getSizeByType(_ArgTypeCode);
    if (_ArgTypeCode == 0x03) {
        _sizeArg = _NbrArgs;
        _NbrArgs = 1;
    }
    _args.resize(_NbrArgs);
    for (int i = 0; i < _NbrArgs; i++) {
        memcpy(&_args[i], &_message[5 + _NbrId + i * _sizeArg], _sizeArg);
    }
}

void Network::Message::initializeMessage(const std::vector<unsigned int>& IDs, const std::vector<std::uint8_t>& serializedArgs)
{
    _message.push_back(actionToCodeMap[_action]);
    _message.push_back(static_cast<uint8_t>(_NbrId >> 8));
    _message.push_back(static_cast<uint8_t>(_NbrId));
    _message.insert(_message.end(), IDs.begin(), IDs.end());
    uint8_t Type = typeToCodeMap[_ArgType];
    if (Type == 0x03)
        _NbrArgs = serializedArgs.size();
    _message.push_back(Type);
    _message.push_back(_NbrArgs);
    _message.insert(_message.end(), serializedArgs.begin(), serializedArgs.end());
}