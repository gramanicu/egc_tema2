#pragma once

#include <Core/Engine.h>
#include <Component/SimpleScene.h>

namespace Skyroads {
	class GameObject
	{
	private:
		glm::vec3 position;
		glm::vec3 scale;
		Mesh *mesh;
		Shader *shader;

	public:
		/// <summary>
		/// Constructor for a GameObject.
		/// </summary>
		/// <param name="position">The position of the object</param>
		/// <param name="scale">The scale of the object</param>
		/// <param name="mesh">The mesh of the object</param>
		/// <param name="shader">The shader used by the object</param>
		GameObject(const glm::vec3& position, const glm::vec3& scale, Mesh *mesh, Shader *shader);

		/// <summary>
		/// Renders the GameObject on the scene.
		/// </summary>
		/// <param name="scene">The scene in which the GameObject should be rendered</param>
		virtual void Render(SimpleScene* scene);
	};
}

