#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>

#include "GameEngine/GameObject.hpp"

namespace Skyroads {
	namespace Constants {
		const std::vector<std::string> objectTypes { "ball", "platform_red", "platform_green", "platform_yellow", "platform_orange", "platform_purple" };
		const std::vector<std::string> shaderNames { "Base" };
		const std::vector<std::string> meshNames { "box", "sphere" };

		const std::vector<float> lanesX{ -1.5f, 0.f, 1.5f };
		
		const glm::vec3 lightPosition = glm::vec3(0., 2.75f, 0.);
	}

	class GameManager : public SimpleScene
	{
	public:
		GameManager();
		~GameManager();
		void Init() override;

		void addGameObject(GameEngine::GameObject object);
		std::vector<GameEngine::GameObject>* getGameObjects(const std::string type);

	private:
		std::unordered_map<std::string, std::vector<GameEngine::GameObject>> gameObjects;

		/// <summary>
		/// Stores pointers to all gameObjects, in an ordered manner
		/// </summary>
		std::vector<GameEngine::GameObject*> gameObjectsArray;

		void LoadShader(std::string name);
		void LoadMesh(std::string name);

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;

	};
}
