#include "PostProcessRenderer.h"
#include "MasterRenderer.h"

namespace Atlas {

	namespace Renderer {

		std::string PostProcessRenderer::vertexPath = "postprocessing.vsh";
		std::string PostProcessRenderer::fragmentPath = "postprocessing.fsh";

		PostProcessRenderer::PostProcessRenderer() {

			shader.AddStage(AE_VERTEX_STAGE, vertexPath);
			shader.AddStage(AE_FRAGMENT_STAGE, fragmentPath);

			shader.Compile();

			GetUniforms();

		}

		void PostProcessRenderer::Render(Window* window, RenderTarget* target, Camera* camera, Scene* scene) {

			glViewport(window->viewport->x, window->viewport->y, window->viewport->width, window->viewport->height);

			auto postProcessing = scene->postProcessing;

			bool hasFilmicTonemappingMacro = shader.HasMacro("FILMIC_TONEMAPPING");
			bool hasVignetteMacro = shader.HasMacro("VIGNETTE");
			bool hasChromaticAberrationMacro = shader.HasMacro("CHROMATIC_ABERRATION");

			if (postProcessing->filmicTonemapping && !hasFilmicTonemappingMacro) {
				shader.AddMacro("FILMIC_TONEMAPPING");
			}
			else if (!postProcessing->filmicTonemapping && hasFilmicTonemappingMacro) {
				shader.RemoveMacro("FILMIC_TONEMAPPING");
			}

			if (postProcessing->vignette != nullptr && !hasVignetteMacro) {
				shader.AddMacro("VIGNETTE");
			}
			else if (postProcessing->vignette == nullptr  && hasVignetteMacro) {
				shader.RemoveMacro("VIGNETTE");
			}

			if (postProcessing->chromaticAberration != nullptr && !hasChromaticAberrationMacro) {
				shader.AddMacro("CHROMATIC_ABERRATION");
			}
			else if (postProcessing->chromaticAberration == nullptr && hasChromaticAberrationMacro) {
				shader.RemoveMacro("CHROMATIC_ABERRATION");
			}

			shader.Bind();

			hdrTexture->SetValue(0);
			bloomFirstTexture->SetValue(1);
			bloomSecondTexture->SetValue(2);
			bloomThirdTexture->SetValue(3);

			hdrTextureResolution->SetValue(
					vec2(target->lightingFramebuffer->GetComponentTexture(GL_COLOR_ATTACHMENT0)->width,
						 target->lightingFramebuffer->GetComponentTexture(GL_COLOR_ATTACHMENT0)->height));

			exposure->SetValue(postProcessing->exposure);
			saturation->SetValue(postProcessing->saturation);
			timeInMilliseconds->SetValue((float)clock());

			if (postProcessing->chromaticAberration != nullptr) {
				float reversedValue = postProcessing->chromaticAberration->colorsReversed ? 1.0f : 0.0f;
				aberrationStrength->SetValue(postProcessing->chromaticAberration->strength);
				aberrationReversed->SetValue(reversedValue);
			}

			if (postProcessing->vignette != nullptr) {
				vignetteOffset->SetValue(postProcessing->vignette->offset);
				vignettePower->SetValue(postProcessing->vignette->power);
				vignetteStrength->SetValue(postProcessing->vignette->strength);
				vignetteColor->SetValue(postProcessing->vignette->color);
			}

			target->lightingFramebuffer->GetComponentTexture(GL_COLOR_ATTACHMENT0)->Bind(GL_TEXTURE0);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		}

		void PostProcessRenderer::GetUniforms() {

			hdrTexture = shader.GetUniform("hdrTexture");
			bloomFirstTexture = shader.GetUniform("bloomFirstTexture");
			bloomSecondTexture = shader.GetUniform("bloomSecondTexture");
			bloomThirdTexture = shader.GetUniform("bloomThirdTexture");
			hdrTextureResolution = shader.GetUniform("hdrTextureResolution");
			exposure = shader.GetUniform("exposure");
			saturation = shader.GetUniform("saturation");
			bloomPassses = shader.GetUniform("bloomPassses");
			aberrationStrength = shader.GetUniform("aberrationStrength");
			aberrationReversed = shader.GetUniform("aberrationReversed");
			vignetteOffset = shader.GetUniform("vignetteOffset");
			vignettePower = shader.GetUniform("vignettePower");
			vignetteStrength = shader.GetUniform("vignetteStrength");
			vignetteColor = shader.GetUniform("vignetteColor");
			timeInMilliseconds = shader.GetUniform("timeInMilliseconds");

		}

	}

}