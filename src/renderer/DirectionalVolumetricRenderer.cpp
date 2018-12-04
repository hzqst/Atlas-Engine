#include "DirectionalVolumetricRenderer.h"
#include "../lighting/DirectionalLight.h"

string DirectionalVolumetricRenderer::volumetricVertexPath = "volumetric.vsh";
string DirectionalVolumetricRenderer::volumetricFragmentPath = "volumetric.fsh";
string DirectionalVolumetricRenderer::bilateralBlurVertexPath = "bilateralBlur.vsh";
string DirectionalVolumetricRenderer::bilateralBlurFragmentPath = "bilateralBlur.fsh";

DirectionalVolumetricRenderer::DirectionalVolumetricRenderer() {

    framebuffer = new Framebuffer(0, 0);

    volumetricShader = new Shader();
    volumetricShader->AddComponent(VERTEX_SHADER, volumetricVertexPath);
    volumetricShader->AddComponent(FRAGMENT_SHADER, volumetricFragmentPath);

    volumetricShader->Compile();

    GetVolumetricUniforms();

    bilateralBlurShader = new Shader();
    bilateralBlurShader->AddComponent(VERTEX_SHADER, bilateralBlurVertexPath);
    bilateralBlurShader->AddComponent(FRAGMENT_SHADER, bilateralBlurFragmentPath);

    bilateralBlurShader->AddMacro("BILATERAL");
    bilateralBlurShader->AddMacro("BLUR_R");

    bilateralBlurShader->Compile();

    GetBilateralBlurUniforms();

}

void DirectionalVolumetricRenderer::Render(Window *window, RenderTarget *target, Camera *camera, Scene *scene, bool masterRenderer) {

    framebuffer->Bind();

    volumetricShader->Bind();

	depthTexture->SetValue(0);
	shadowTexture->SetValue(1);

    inverseProjectionMatrix->SetValue(camera->inverseProjectionMatrix);
    target->geometryFramebuffer->GetComponentTexture(GL_DEPTH_ATTACHMENT)->Bind(GL_TEXTURE0);

    for (ILight* light : scene->lights) {

        if (light->type != DIRECTIONAL_LIGHT || light->shadow == nullptr || light->volumetric == nullptr) {
            continue;
        }

        DirectionalLight* directionalLight = (DirectionalLight*)light;

        glViewport(0, 0, directionalLight->volumetric->map->width, directionalLight->volumetric->map->height);

        framebuffer->AddComponentTexture(GL_COLOR_ATTACHMENT0, directionalLight->volumetric->map);

        vec3 direction = normalize(vec3(camera->viewMatrix * vec4(directionalLight->direction, 0.0f)));

        lightDirection->SetValue(direction);
        shadowCascadeCount->SetValue(directionalLight->shadow->componentCount);
        sampleCount->SetValue(directionalLight->volumetric->sampleCount);
		scattering->SetValue(glm::clamp(directionalLight->volumetric->scattering, -1.0f, 1.0f));
		framebufferResolution->SetValue(vec2(directionalLight->volumetric->map->width,
		        directionalLight->volumetric->map->height));

        light->shadow->maps->Bind(GL_TEXTURE1);
#ifdef ENGINE_OGL
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
#endif

        for (int32_t i = 0; i < light->shadow->componentCount; i++) {
            ShadowComponent* cascade = &directionalLight->shadow->components[i];
            cascades[i].distance->SetValue(cascade->farDistance);
            cascades[i].lightSpace->SetValue(cascade->projectionMatrix * cascade->viewMatrix * camera->inverseViewMatrix);
        }

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    }
	
    bilateralBlurShader->Bind();

    diffuseTexture->SetValue(0);
	bilateralDepthTexture->SetValue(1);

	target->geometryFramebuffer->GetComponentTexture(GL_DEPTH_ATTACHMENT)->Bind(GL_TEXTURE1);

    float offsetArray[] = {0, 1, 2, 3, 4, 5, 6, 7};
    float weightArray[] = {1/15.0f, 1/15.0f, 1/15.0f, 1/15.0f, 1/15.0f, 1/15.0f, 1/15.0f, 1/15.0f};

    offsets->SetValue(offsetArray, 8);
    weights->SetValue(weightArray, 8);
    kernelSize->SetValue(8);

    for (ILight* light : scene->lights) {

        if (light->type != DIRECTIONAL_LIGHT || light->shadow == nullptr || light->volumetric == nullptr) {
            continue;
        }

        DirectionalLight* directionalLight = (DirectionalLight*)light;

        glViewport(0, 0, directionalLight->volumetric->map->width, directionalLight->volumetric->map->height);

        framebuffer->AddComponentTexture(GL_COLOR_ATTACHMENT0, directionalLight->volumetric->blurMap);

        directionalLight->volumetric->map->Bind(GL_TEXTURE0);

        blurDirection->SetValue(vec2(1.0f / (float)directionalLight->volumetric->map->width, 0.0f));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        framebuffer->AddComponentTexture(GL_COLOR_ATTACHMENT0, directionalLight->volumetric->map);

        directionalLight->volumetric->blurMap->Bind(GL_TEXTURE0);

        blurDirection->SetValue(vec2(0.0f, 1.0f / (float)directionalLight->volumetric->map->height));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    }


}

void DirectionalVolumetricRenderer::GetVolumetricUniforms() {

    depthTexture = volumetricShader->GetUniform("depthTexture");
    shadowTexture = volumetricShader->GetUniform("cascadeMaps");
    lightDirection = volumetricShader->GetUniform("light.direction");
    inverseProjectionMatrix = volumetricShader->GetUniform("ipMatrix");
    sampleCount = volumetricShader->GetUniform("sampleCount");
	scattering = volumetricShader->GetUniform("scattering");
    shadowCascadeCount = volumetricShader->GetUniform("light.shadow.cascadeCount");
	framebufferResolution = volumetricShader->GetUniform("framebufferResolution");

    for (int32_t i = 0; i < MAX_SHADOW_CASCADE_COUNT; i++) {
        cascades[i].distance = volumetricShader->GetUniform("light.shadow.cascades[" + to_string(i) + "].distance");
        cascades[i].lightSpace = volumetricShader->GetUniform("light.shadow.cascades[" + to_string(i) + "].cascadeSpace");
    }

}

void DirectionalVolumetricRenderer::GetBilateralBlurUniforms() {

    diffuseTexture = bilateralBlurShader->GetUniform("diffuseTexture");
	bilateralDepthTexture = bilateralBlurShader->GetUniform("depthTexture");
    blurDirection = bilateralBlurShader->GetUniform("blurDirection");
    offsets = bilateralBlurShader->GetUniform("offset");
    weights = bilateralBlurShader->GetUniform("weight");
    kernelSize = bilateralBlurShader->GetUniform("kernelSize");

}