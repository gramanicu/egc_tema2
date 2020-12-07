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
	// Initialise the data structure for the game objects
	for each (auto& name in Constants::objectTypes)
	{
		gameObjects.emplace(name, std::vector<GameEngine::GameObject>());
	}

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
		GameObject player("ball", glm::vec3(0, 0.5f, 0));
		addGameObject(player);
	}

	// Add the first platform under the player
	{
		GameObject platform4("platform_purple", glm::vec3(-3.f, -0.125, 0));
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
	gameObjects[object.getType()].push_back(object);
	gameObjectsArray.push_back(&object);
}

std::vector<GameEngine::GameObject>* GameManager::getGameObjects(const std::string type)
{
	return &(gameObjects[type]);
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
			object->Render(GetSceneCamera(), Constants::lightPosition); 
			object++;
		}
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
}

void GameManager::OnKeyRelease(int key, int mods)
{
}
