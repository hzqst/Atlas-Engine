#ifndef AE_GEOMETRYHELPER_H
#define AE_GEOMETRYHELPER_H

#include "../../System.h"
#include "buffer/VertexArray.h"

namespace Atlas {

	namespace Renderer {

		namespace Helper {

			class GeometryHelper {

			public:
				static void GenerateRectangleVertexArray(Buffer::VertexArray& vertexArray);

				static void GenerateCubeVertexArray(Buffer::VertexArray& vertexArray);

				static void GenerateSphereVertexArray(Buffer::VertexArray& vertexArray, uint32_t rings, uint32_t segments);

			private:
				static void GenerateSphere(uint32_t rings, uint32_t segments, uint32_t*& indices, vec3*& vertices,
										   uint32_t* indexCount, uint32_t* vertexCount);

				static int8_t rectangleVertices[];

				static float cubeVertices[];

			};

		}

	}

}

#endif