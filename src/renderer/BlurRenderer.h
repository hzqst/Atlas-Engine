#ifndef AE_BLURRENDERER_H
#define AE_BLURRENDERER_H

#include "../System.h"
#include "IRenderer.h"

namespace Atlas {

    namespace Renderer {

        class BlurRenderer : IRenderer {

        public:
            BlurRenderer(std::string vertexSource, std::string fragmentSource, int32_t channelCount, float* kernelOffsets,
                         float* kernelWeights, int32_t kernelSize, bool bilateral = false);

            virtual void Render(Window* window, RenderTarget* target, Camera* camera, Scene* scene);

            void Render(Texture::Texture2D* texture, Texture::Texture2D* swapTexture, Texture::Texture2D* depthTexture);

        private:
            void GetUniforms();

            float* kernelOffsets;
            float* kernelWeights;
            int32_t kernelSize;

            bool bilateralBlur;

            Framebuffer* framebuffer;

            Shader::Shader shader;

            Shader::Uniform* diffuseTexture;
            Shader::Uniform* depthTexture;
            Shader::Uniform* blurDirection;


        };


    }

}

#endif
