//
// Created by Theophilus Homawoo on 19/09/2023.
//

#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <optional>
#include <any>
#include "ISystem.hpp"
#include "EventHandler.hpp"
#include "IComponent.hpp"


namespace GameEngine {
    class Registry {
        public:
            Registry() = default;
            ~Registry() = default;

            std::shared_ptr<EventHandler> getEventHandler() {
                return eventHandler;
            }

            void bindSceneInitiation(std::string sceneName, std::function<void(Registry&)> sceneInitiation) {
                sceneMap[sceneName] = sceneInitiation;
            }
            void changeScene(std::string sceneName) {
                for (auto& componentTypePair : componentsContainer) {
                    componentTypePair.second.clear();
                }
                freeMemorySlots.clear();

                auto it = sceneMap.find(sceneName);
                if (it != sceneMap.end()) {
                    it->second(*this);
                } else {
                    std::cerr << "Error: Scene '" << sceneName << "' not found!" << std::endl; // NE PAS OUBLIER LES CUSTOMS ERROS
                }
            }

            std::vector<std::optional<IComponent>> getComponents(size_t componentType) {
                return componentsContainer[componentType];
            }
            std::optional<IComponent> getComponent(size_t entityID, size_t componentType) {
                return componentsContainer[componentType][entityID];
            }
            std::vector<size_t> getEntitiesWithComponent(size_t componentType) {
                std::vector<size_t> entities;
                for (size_t i = 0; i < componentsContainer[componentType].size(); i++) {
                    if (componentsContainer[componentType][i].has_value()) {
                        entities.push_back(i);
                    }
                }
                return entities;
            }
            std::vector<std::optional<IComponent>> getComponentsFromEntity(size_t entityID) {
                std::vector<std::optional<IComponent>> components;
                for (auto componentType : componentsContainer) {
                    components.push_back(componentType.second[entityID]);
                }
                return components;
            }

            void bindComponentToEntity(size_t entityID, size_t componentType, std::optional<IComponent> component) {
                if(entityID >= componentsContainer[componentType].size()) {
                    componentsContainer[componentType].resize(entityID + 1);
                }
                componentsContainer[componentType][entityID] = component;
            }
            void unbindComponentFromEntity(size_t entityID, size_t componentType) {
                if(entityID < componentsContainer[componentType].size()) {
                    componentsContainer[componentType][entityID] = std::nullopt;

                    while (!componentsContainer[componentType].empty() &&
                           !componentsContainer[componentType].back().has_value()) {
                        componentsContainer[componentType].pop_back();
                    }
                }
            }

            void deleteEntity(size_t entityID) {
                freeMemorySlots.push_back(entityID);

                for (auto& [componentType, components] : componentsContainer) {
                    if (entityID < components.size()) {
                        components[entityID] = std::nullopt;
                    }
                }
            }
            size_t createEntity() {
                size_t entityID;
                if (!freeMemorySlots.empty()) {
                    entityID = freeMemorySlots.back();
                    freeMemorySlots.pop_back();
                } else {
                    if (!componentsContainer.empty()) {
                        entityID = componentsContainer.begin()->second.size();
                    } else {
                        entityID = 0;
                    }
                }
                return entityID;

            }
            size_t createEntity(std::vector<std::optional<IComponent>> components) {
                size_t entityID = createEntity();
                for (size_t i = 0; i < components.size(); i++) {
                    bindComponentToEntity(entityID, i, components[i]);
                }
                return entityID;
            }

            void addSystem(std::string systemName, std::shared_ptr<ISystem> system) {
                systemMap[systemName] = {system, 1};
            }
            void addSystem(std::string systemName, std::string systemPath) {
                SharedLibrary lib(systemPath);
                std::shared_ptr<ISystem> system = lib.getFunction<ISystem*()>("createSystem")();
                systemMap[systemName] = {system, 1};
            }
            void addSystem(std::string systemName, std::shared_ptr<ISystem> system, int priority) {
                systemMap[systemName] = {system, priority};
            }
            void addSystem(std::string systemName, std::string systemPath, int priority) {
                SharedLibrary lib(systemPath);
                std::shared_ptr<ISystem> system = lib.getFunction<ISystem*()>("createSystem")();
                systemMap[systemName] = {system, priority};
            }
            void deleteSystem(std::string systemName) {
                systemMap.erase(systemName);
            }

            void updateSystems() {
                std::vector<std::pair<std::string, std::pair<std::shared_ptr<ISystem>, int>>> sortedSystems(systemMap.begin(), systemMap.end());

                std::sort(sortedSystems.begin(), sortedSystems.end(), [](const auto& a, const auto& b) {
                    return a.second.second < b.second.second; // Sort by priority values
                });

                for (auto& [name, systemPair] : sortedSystems) {
                    systemPair.first->update();
                }
            }

        private:
            std::vector<size_t> freeMemorySlots;
            std::unordered_map<size_t, std::vector<std::optional<IComponent>>> componentsContainer;
            std::map<std::string, std::pair<std::shared_ptr<ISystem>, int>> systemMap;
            std::unordered_map<std::string, std::function<void(Registry&)>> sceneMap;
            std::unique_ptr<EventHandler> eventHandler;
    };
} // namespace GameEngine
