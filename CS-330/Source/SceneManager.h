///////////////////////////////////////////////////////////////////////////////
// shadermanager.h
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU In
// or / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"

#include <string>
#include <vector>


/***********************************************************
 *  MESHLIST
 *
 *  This enum class contains the list of 
 *  availble meshes
 ***********************************************************/
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

/***********************************************************
 *  RenderData
 *
 *  This struct contains the data necessary
 *  for rendering a single shader object
 ***********************************************************/

struct RenderData {
	// positional and rotation data
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// visual effects
	std::string ShaderTexture;
	std::string ShaderMaterial;
	glm::vec4 ShaderColor;
	glm::vec2 TextureUVScale; //{U, V} => (float, float)
	
	// the mesh to be rendered
	MESHLIST MeshType;

};

/***********************************************************
 *  SceneManager
 *
 *  This class contains the code for preparing and rendering
 *  3D scenes, including the shader settings.
 ***********************************************************/
class SceneManager
{
public:
	// constructor
	SceneManager(ShaderManager *pShaderManager);
	// destructor
	~SceneManager();

	struct TEXTURE_INFO
	{
		std::string tag;
		uint32_t ID;
	};

	struct OBJECT_MATERIAL
	{
		float ambientStrength;
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
		std::string tag;
	};


private:
	// pointer to shader manager object
	ShaderManager* m_pShaderManager;
	// pointer to basic shapes object
	ShapeMeshes* m_basicMeshes;
	// total number of loaded textures
	int m_loadedTextures;
	// loaded textures info
	TEXTURE_INFO m_textureIDs[16];
	// defined object materials
	std::vector<OBJECT_MATERIAL> m_objectMaterials;

	// load texture images and convert to OpenGL texture data
	bool CreateGLTexture(const char* filename, std::string tag);
	// bind loaded OpenGL textures to slots in memory
	void BindGLTextures();
	// free the loaded OpenGL textures
	void DestroyGLTextures();
	// find a loaded texture by tag
	int FindTextureID(std::string tag);
	int FindTextureSlot(std::string tag);
	// find a defined material by tag
	bool FindMaterial(std::string tag, OBJECT_MATERIAL& material);

	// set the transformation values 
	// into the transform buffer
	void SetTransformations(
		glm::vec3 scaleXYZ,
		float XrotationDegrees,
		float YrotationDegrees,
		float ZrotationDegrees,
		glm::vec3 positionXYZ);

	// set the color values into the shader
	void SetShaderColor(
		float redColorValue,
		float greenColorValue,
		float blueColorValue,
		float alphaValue);

	// set the color values into the shader
	void SetShaderColor(glm::vec4 currentColor);

	// set the texture data into the shader
	void SetShaderTexture(
		std::string textureTag);

	// set the UV scale for the texture mapping
	void SetTextureUVScale(
		float u, float v);

	// set the UV scale for the texture mapping - overload
	void SetTextureUVScale(glm::vec2 UVScale);

	// set the object material into the shader
	void SetShaderMaterial(
		std::string materialTag);

	// load the necessary textures
	void LoadSceneTextures();

	// load material types
	void DefineObjectMaterials();

	//load lights
	void SetupSceneLights();

	// Factory function for loading lists of Meshes to render
	void RenderList(std::vector<RenderData> AssetList);
public:

	void PrepareScene();
	void RenderScene();
	void RenderWoodBase();
	void RenderPlane();
	void RenderCoffeeCup();
	void RenderPitcher();
	void RenderKettle();
	void RenderCarafe();
	void AxisReference();
};
