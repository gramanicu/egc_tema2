#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>

#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include "GameObjects/GameObject.hpp"

namespace Skyroads {
	namespace Constants {
		const std::vector<std::string> objectTypes{ "ball", "platforms" };
	}

	class GameManager : public SimpleScene
	{
	public:
		GameManager();
		~GameManager();
		void Init() override;

		void addGameObject(const std::string type, const GameObject& object);
		std::vector<GameObject>* getGameObjects(const std::string type);

	private:
		std::unordered_map<std::string, std::vector<GameObject>> gameObjects;

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
	};
}
