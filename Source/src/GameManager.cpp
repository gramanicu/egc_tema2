#include "GameManager.hpp"

#include <vector>
#include <queue>
#include <math.h>

/// <summary>
/// Map a value that is in a range to another range
/// </summary>
/// <param name="sourceNumber">The value</param>
/// <param name="fromA">Starting value of the first range</param>
/// <param name="fromB">End value of the first range</param>
/// <param name="toA">Starting value of the second range</param>
/// <param name="toB">End value of the second range</param>
/// <param name="decimalPrecision">The number of decimals to use</param>
/// <returns>The mapped value</returns>
double mapBetweenRanges(double sourceNumber, double fromA, double fromB, double toA, double toB, int decimalPrecision) {
	double deltaA = fromB - fromA;
	double deltaB = toB - toA;
	double scale = deltaB / deltaA;
	double negA = -1 * fromA;
	double offset = (negA * scale) + toA;
	double finalNumber = (sourceNumber * scale) + offset;
	int calcScale = (int)pow(10, decimalPrecision);
	return (double)round(finalNumber * calcScale) / calcScale;
}

using namespace Skyroads;

GameManager::GameManager()
{
	camera = new GameEngine::Camera();
	camera->Set(glm::vec3(0, 5.f, 30.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	camera->distanceToTarget = 3.5f;
	camera->projectionMatrix = glm::perspective(RADIANS(gameState.cameraSettings.cameraFOV), window->props.aspectRatio, 0.01f, 200.f);
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	// Load meshes
	for each (auto & name in Constants::meshNames) {
		LoadMesh(name);
	}

	// Load shaders
	for each (auto & name in Constants::shaderNames) {
		LoadShader(name);
	}

	using namespace GameEngine;
	// Link the meshes and shaders to the game objects
	GameObject::meshes = &meshes;
	GameObject::shaders = &shaders;
	
	// Initialize the player object
	{
		GameObject player("player", glm::vec3(Constants::playerStartingPosition));
		player.getRigidBody().state.drag_coef = 10.f;
		player.isInJump = true;
		addGameObject(player);
	}

	// Add the first platform under the player
	{
		GameObject platform1("platform_green", glm::vec3(Constants::lanesX[0], -0.125, 0));
		addGameObject(platform1);
		GameObject platform2("platform_white", glm::vec3(Constants::lanesX[1], -0.125, 0));
		addGameObject(platform2);
		GameObject platform3("platform_yellow", glm::vec3(Constants::lanesX[2], -0.125, 0));
		addGameObject(platform3);
	}
}

void GameManager::addGameObject(GameEngine::GameObject object)
{
	gameObjects[object.getID()] = object;
}

GameEngine::GameObject* GameManager::getGameObject(const long int id)
{
	return &(gameObjects[id]);
}

void GameManager::LoadShader(std::string name)
{
	std::string shaderPath = "Source/src/Shaders/";
	Shader* shader = new Shader(name.c_str());
	shader->AddShader(shaderPath + name + ".VS.glsl", GL_VERTEX_SHADER);
	shader->AddShader(shaderPath + name + ".FS.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}

void GameManager::LoadMesh(std::string name)
{
	std::string meshPath = "Source/src/Meshes/";
	Mesh* mesh = new Mesh(name.c_str());
	mesh->LoadMesh(meshPath, name + ".obj");
	meshes[mesh->GetMeshID()] = mesh;
}

void GameManager::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void GameManager::UpdateCamera() {
	// Update camera mode and position
	if (gameState.cameraSettings.cameraMode) {
		// 3rd Person
		camera->Set(gameObjects[0].getRigidBody().state.x + glm::vec3(0.f, .5f, 3.5f), gameObjects[0].getRigidBody().state.x - glm::vec3(0, 1, 100), glm::vec3(0, 1, 0));
		gameObjects[0].isRendered(true);
		camera->RotateThirdPerson_OX(gameState.cameraSettings.cameraRotation.x);
		camera->RotateThirdPerson_OY(gameState.cameraSettings.cameraRotation.y);
	}
	else {
		// 1st Person
		camera->Set(gameObjects[0].getRigidBody().state.x, gameObjects[0].getRigidBody().state.x - glm::vec3(0, 1, 100), glm::vec3(0, 1, 0));
		gameObjects[0].isRendered(false);
		camera->RotateFirstPerson_OX(gameState.cameraSettings.cameraRotation.x);
		camera->RotateFirstPerson_OY(gameState.cameraSettings.cameraRotation.y);
	}

	// Map the camera FOV to the player speed
	gameState.cameraSettings.cameraFOV = (float)mapBetweenRanges(gameState.playerState.playerSpeed, Constants::minSpeed, Constants::maxSpeed, Constants::minFov, Constants::maxFov, 1);

	// Update the projection matrix
	camera->projectionMatrix = glm::perspective(RADIANS(gameState.cameraSettings.cameraFOV), window->props.aspectRatio, 0.01f, 200.f);
}

void GameManager::UpdatePlayer()
{
	// Move the player forward
	gameObjects[0].getRigidBody().state.x.z -= gameState.playerState.playerSpeed;

	if (window->KeyHold(GLFW_KEY_A)) {
		// Move player left
		gameObjects[0].getRigidBody().state.v.x = -Constants::lateralSpeed;
	}
	else if (window->KeyHold(GLFW_KEY_D)) {
		// Move player right
		gameObjects[0].getRigidBody().state.v.x = Constants::lateralSpeed;
	}
}

void GameManager::UpdateGameState(const float deltaTime)
{
	// Update Player
	UpdatePlayer();

	// Update camera
	UpdateCamera();

	// Compute the current score
	ComputeScore();

	// Check fuel state
	gameState.playerState.fuel -= deltaTime * Constants::fuelFlow;
	if (gameState.playerState.fuel <= 0) {
		// If all the fuel was used, a life is lost. If the game can go on
		// (at least 1 life remaining), reset the fuel to max

		gameState.playerState.lives--;
		if (gameState.playerState.lives > 0) {
			gameState.playerState.fuel = Constants::maxFuel;
		}
	}

	// Check lives
	if(gameState.playerState.lives <= 0) {
		// If all lives are lost, game over
		GameOver();
	}

	// Check if full speed should still be applied
	if (gameState.playerState.isFullSpeed && Engine::GetElapsedTime() - gameState.playerState.forcedSpeedStart >= Constants::forcedSpeedTime) {
		gameState.playerState.isFullSpeed = false;
		gameState.playerState.playerSpeed = gameState.playerState.oldPlayerSpeed;
	}
}

void GameManager::Update(float deltaTimeSeconds)
{
	std::vector<GameEngine::GameObject*> gameObjectsVector;
	for (auto& object : gameObjects) {
		gameObjectsVector.push_back(&(object.second));
	}

	UpdateGameState(deltaTimeSeconds);

	// Update Light
	glm::vec3 lightPosition = gameObjects[0].getRigidBody().state.x + Constants::lightPositionOffset;
	// For every gameObject types (type.first = id, type.second = the object)
	for (auto& object : gameObjects) {
		// Update position
		object.second.UpdatePhysics(deltaTimeSeconds);

		// Check collisions
		CheckCollisions(object.second.ManageCollisions(gameObjectsVector, &gameObjects));

		// Render objects
		object.second.Render(camera, lightPosition);
	};
}

void GameManager::FrameEnd()
{
}

void Skyroads::GameManager::CheckCollisions(std::vector<int> collided)
{
	if (collided.size() == 0) return;

	for (int id : collided) {
		std::string type = gameObjects[id].getType();

		// Make sure this is a platform
		if (type.rfind("platform_", 0) != 0) return;

		std::string color_string = type.substr(type.find("_") + 1);
		if (color_string == "red") {
			// Instant Loss
			GameOver();
		}
		else if (color_string == "yellow") {
			// Lose fuel
			gameState.playerState.fuel -= Constants::fuelLoss;
		}
		else if (color_string == "orange") {
			// Speed up
			gameState.playerState.isFullSpeed = true;
			gameState.playerState.forcedSpeedStart = Engine::GetElapsedTime();
			gameState.playerState.oldPlayerSpeed = gameState.playerState.playerSpeed;
			gameState.playerState.playerSpeed = Constants::maxSpeed;
		}
		else if (color_string == "green") {
			// Gain fuel
			gameState.playerState.fuel += Constants::fuelGain;
		}
		else if (color_string == "white") {
			if (gameState.playerState.lives < Constants::maxLives) {
				// Gain life
				gameState.playerState.lives += 1;
			}
		}

		gameObjects[id].setType("platform_purple");
	}
}

void Skyroads::GameManager::ComputeScore()
{
	gameState.points = abs(gameObjects[0].getRigidBody().state.x.z - Constants::playerStartingPosition.z);
}

void Skyroads::GameManager::GameOver()
{
	std::cout << " --- Game Over --- " << "\n";
	std::cout << " Your score was : " << (int)gameState.points << "\n";
	std::cout << " Press any key to exit ...\n";
	int aux = _getch();
	exit(0);
}

void GameManager::PlatformManagement()
{
	// This function manages all the platforms, their spawning and removal
	// TODO - a constant for the number of platforms in the scene
	// TODO - platform length
	// The platforms will be randomly spawned, many will be without effects.
	// - red - very few
	// - yellow - some
	// - green - few
	// - orange - few
	// - white - very few
}

void GameManager::OnInputUpdate(float deltaTime, int mods)
{
}

void GameManager::OnKeyPress(int key, int mods)
{
	float pSpeed = gameState.playerState.playerSpeed;

	switch (key) {
	case GLFW_KEY_C: {
		// Change camera modes
		gameState.cameraSettings.cameraMode = !gameState.cameraSettings.cameraMode;
		gameState.cameraSettings.cameraRotation = glm::vec2(0);
	} break;
	case GLFW_KEY_W: {
		if (!gameState.playerState.isFullSpeed) {
			// Speed up
			pSpeed += Constants::speedStep;
			if (pSpeed > Constants::maxSpeed) {
				pSpeed = Constants::maxSpeed;
			}
		}
	} break;
	case GLFW_KEY_S: {
		if (!gameState.playerState.isFullSpeed) {
			// Slow down
			pSpeed -= Constants::speedStep;
			if (pSpeed < Constants::minSpeed) {
				pSpeed = Constants::minSpeed;
			}
		}
	} break;
	case GLFW_KEY_SPACE: {
		// Jump
		if (!gameObjects[0].isInJump) {
			gameObjects[0].isInJump = true;
			gameObjects[0].getRigidBody().state.v.y = 2.f;
		}
	} break;
	}

	gameState.playerState.playerSpeed = pSpeed;
}

void GameManager::OnKeyRelease(int key, int mods)
{
}

void GameManager::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float xLimit = 0.275;
		float yLimit = 0.5;

		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;
		glm::vec2 rotation = gameState.cameraSettings.cameraRotation;

		rotation += glm::vec2(-sensivityOX * deltaY, -sensivityOY * deltaX);

		if (rotation.x > xLimit) rotation.x = xLimit;
		if (rotation.x < -xLimit) rotation.x = -xLimit;
		if (rotation.y > yLimit) rotation.y = yLimit;
		if (rotation.y < -yLimit) rotation.y = -yLimit;

		gameState.cameraSettings.cameraRotation = rotation;
	}
}
