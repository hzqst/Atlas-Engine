#ifndef AE_ACTOR_H
#define AE_ACTOR_H

#include "../System.h"
#include "../common/AABB.h"

namespace Atlas {

    namespace Actor {

        class Actor {

        public:
			Actor() {}
            /**
             * Virtual destructor of the actor object.
             */
            virtual ~Actor() {}

            /**
             * Sets the transformation matrix of an actor relative to its parent
             * @param matrix A 4x4 matrix
             */
            inline void SetMatrix(mat4 matrix) { this->matrix = matrix; matrixChanged = true; };

            /**
             * Returns the transformation matrix relative to its parent
             * @return A 4x4 matrix
             */
            inline mat4 GetMatrix() { return matrix; }

			inline bool HasMatrixChanged() { return matrixChanged; }

            /**
             * Updates the axis-aligned bounding box and transformedMatrix according to it's
             * parent matrix. (Only if update is needed => parentUpdate && matrixChanged)
             * @param deltaTime
             * @param parentTransform
             * @param parentUpdate
             */
            virtual void Update(float deltaTime, mat4 parentTransform, bool parentUpdate) = 0;

            Common::AABB aabb;
            mat4 transformedMatrix;

            bool matrixChanged = false;

        private:
			mat4 matrix;



        };

    }

}


#endif