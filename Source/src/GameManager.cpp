#include "GameManager.hpp"

#include <vector>
#include <queue>
#include <math.h>

using namespace Skyroads;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	camera = new GameEngine::Camera();
	camera->Set(glm::vec3(0, 5.f, 30.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	camera->distanceToTarget = 3.5f;
	camera->projectionMatrix = glm::perspective(RADIANS(cameraFOV), window->props.aspectRatio, 0.01f, 200.f);

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
		GameObject player("player", glm::vec3(0, 5.f, 25.f));
		player.getRigidBody().state.drag_coef = 0;
		addGameObject(player);
	}

	// Add the first platform under the player
	{
		GameObject platform4("platform_blue", glm::vec3(-3.f, -0.125, 0));
		addGameObject(platform4);
		GameObject platform1("platform_red", glm::vec3(Constants::lanesX[0], -0.125, 0));
		addGameObject(platform1);
		GameObject platform2("platform_green", glm::vec3(Constants::lanesX[1], -0.125, 0));
		addGameObject(platform2);
		GameObject platform3("platform_yellow", glm::vec3(Constants::lanesX[2], -0.125, 0));
		addGameObject(platform3);
		GameObject platform5("platform_orange", glm::vec3(3.f, -0.125, 0));
		addGameObject(platform5);
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

void Skyroads::GameManager::LoadShader(std::string name)
{
	std::string shaderPath = "Source/src/Shaders/";
	Shader* shader = new Shader(name.c_str());
	shader->AddShader(shaderPath + name + ".VS.glsl", GL_VERTEX_SHADER);
	shader->AddShader(shaderPath + name + ".FS.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}

void Skyroads::GameManager::LoadMesh(std::string name)
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
	// Update Camera
	if (cameraMode) {
		// 3rd Person
		camera->Set(gameObjects[0].getRigidBody().state.x + glm::vec3(0.f, .5f, 3.5f), gameObjects[0].getRigidBody().state.x - glm::vec3(0, 1, 100), glm::vec3(0, 1, 0));
		gameObjects[0].isRendered(true);
		camera->RotateThirdPerson_OX(cameraRot.x);
		camera->RotateThirdPerson_OY(cameraRot.y);
	}
	else {
		// 1st Person
		camera->Set(gameObjects[0].getRigidBody().state.x, gameObjects[0].getRigidBody().state.x - glm::vec3(0, 1, 100), glm::vec3(0, 1, 0));
		gameObjects[0].isRendered(false);
		camera->RotateFirstPerson_OX(cameraRot.x);
		camera->RotateFirstPerson_OY(cameraRot.y);
	}

	camera->projectionMatrix = glm::perspective(RADIANS(cameraFOV), window->props.aspectRatio, 0.01f, 200.f);
}


void GameManager::Update(float deltaTimeSeconds)
{
	std::vector<GameEngine::GameObject*> gameObjectsVector;
	for (auto& object : gameObjects) {
		gameObjectsVector.push_back(&(object.second));
	}

	// Update Player
	gameObjects[0].getRigidBody().state.x.z -= 0.05;

	// Update Light
	glm::vec3 lightPosition = gameObjects[0].getRigidBody().state.x + lightPositionOffset;

	UpdateCamera();

	// For every gameObject types (type.first = id, type.second = the object)
	for (auto& object : gameObjects) {
		// Update position
		object.second.UpdatePhysics(deltaTimeSeconds);

		// Check collisions
		object.second.ManageCollisions(gameObjectsVector, &gameObjects);

		// Render objects
		object.second.Render(camera, lightPosition);
	};
}

void GameManager::FrameEnd()
{
}

void GameManager::OnInputUpdate(float deltaTime, int mods)
{
}

void GameManager::OnKeyPress(int key, int mods)
{
	switch (key) {
	case GLFW_KEY_KP_ADD: {
		cameraFOV += 5;
	} break;
	case GLFW_KEY_KP_SUBTRACT: {
		cameraFOV -= 5;
	} break;
	case GLFW_KEY_C: {
		cameraMode = !cameraMode;
		cameraRot = glm::vec2(0);
	} break;
	}
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
		cameraRot += glm::vec2(-sensivityOX * deltaY, -sensivityOY * deltaX);

		if (cameraRot.x > xLimit) cameraRot.x = xLimit;
		if (cameraRot.x < -xLimit) cameraRot.x = -xLimit;
		if (cameraRot.y > yLimit) cameraRot.y = yLimit;
		if (cameraRot.y < -yLimit) cameraRot.y = -yLimit;
	}
}
