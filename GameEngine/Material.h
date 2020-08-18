#pragma once

#include <glm/common.hpp>

struct Material
{
	glm::vec3 Ka;       // Ambient Color
	glm::vec3 Kd;       // Diffuse Color
	glm::vec3 Ks;       // Specular Color
	glm::vec3 Ke;       // Emissive Light
	float Ni;           // Optical Index (Index of Refraction)
	float d;            // Alpha (Transparency)
	unsigned int illum; // Illumination Info
};
