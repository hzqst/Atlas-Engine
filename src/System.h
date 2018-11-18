#ifndef SYSTEM_H
#define SYSTEM_H

#define _CRT_SECURE_NO_WARNINGS

#define ENGINE_INSTANT_SHADER_RELOAD
#define ENGINE_SHOW_LOG

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <exception>

#include "libraries/glm/glm.hpp"
#include "libraries/glm/gtc/type_ptr.hpp"
#include "libraries/glm/gtx/common.hpp"
#include "libraries/glm/gtx/transform.hpp"

#if  defined(ANDROID)
#define ENGINE_GLES

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
// This extension is not declared in gl3ext.h, only in gl2ext.h which we can't include.
// This means we have to declare these extension by ourselves
#ifndef GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT                           0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT                       0x84FF
#endif

#include <android/log.h>

#define EngineLog(...) __android_log_print(ANDROID_LOG_INFO, "ENGINE_LOG", __VA_ARGS__)

#elif defined(_MSC_VER) || defined(__linux__)
#define ENGINE_OGL

#ifdef _WIN32
#define APIENTRY __stdcall
#endif
#include "libraries/glad/glad/glad.h"

#define EngineLog(...) printf(__VA_ARGS__); printf("\n");

#endif

// Important definitions
using glm::vec4;
using glm::vec3;
using glm::vec2;

using glm::mat4;
using glm::mat3;
using namespace std;

typedef short float16_t;

class EngineException : public std::exception {

public:
	EngineException(const char* message) : msg(message) { }

	const char* what() const throw()
	{
		return msg;
	}

private:
	const char* msg;

};

#endif
