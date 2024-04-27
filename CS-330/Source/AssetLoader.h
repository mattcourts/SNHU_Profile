#pragma once

#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>
#include <functional>
#include "ShapeMeshes.h"
#include "SceneManager.h"

enum class MESHLIST {
	Box,
	Cone,
	ConeNoBottom,
	Cylinder,
	CylinderNoTop,
	CylinderNoBottom,
	CylinderOpen,
	Plane,
	Prism,
	Pyramid3,
	Pyramid4,
	Sphere,
	HalfSphere,
	TaperedCylinder,
	TaperedCylinderNoTop,
	TaperedCylinderNoBottom,
	TaperedCylinderOpen,
	Torus,
	HalfTorus
};

struct GeometricShape {
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	std::string ShaderTexture;
	std::string ShaderMaterial;
	glm::vec4 ShaderColor;
	glm::vec2 TextureUVScale; //{U, V} => (float, float)
	
	MESHLIST MeshType;

};

class AssetLoader {

public:
	static void RenderAssets(
		std::vector<GeometricShape> AssetList,
		std::function<void(glm::vec3, float, float, float, glm::vec3)> SetTransformations,
		std::function<void(float, float, float, float)> SetShaderColor,
		std::function<void(std::string)> SetShaderTexture,
		std::function<void(float, float)> SetTextureUVScale,
		std::function<void(std::string)> SetShaderMaterial,
		ShapeMeshes* m_basicMeshes,
		glm::vec3 CenterPoint
	);

};