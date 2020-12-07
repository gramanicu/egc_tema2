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
	for each (auto& name in Constants::objectTypes)
	{
		gameObjects.emplace(name, std::vector<GameObject>());
	}
}

void GameManager::addGameObject(const std::string type, GameObject& object)
{
	gameObjects[type].push_back(object);
	gameObjectsArray.push_back(&object);
}

std::vector<GameObject>* GameManager::getGameObjects(const std::string type)
{
	return &(gameObjects[type]);
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

void GameManager::Update(float deltaTimeSeconds)
{
	// For every gameObject types (type.first = type, type.second = array of all object of that types)
	for (auto& type : gameObjects) {
		auto& object = std::begin(type.second);
		while (object != std::end(type.second)) {
			// Update position


			// Check collisions
			object->ManageCollisions(gameObjectsArray);

			// Render objects
			object->Render(this); 
		}
	}
}

void GameManager::FrameEnd()
{
}

void GameManager::OnInputUpdate(float deltaTime, int mods)
{
}

void GameManager::OnKeyPress(int key, int mods)
{
}

void GameManager::OnKeyRelease(int key, int mods)
{
}
