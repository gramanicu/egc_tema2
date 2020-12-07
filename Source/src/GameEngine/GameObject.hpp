#pragma once

#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include "CollisionManager.hpp"

namespace Skyroads {
	class GameObject
	{
	private:
		static int currentMaxID;
		int id;

		glm::vec3 position;
		glm::vec3 scale;

		Mesh *mesh;
		Shader *shader;

		/// <summary>
		/// Get the collider of the object
		/// </summary>
		/// <returns>The collider</returns>
		virtual GameEngine::Collider getCollider() = 0;

		/// <summary>
		/// Find all existing collisions between this object and the specified objects
		/// <param name="gameObjects">All the objects to be checked (this object is automatically ignored)</param>
		/// <returns>An array with the id's of all the objects this object is colliding with</returns>
		virtual std::vector<int> CollisionCheck(std::vector<GameObject*> gameObjects);
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

		/// <summary>
		/// Manage the collisions between this object and all the other game objects
		/// </summary>
		/// <param name="gameObjects">All the gameobjects (as an array)</param>
		virtual void ManageCollisions(std::vector<GameObject*> gameObjects);
	};
}

