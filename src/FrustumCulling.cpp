#include "FrustumCulling.h"

namespace Atlas {

	void FrustumCulling::CullActorsFromScene(Scene *scene, Camera *camera) {


	}

	void FrustumCulling::CullLightsFromScene(Scene *scene, Camera *camera) {


	}

	void FrustumCulling::CullActorsFromShadow(ILight *light, Scene *scene, Camera *camera) {

		switch (light->type) {
			case AE_DIRECTIONAL_LIGHT:
				CullActorsFromDirectionalShadow((DirectionalLight *) light, scene, camera);
				break;
			case AE_POINT_LIGHT:
				CullActorsFromPointShadow((PointLight *) light, scene, camera);
				break;
		}

	}

	void FrustumCulling::CullActorsFromPointShadow(PointLight *light, Scene *scene, Camera *camera) {


	}

	void FrustumCulling::CullActorsFromDirectionalShadow(DirectionalLight *light, Scene *scene, Camera *camera) {


	}

}