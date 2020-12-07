#pragma once

#include <Core/Engine.h>

namespace GameEngine {
    /// <summary>
    /// Base collider class
    /// </summary>
    class Collider {
    protected:
        int gameObjectID;
        glm::vec3 position;

    public:
        //virtual ~Collider() = 0;            // Force this class to be an abstract one
        glm::vec3 getPosition() const;
        void setPosition(const glm::vec3& pos);
    };

	/// <summary>
	/// Box collider class
	/// </summary>
	class BoxCollider : public Collider {
	private:
		glm::vec3 dimensions;

	public:
        /// <summary>
        /// Create a new box collider, with a specific dimensions, linked to a gameObject with a specific id
        /// </summary>
        /// <param name="id">The id of the gameObject</param>
        /// <param name="position">The position of the box collider</param>
        /// <param name="dimension">The dimensions of the collider</param>
        BoxCollider(const int id, const glm::vec3& position, const glm::vec3& dimensions);

		/// <summary>
		/// Get the dimensions/scale of the collider
		/// </summary>
		/// <returns>The dimensions (width, height, length)</returns>
		glm::vec3 getDimensions() const;

		/// <summary>
		/// Set the dimensions/scale of the collider
		/// (width, height, length)
		/// </summary>
		/// <param name="dim">The new dimensions</param>
		void setDimensions(const glm::vec3& dim);
	};

    /// <summary>
    /// Sphere collider class
    /// </summary>
    class SphereCollider : public Collider {
    private:
        double radius;

    public:
        /// <summary>
        /// Create a new sphere collider, with a specific radius, linked to a gameObject with a specific id
        /// </summary>
        /// <param name="id">The id of the gameObject</param>
        /// <param name="position">The position of the sphere collider</param>
        /// <param name="radius">The radius of the sphere</param>
        SphereCollider(const int id, const glm::vec3& position ,const double radius);

        /// <summary>
        /// Get the radius of the sphere
        /// </summary>
        /// <returns>The radius</returns>
        double getRadius() const;

        /// <summary>
        /// Set the radius of the sphere
        /// </summary>
        /// <param name="rad">The new radius</param>
        void setRadius(const double radius);
    };
}