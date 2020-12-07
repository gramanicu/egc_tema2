#include "GameObject.hpp"

#include <iostream>

int GameEngine::GameObject::currentMaxID = 0;
std::unordered_map<std::string, Mesh*>* GameEngine::GameObject::meshes = nullptr;
std::unordered_map<std::string, Shader*>* GameEngine::GameObject::shaders = nullptr;

GameEngine::GameObject::GameObject(const std::string& type, const glm::vec3& position) : type(type), position(position), mesh(nullptr), shader(nullptr) {
	id = currentMaxID++;

	if (type == "ball") {
		scale = glm::vec3(1);
		mesh = (*meshes)["sphere"];
		shader = (*shaders)["Base"];
		lightingInfo = { 5.f, 0.5f, .25f };
		color = glm::vec3(1);
	}
	else if (type.rfind("platform_", 0) == 0) {
		scale = glm::vec3(1, 0.25f, 20.f);
		mesh = (*meshes)["box"];
		shader = (*shaders)["Base"];
		lightingInfo = { 0.1f, 0.99f, .001f };

		std::string color_string = type.substr(type.find("_") + 1);
		if (color_string == "red") {
			color = glm::vec3(1, 0, 0);
		}
		else if (color_string == "yellow") {
			color = glm::vec3(1, 1, 0);
		}
		else if (color_string == "orange") {
			color = glm::vec3(0.9, 0.6, 0.2);
		}
		else if (color_string == "green") {
			color = glm::vec3(0, 1, 0);
		}
		else if (color_string == "purple") {
			color = glm::vec3(0.5, 0.1, 0.4);
		}
	}
}

GameEngine::GameObject::GameObject(const GameObject& other)
{
	id = other.id;
	position = other.position;
	type = other.type;
	scale = other.scale;
	mesh = other.mesh;
	shader = other.shader;
	color = other.color;
	lightingInfo = other.lightingInfo;
}

void GameEngine::GameObject::Render(EngineComponents::Camera *camera, const glm::vec3& lightLocation)
{
	if (mesh == nullptr || shader == nullptr) return;

	glm::mat4 matrix = glm::mat4(1);
	matrix = glm::translate(matrix, position);
	matrix = glm::scale(matrix, scale);

	// Render the object
	shader->Use();

	// Bind MVP
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(matrix));
	
	// Bind Light-Data
	glm::vec3 cameraPos = camera->transform->GetWorldPosition();
	glUniform3f(shader->loc_eye_pos, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(glGetUniformLocation(shader->program, "light_position"), lightLocation.x, lightLocation.y, lightLocation.z);

	// Bind Material Data
	glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), (GLfloat)lightingInfo.materialShine);
	glUniform1f(glGetUniformLocation(shader->program, "material_kd"), (GLfloat)lightingInfo.materialKd);
	glUniform1f(glGetUniformLocation(shader->program, "material_ks"), (GLfloat)lightingInfo.materialKs);
	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	mesh->Render();
}

void GameEngine::GameObject::ManageCollisions(std::vector<GameObject*> gameObjects){}

std::string GameEngine::GameObject::getType() const
{
	return type;
}

std::vector<int> GameEngine::GameObject::CollisionCheck(std::vector<GameObject*> gameObjects)
{
	using namespace GameEngine;
	std::vector<Collider> colArray;

	for (auto& obj : gameObjects) {
		// Do not include this object in the search
		if (obj->id != id) {
			colArray.push_back(obj->collider);
		}
	}

	return CollisionManager::getCollisions(this->collider, colArray);
}
