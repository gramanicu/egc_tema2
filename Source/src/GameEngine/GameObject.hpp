#pragma once

#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include "CollisionManager.hpp"

namespace GameEngine {
	namespace Data {
		typedef struct _lightingData {
			float materialShine = 5.f;
			float materialKd = 0.5f;
			float materialKs = 0.5f;
		} lightingData;
	}

	class GameObject
	{
	private:
		static int currentMaxID;

		int id;
		std::string type;

		glm::vec3 position;
		glm::vec3 scale;

		Mesh *mesh;
		Shader *shader;
		Collider collider;
		glm::vec3 color;
		Data::lightingData lightingInfo;

		/// <summary>
		/// Find all existing collisions between this object and the specified objects
		/// <param name="gameObjects">All the objects to be checked (this object is automatically ignored)</param>
		/// <returns>An array with the id's of all the objects this object is colliding with</returns>
		virtual std::vector<int> CollisionCheck(std::vector<GameObject*> gameObjects);
	public:
		static std::unordered_map<std::string, Mesh*>* meshes;
		static std::unordered_map<std::string, Shader*>* shaders;

		/// <summary>
		/// Constructor for a GameObject.
		/// </summary>
		/// <param name="type">The type of the object</param>
		/// <param name="position">The position of the object</param>
		GameObject(const std::string& type, const glm::vec3& position);

		// Copy-Constructor
		GameObject(const GameObject& other);

		/// <summary>
		/// Renders the GameObject on the scene.
		/// </summary>
		/// <param name="camera">The camera used in the scene</param>
		/// <param name="lightLocation">The location of the light</param>
		virtual void Render(EngineComponents::Camera* camera, const glm::vec3& lightLocation);

		/// <summary>
		/// Manage the collisions between this object and all the other game objects
		/// </summary>
		/// <param name="gameObjects">All the gameobjects (as an array)</param>
		virtual void ManageCollisions(std::vector<GameObject*> gameObjects);

		/// <summary>
		/// Get the type of the game object
		/// </summary>
		/// <returns>The type</returns>
		std::string getType() const;
	};
}

