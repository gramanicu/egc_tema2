#include "GameObject.hpp"

using namespace Skyroads;

Skyroads::GameObject::GameObject(std::string name) : name(name)
{
}

GameObject::GameObject(const glm::vec3& position, const glm::vec3& scale, Mesh *mesh, Shader *shader) : position(position), scale(scale), mesh(mesh), shader(shader) {};

void GameObject::Render(SimpleScene *scene)
{
	glm::mat4 matrix = glm::mat4(1);

	matrix = glm::translate(matrix, position);
	matrix = glm::scale(matrix, scale);

	scene->RenderMesh(mesh, shader, matrix);
}
