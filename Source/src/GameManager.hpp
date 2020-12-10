#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>

#include <Component/SimpleScene.h>
#include "GameEngine/GameObject.hpp"
#include "GameEngine/Camera.hpp"

namespace Skyroads {
	namespace Constants {
		const std::vector<std::string> objectTypes { "ball", "platform_red", "platform_green", "platform_yellow", "platform_orange", "platform_purple", "platform_blue" };
		const std::vector<std::string> shaderNames { "Base" };
		const std::vector<std::string> meshNames { "box", "sphere" };

		const std::vector<float> lanesX{ -1.5f, 0.f, 1.5f };
	}

	class GameManager : public SimpleScene
	{
	public:
		GameManager();
		~GameManager();
		void Init() override;
		
		void addGameObject(GameEngine::GameObject object);

		/// <summary>
		/// Get the game object with a specific id
		/// </summary>
		/// <param name="id">The id of the game object</param>
		/// <returns>The game object</returns>
		GameEngine::GameObject* getGameObject(const long int id);

	private:
		/// <summary>
		/// A map of all the gameobjects, using the object id as a key
		/// </summary>
		std::unordered_map<long int, GameEngine::GameObject> gameObjects;

		/// <summary>
		/// The position offset of the scene light
		/// </summary>
		glm::vec3 lightPositionOffset = glm::vec3(0., 2.75f, 0.);

		/// <summary>
		/// The camera field of view
		/// </summary>
		float cameraFOV = 75.f;

		/// <summary>
		/// If the camera is in 3rd person
		/// </summary>
		bool cameraMode = true;

		/// <summary>
		/// The rotation of the camera
		/// </summary>
		glm::vec2 cameraRot = glm::vec2(0);

		GameEngine::Camera* camera;

		void LoadShader(std::string name);
		void LoadMesh(std::string name);

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void UpdateCamera();

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	};
}
