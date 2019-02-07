#ifndef AE_DECAL_H
#define AE_DECAL_H

#include "System.h"
#include "texture/Texture2D.h"

namespace Atlas {

	class Decal {

	public:
		Decal(Texture2D *texture, float rowCount = 1.0f, float columnCount = 1.0f, float animationLength = 1.0f)
				: texture(texture), rowCount(rowCount), columnCount(columnCount), animationLength(animationLength) {}

		Texture2D *texture;

		float rowCount;
		float columnCount;
		float animationLength;

		mat4 matrix;

		vec4 color = vec4(1.0f);

	};

}

#endif