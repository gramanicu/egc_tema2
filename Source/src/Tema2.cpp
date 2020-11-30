#include "Tema2.hpp"
#include <vector>
#include <queue>
#include <math.h>

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	srand(time(NULL));

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
}

void Tema1::AddMeshToScene(Mesh *mesh)
{
	AddMeshToList(mesh);
}

void Tema1::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat3 &modelMatrix)
{
	this->RenderMesh2D(mesh, shader, modelMatrix);
}

std::unordered_map<std::string, Mesh *> &Tema1::getMeshes()
{
	return this->meshes;
}

std::unordered_map<std::string, Shader *> &Tema1::getShaders()
{
	return this->shaders;
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
	
}

void Tema1::OnKeyRelease(int key, int mods)
{
	
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
