#include "SkyboxRenderer.h"
#include "helper/GeometryHelper.h"

namespace Atlas {

	namespace Renderer {

		std::string SkyboxRenderer::vertexPath = "skybox.vsh";
		std::string SkyboxRenderer::fragmentPath = "skybox.fsh";

		SkyboxRenderer::SkyboxRenderer() {

			Helper::GeometryHelper::GenerateCubeVertexArray(vertexArray);

			shader.AddStage(AE_VERTEX_STAGE, vertexPath);
			shader.AddStage(AE_FRAGMENT_STAGE, fragmentPath);

			shader.Compile();

			GetUniforms();

		}

		void SkyboxRenderer::Render(Window* window, RenderTarget* target, Camera* camera, Scene* scene) {

			shader.Bind();

			skyCubemap->SetValue(0);

			mat4 mvpMatrix = camera->projectionMatrix * glm::mat4(glm::mat3(camera->viewMatrix)) * scene->sky->skybox->matrix;

			modelViewProjectionMatrix->SetValue(mvpMatrix);

			vertexArray.Bind();

			scene->sky->skybox->cubemap->Bind(GL_TEXTURE0);

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		void SkyboxRenderer::GetUniforms() {

			skyCubemap = shader.GetUniform("skyCubemap");
			modelViewProjectionMatrix = shader.GetUniform("mvpMatrix");

		}

	}

}