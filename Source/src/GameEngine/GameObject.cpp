#include "GameObject.hpp"

using namespace Skyroads;

int GameObject::currentMaxID = 0;

GameObject::GameObject(const glm::vec3& position, const glm::vec3& scale, Mesh *mesh, Shader *shader) : position(position), scale(scale), mesh(mesh), shader(shader) {
	id = currentMaxID++;
};

void GameObject::Render(SimpleScene *scene)
{
	glm::mat4 matrix = glm::mat4(1);

	matrix = glm::translate(matrix, position);
	matrix = glm::scale(matrix, scale);

	scene->RenderMesh(mesh, shader, matrix);
}

void Skyroads::GameObject::ManageCollisions(std::vector<GameObject*> gameObjects){}

std::vector<int> Skyroads::GameObject::CollisionCheck(std::vector<GameObject*> gameObjects)
{
	using namespace GameEngine;
	std::vector<Collider> colArray;

	for (auto& obj : gameObjects) {
		// Do not include this object in the search
		if (obj->id != id) {
			colArray.push_back(obj->getCollider());
		}
	}

	return CollisionManager::getCollisions(this->getCollider(), colArray);
}
