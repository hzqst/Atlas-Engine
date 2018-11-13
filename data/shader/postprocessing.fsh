in vec2 fPosition;

uniform sampler2D hdrTexture;
uniform sampler2D bloomFirstTexture;
uniform sampler2D bloomSecondTexture;
uniform sampler2D bloomThirdTexture;
uniform float exposure;
uniform float saturation;

#ifdef BLOOM
uniform int bloomPasses;
#endif
#ifdef CHROMATIC_ABERRATION
uniform float aberrationStrength;
uniform float aberrationReversed;
#endif
#ifdef VIGNETTE
uniform float vignetteOffset;
uniform float vignettePower;
uniform float vignetteStrength;
uniform vec3 vignetteColor;
#endif

out vec3 color;

const float gamma = 1.0f / 2.2f;

vec3 ACESToneMap(vec3 hdrColor) {
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((hdrColor*(a*hdrColor+b))/
		(hdrColor*(c*hdrColor+d)+e), 0.0f, 1.0f);
}

vec3 ToneMap(vec3 hdrColor) {
	
	return vec3(1.0f) - exp(-hdrColor);
	
}

vec3 saturate(vec3 color, float factor) {
	const vec3 luma = vec3(0.299f, 0.587f, 0.114f);
    vec3 pixelLuminance = vec3(dot(color, luma));
	return mix(pixelLuminance, color, factor);
}

void main() {
	
	vec2 fTexCoord = 0.5f * fPosition + 0.5f;
	
#ifdef CHROMATIC_ABERRATION
	vec2 uvRedChannel = (fPosition - fPosition * 0.005f * aberrationStrength 
		* aberrationReversed) * 0.5f + 0.5f;
	vec2 uvGreenChannel = (fPosition - fPosition * 0.0025f * aberrationStrength) * 0.5f + 0.5f;
	vec2 uvBlueChannel =  (fPosition - fPosition * 0.005f * aberrationStrength
		* (1.0f - aberrationReversed)) * 0.5f + 0.5f;
	
	color.r = texture(hdrTexture, uvRedChannel).r;
	color.g = texture(hdrTexture, uvGreenChannel).g;
	color.b = texture(hdrTexture, uvBlueChannel).b;
	
#ifdef BLOOM
    // We want to keep a constant expression in texture[const]
	// because OpenGL ES doesn't support dynamic texture fetches
	// inside a loop
	if (bloomPasses > 0) {
		color.r += texture(bloomFirstTexture, uvRedChannel).r;
		color.g += texture(bloomFirstTexture, uvGreenChannel).g;
		color.b += texture(bloomFirstTexture, uvBlueChannel).b;
	}
	if (bloomPasses > 1) {
		color.r += texture(bloomSecondTexture, uvRedChannel).r;
		color.g += texture(bloomSecondTexture, uvGreenChannel).g;
		color.b += texture(bloomSecondTexture, uvBlueChannel).b;
	}
	if (bloomPasses > 2) {
		color.r += texture(bloomThirdTexture, uvRedChannel).r;
		color.g += texture(bloomThirdTexture, uvGreenChannel).g;
		color.b += texture(bloomThirdTexture, uvBlueChannel).b;
	}
#endif
#else
	color = texture(hdrTexture, fTexCoord).rgb;
	
#ifdef BLOOM
	if (bloomPasses > 0) {
		color += texture(bloomFirstTexture, fTexCoord).rgb;
	}
	if (bloomPasses > 1) {
		color += texture(bloomSecondTexture, fTexCoord).rgb;
	}
	if (bloomPasses > 2) {
		color += texture(bloomThirdTexture, fTexCoord).rgb;
	}
#endif
#endif

	// If performance mode is activated the colors are in [0,1] range
#ifndef ENGINE_FASTER_FRAMEBUFFERS
	color *= exposure;
	
	// Apply the tone mapping because we want the colors to be back in
	// normal range
#ifdef FILMIC_TONEMAPPING
	color = ACESToneMap(color);
#else
	color = ToneMap(color);
#endif
#endif
	
	color = pow(color, vec3(gamma));
	
	color = saturate(color, saturation);

#ifdef VIGNETTE	
	float vignetteFactor = max(1.0f - max(pow(length(fPosition) - vignetteOffset, vignettePower), 0.0f)
		* vignetteStrength, 0.0f);
	
	color = mix(vignetteColor, color, vignetteFactor);
#endif
	
}