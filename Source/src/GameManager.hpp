#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <conio.h>

#include <Component/SimpleScene.h>
#include "GameEngine/GameObject.hpp"
#include "GameEngine/Camera.hpp"

namespace Skyroads {
	namespace Constants {
		const std::vector<std::string> platformTypes{ "platform_red", "platform_green", "platform_yellow", "platform_orange", "platform_purple", "platform_blue", "platform_white" };
		const std::vector<std::string> shaderNames{ "Base", "UI" };
		const std::vector<std::string> meshNames{ "box", "sphere" };

		const glm::vec3 lightPositionOffset = glm::vec3(0., 2.75f, 0.);
		const glm::vec3 playerStartingPosition = glm::vec3(0, 5.f, 25.f);

		const std::vector<float> lanesX{ -2.5f, 0.f, 2.5f };

		// Player constants
		const float maxSpeed = 0.1f;
		const float minSpeed = 0.0125f;
		const float speedStep = 0.0125f;
		const float lateralSpeed = 2.5f;	// Not continously applied
		const float forcedSpeedTime = 5;	// In seconds
		const float maxLives = 3;
		
		// Fuel constants
		const float maxFuel = 100.f;
		const float fuelGain = 0.33f * maxFuel;
		const float fuelLoss = 0.10f * maxFuel;
		const float fuelFlow = 5.f;										// The "fuelFlow" factor
		const glm::vec3 fuelbarScale = glm::vec3(0.07, 1.9f, 1);		// The maximum scale/size of the fuelbar
		const float fuelbarsDiff = 0.01;

		// Camera constants
		const float minFov = 60.f;
		const float maxFov = 90.f;
	};

	// Defines variables used in the game logic
	struct GameState {
		struct CameraSettings {
			float cameraFOV = 75.f;
			bool cameraMode = true;
			glm::vec2 cameraRotation = glm::vec2(0);
		};
		CameraSettings cameraSettings;

		struct PlayerState {
			float fuel = Constants::maxFuel;
			bool isFullSpeed = false;
			double forcedSpeedStart = 0;	// The start time of the forced speed effect
			float lives = 1;
			float playerSpeed = 0.05f;
			float oldPlayerSpeed = 0.05f;   // The speed of the player before the forced speed effect
		};
		PlayerState playerState;

		float points = 0.f;
	};

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

		GameEngine::Camera* camera;
		GameState gameState;

		void LoadShader(std::string name);
		void LoadMesh(std::string name);

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		/// <summary>
		/// Update the camera data
		/// </summary>
		void UpdateCamera();

		/// <summary>
		/// Update the player data
		/// </summary>
		void UpdatePlayer();

		/// <summary>
		/// Update all the data related to the game logic
		/// </summary>
		void UpdateGameState(const float deltaTime);

		/// <summary>
		/// Render the UI
		/// </summary>
		void RenderUI();

		/// <summary>
		/// Check collisions and update the game state
		/// </summary>
		/// <param name="collided">A vector with the id's of the collided objects</param>
		void CheckCollisions(std::vector<int> collided);

		/// <summary>
		/// Compute the score
		/// </summary>
		void ComputeScore();

		/// <summary>
		/// Function that handles the game end
		/// </summary>
		void GameOver();

		/// <summary>
		/// Spawn/Remove platforms from the game
		/// </summary>
		void PlatformManagement();

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	};
}
