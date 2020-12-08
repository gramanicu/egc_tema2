#pragma once

#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include "Physics.hpp"
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
		RigidBody rigidbody;
		Collider *collider;
		glm::vec3 color;
		Data::lightingData lightingInfo;

		/// <summary>
		/// Find all existing collisions between this object and the specified objects
		/// <param name="gameObjects">All the objects to be checked (this object is automatically ignored)</param>
		/// <returns>An array with the id's of all the objects this object is colliding with</returns>
		std::vector<int> CollisionCheck(std::vector<GameObject*> gameObjects);
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
		void Render(EngineComponents::Camera* camera, const glm::vec3& lightLocation);

		/// <summary>
		/// Manage the collisions between this object and all the other game objects
		/// </summary>
		/// <param name="gameObjects">All the gameobjects (as an array)</param>
		void ManageCollisions(std::vector<GameObject*> gameObjects);

		/// <summary>
		/// Get the type of the game object
		/// </summary>
		/// <returns>The type</returns>
		std::string getType() const;

		/// <summary>
		/// Set the type of the object
		/// </summary>
		/// <param name="newType">The new type</param>
		void setType(const std::string newType);

		/// <summary>
		/// Makes the physics computations to update things like position, velocity, acceleration, etc.
		/// </summary>
		/// <param name="deltaTime">The delta time of the frame</param>
		void UpdatePhysics(const double deltaTime);

		/// <summary>
		/// Turns the physics simulations on for the object
		/// </summary>
		void EnablePhysics();

		/// <summary>
		/// Turns the physics simulations off for the object
		/// </summary>
		void DisablePhysics();

		/// <summary>
		/// Set the movement type for the rigidbody
		/// Simulated - use physics to determine the position
		/// Function - the movement is based on a time-dependant function
		/// </summary>
		/// <param name="type">The type of movement</param>
		void MovementType(PhysicsConstants::Motion_Type type);

		/// <summary>
		/// Set the function that will be used for the function based movement
		/// </summary>
		/// <param name="f">The function that will be used. It will have a state, the total time and deltaTime as parameters</param>
		void MovementFunction(void (*f)(State& state, double time, double dt));

		/// <summary>
		/// Returns a reference to the rigidbody of the object
		/// </summary>
		/// <returns></returns>
		RigidBody& getRigidBody();
	};
}

