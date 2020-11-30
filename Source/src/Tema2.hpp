#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <Core/Engine.h>
#include <Component/SimpleScene.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

	// Exposes the scene mesh list
	void AddMeshToScene(Mesh *mesh);

	// Exposes the render mesh method
	void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat3 &modelMatrix);

	std::unordered_map<std::string, Mesh *> &getMeshes();
	std::unordered_map<std::string, Shader *> &getShaders();

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
};
