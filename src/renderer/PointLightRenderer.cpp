#include "PointLightRenderer.h"
#include "helper/GeometryHelper.h"

PointLightRenderer::PointLightRenderer(string vertexSource, string fragmentSource) {

	vertexArray = GeometryHelper::GenerateSphereVertexArray(16, 16);

	shader = new Shader();

	shader->AddComponent(VERTEX_SHADER, vertexSource);
	shader->AddComponent(FRAGMENT_SHADER, fragmentSource);

	shader->Compile();

	GetUniforms();

}

void PointLightRenderer::Render(Window* window, RenderTarget* target, Camera* camera, Scene* scene, bool masterRenderer) {

	shader->Bind();

	vertexArray->Bind();

	diffuseTexture->SetValue(0);
	normalTexture->SetValue(1);
	materialTexture->SetValue(2);
	depthTexture->SetValue(3);

	viewProjectionMatrix->SetValue(camera->projectionMatrix * camera->viewMatrix);
	inverseProjectionMatrix->SetValue(camera->inverseProjectionMatrix);

	target->geometryFramebuffer->GetComponent(GL_COLOR_ATTACHMENT0)->Bind(GL_TEXTURE0);
	target->geometryFramebuffer->GetComponent(GL_COLOR_ATTACHMENT1)->Bind(GL_TEXTURE1);
	target->geometryFramebuffer->GetComponent(GL_COLOR_ATTACHMENT2)->Bind(GL_TEXTURE2);
	target->geometryFramebuffer->GetComponent(GL_DEPTH_ATTACHMENT)->Bind(GL_TEXTURE3);

	for (auto light : scene->lights) {

		if (light->type != POINT_LIGHT) {
			continue;
		}

		float radius = 5.0f;

		viewSpaceLightLocation->SetValue(vec3(camera->viewMatrix * vec4(light->location, 1.0f)));
		lightLocation->SetValue(light->location);
		lightColor->SetValue(light->diffuseColor);
		lightAmbient->SetValue(light->ambient);
		lightRadius->SetValue(radius);

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexComponent()->GetElementCount(),
			vertexArray->GetIndexComponent()->GetDataType(), NULL);

	}

}

void PointLightRenderer::GetUniforms() {

	diffuseTexture = shader->GetUniform("diffuseTexture");
	normalTexture = shader->GetUniform("normalTexture");
	materialTexture = shader->GetUniform("materialTexture");
	depthTexture = shader->GetUniform("depthTexture");
	viewProjectionMatrix = shader->GetUniform("vpMatrix");
	inverseProjectionMatrix = shader->GetUniform("ipMatrix");
	viewSpaceLightLocation = shader->GetUniform("viewSpaceLightLocation");
	lightLocation = shader->GetUniform("light.location");
	lightColor = shader->GetUniform("light.color");
	lightAmbient = shader->GetUniform("light.ambient");
	lightRadius = shader->GetUniform("light.radius");

}