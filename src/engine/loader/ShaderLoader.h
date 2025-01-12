#pragma once

#include "../System.h"
#include "../graphics/Shader.h"

#include <vector>
#include <filesystem>

namespace Atlas {

    namespace Loader {

        class ShaderLoader {

        public:
            static Graphics::ShaderStageFile LoadFile(const std::string& filename, VkShaderStageFlagBits shaderStage);

            static bool CheckForReload(const std::string& filename, const std::filesystem::file_time_type fileTime,
                std::filesystem::file_time_type& pathLastModified);

            static void SetSourceDirectory(const std::string& sourceDirectory);

        private:
            static std::string ReadShaderFile(const std::string& filename, bool mainFile,
                std::vector<std::string>& includes, std::vector<Graphics::ShaderStageFile::Extension>& extensions,
                std::filesystem::file_time_type& lastModified);

            static std::string ExtractIncludes(const std::string& filename, std::string& code,
                std::vector<std::string>& includes, std::vector<Graphics::ShaderStageFile::Extension>& extensions,
                std::filesystem::file_time_type& lastModified);

            static std::vector<std::string> ExtractExtensions(std::vector<std::string> codeLines,
                std::vector<Graphics::ShaderStageFile::Extension>& extensions);

            static std::filesystem::file_time_type GetModifiedTime(const std::string& path, 
                const std::filesystem::file_time_type defaultTime);

            static std::string sourceDirectory;

        };

    }

}