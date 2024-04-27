///////////////////////////////////////////////////////////////////////////////
// shadermanager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	 //modelView = translation * rotationX * rotationY * rotationZ * scale; 
	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command - overloaded
 ***********************************************************/
void SceneManager::SetShaderColor(glm::vec4 currentColor)
{

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(glm::vec2 UVScale)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", UVScale);
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/

/***********************************************************
 *  LoadSceneTextures()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void SceneManager::LoadSceneTextures()
{
	bool bReturn = false;

	bReturn = CreateGLTexture(
		"../../Utilities/textures/rusticwood.jpg","DecorativeBase");

	bReturn = CreateGLTexture(
		"textures/lighttile.jpg","lighttile");

	bReturn = CreateGLTexture(
		"textures/darktile.jpg", "darktile");


	// after the texture image data is loaded into memory, the
	// loaded textures need to be bound to texture slots - there
	// are a total of 16 available slots for scene textures
	BindGLTextures();
}


/***********************************************************
 *  DefineObjectMaterials()
 *
 *  This method is used for configuring the various material
 *  settings for all of the objects within the 3D scene.
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{

	// kettle
	OBJECT_MATERIAL steelMaterial;
	steelMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	steelMaterial.ambientStrength = 0.3f;
	steelMaterial.diffuseColor = glm::vec3(0.2f, 0.2f, 0.2f);
	steelMaterial.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	steelMaterial.shininess = 2.0;
	steelMaterial.tag = "steel";

	m_objectMaterials.push_back(steelMaterial);

	// wood 
	OBJECT_MATERIAL woodMaterial;
	woodMaterial.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	woodMaterial.ambientStrength = 0.2f;
	woodMaterial.diffuseColor = glm::vec3(0.3f, 0.3f, 0.3f);
	woodMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	woodMaterial.shininess = 0.3;
	woodMaterial.tag = "wood";

	m_objectMaterials.push_back(woodMaterial);


	//coffee cup
	OBJECT_MATERIAL ceramicMaterial;
	ceramicMaterial.ambientColor = glm::vec3(0.3f, 0.3f, 0.3f);
	ceramicMaterial.ambientStrength = 0.1f;
	ceramicMaterial.diffuseColor = glm::vec3(0.2f, 0.2f, 0.2f);
	ceramicMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.2f);
	ceramicMaterial.shininess = 0.5;
	ceramicMaterial.tag = "ceramic";

	m_objectMaterials.push_back(ceramicMaterial);


	// Glass
	OBJECT_MATERIAL glassMaterial;
	glassMaterial.ambientColor = glm::vec3(0.9f, 1.0f, 1.0f);
	glassMaterial.ambientStrength = 0.1f;
	glassMaterial.diffuseColor = glm::vec3(0.9f, 1.0f, 1.0f);
	glassMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glassMaterial.shininess = 25.0;
	glassMaterial.tag = "glass";

	m_objectMaterials.push_back(glassMaterial);


}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// this line of code is NEEDED for telling the shaders to render 
	// the 3D scene with custom lighting - to use the default rendered 
	// lighting then comment out the following line
	m_pShaderManager->setBoolValue(g_UseLightingName, true);


	// square light pattern with one in center
	m_pShaderManager->setVec3Value("lightSources[0].position", -6.0f, 10.0f, 6.0f);
	m_pShaderManager->setVec3Value("lightSources[0].ambientColor", 0.01f, 0.01f, 0.01f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 0.6f, 0.6f, 0.6f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 32.0f);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.5f);

	m_pShaderManager->setVec3Value("lightSources[1].position", 6.0f, 10.0f, -6.0f);
	m_pShaderManager->setVec3Value("lightSources[1].ambientColor", 0.01f, 0.01f, 0.01f);
	m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 0.6f, 0.6f, 0.6f);
	m_pShaderManager->setVec3Value("lightSources[1].specularColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 32.0f);
	m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 0.5f);

	//center
	m_pShaderManager->setVec3Value("lightSources[2].position", 0.0f, 10.0f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[2].ambientColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setVec3Value("lightSources[2].diffuseColor", 0.1f, 0.1f, 0.1f);
	m_pShaderManager->setVec3Value("lightSources[2].specularColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setFloatValue("lightSources[2].focalStrength", 16.0f);
	m_pShaderManager->setFloatValue("lightSources[2].specularIntensity", 0.5f);

	m_pShaderManager->setVec3Value("lightSources[3].position", 6.0f, 10.0f, 6.0f);
	m_pShaderManager->setVec3Value("lightSources[3].ambientColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setVec3Value("lightSources[3].diffuseColor", 0.6f, 0.6f, 0.6f);
	m_pShaderManager->setVec3Value("lightSources[3].specularColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setFloatValue("lightSources[3].focalStrength", 32.0f);
	m_pShaderManager->setFloatValue("lightSources[3].specularIntensity", 0.5f);

	m_pShaderManager->setVec3Value("lightSources[4].position", -6.0f, 10.0f, -6.0f);
	m_pShaderManager->setVec3Value("lightSources[4].ambientColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setVec3Value("lightSources[4].diffuseColor", 0.6f, 0.6f, 0.6f);
	m_pShaderManager->setVec3Value("lightSources[4].specularColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setFloatValue("lightSources[4].focalStrength", 32.0f);
	m_pShaderManager->setFloatValue("lightSources[4].specularIntensity", 0.5f);


}


/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	//load textures
	LoadSceneTextures();
	// define the materials that will be used for the objects
	// in the 3D scene
	DefineObjectMaterials();
	// add and defile the light sources for the 3D scene
	SetupSceneLights();

	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadTorusMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadSphereMesh();
	m_basicMeshes->LoadPyramid3Mesh();
	m_basicMeshes->LoadBoxMesh();

}

/***********************************************************
 *  RenderList()
 *
 *  This method is for rendering vector containing a list
 *  of objects centered around a location.
 ***********************************************************/
void SceneManager::RenderList(std::vector<RenderData> AssetList)
{
	for (RenderData Asset : AssetList)
	{
		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			Asset.scaleXYZ,
			Asset.XrotationDegrees,
			Asset.YrotationDegrees,
			Asset.ZrotationDegrees,
			Asset.positionXYZ
		);

		//Set Color, Texture, UVScale, and Material
		if (Asset.ShaderColor != glm::vec4{}) { SetShaderColor(Asset.ShaderColor); }
		if (Asset.ShaderTexture != "") { SetShaderTexture(Asset.ShaderTexture); }
		if (Asset.TextureUVScale != glm::vec2{}) { SetTextureUVScale(Asset.TextureUVScale); }
		if (Asset.ShaderMaterial != "") { SetShaderMaterial(Asset.ShaderMaterial); }

		switch (Asset.MeshType)
		{
		case MESHLIST::Box: m_basicMeshes->DrawBoxMesh();  break;
		case MESHLIST::Cone: m_basicMeshes->DrawConeMesh();  break;
		case MESHLIST::ConeNoBottom: m_basicMeshes->DrawConeMesh(false); break;
		case MESHLIST::Cylinder: m_basicMeshes->DrawCylinderMesh(); break;
		case MESHLIST::CylinderNoTop: m_basicMeshes->DrawCylinderMesh(false, true, true); break;
		case MESHLIST::CylinderNoBottom: m_basicMeshes->DrawCylinderMesh(true, false, true); break;
		case MESHLIST::CylinderOpen: m_basicMeshes->DrawCylinderMesh(false, false, true); break;
		case MESHLIST::Plane: m_basicMeshes->DrawPlaneMesh(); break;
		case MESHLIST::Prism: m_basicMeshes->DrawPrismMesh(); break;
		case MESHLIST::Pyramid3: m_basicMeshes->DrawPyramid3Mesh(); break;
		case MESHLIST::Pyramid4: m_basicMeshes->DrawPyramid4Mesh(); break;
		case MESHLIST::Sphere: m_basicMeshes->DrawSphereMesh(); break;
		case MESHLIST::HalfSphere: m_basicMeshes->DrawHalfSphereMesh(); break;
		case MESHLIST::TaperedCylinder: m_basicMeshes->DrawTaperedCylinderMesh(); break;
		case MESHLIST::TaperedCylinderNoTop: m_basicMeshes->DrawTaperedCylinderMesh(false, true, true); break;
		case MESHLIST::TaperedCylinderNoBottom: m_basicMeshes->DrawTaperedCylinderMesh(true, false, true); break;
		case MESHLIST::TaperedCylinderOpen: m_basicMeshes->DrawTaperedCylinderMesh(false, false, true); break;
		case MESHLIST::Torus: m_basicMeshes->DrawTorusMesh(); break;
		case MESHLIST::HalfTorus: m_basicMeshes->DrawHalfTorusMesh(); break;
		}
	}
}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	RenderPlane();
	RenderWoodBase();
	RenderCoffeeCup();
	RenderPitcher();
	RenderKettle();
	RenderCarafe();
	//AxisReference();
}

/***********************************************************
 *  AxisReference()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::AxisReference()
{
	//Stack containing objects for rendering shape
	std::vector<RenderData> ShapeList;

	//Define Colors
	glm::vec4 RedColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 GreenColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 BlueColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	//Define Central Location of Object
	float centerX = -10.0f;
	float centerY = 10.0f;
	float centerZ = 2.0f;

	//XAxis location, rotation, shader and mesh information
	RenderData XAxis;
	XAxis.scaleXYZ = glm::vec3(5.0f, 1.0f, 1.0f);
	XAxis.XrotationDegrees = 0.0f;
	XAxis.YrotationDegrees = 0.0f;
	XAxis.ZrotationDegrees = 0.0f;
	XAxis.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 1.0f, centerZ + 0.0f);
	XAxis.ShaderColor = RedColor;
	XAxis.MeshType = MESHLIST::Box;
	ShapeList.push_back(XAxis);

	//YAxis location, rotation, shader and mesh information
	RenderData YAxis;
	YAxis.scaleXYZ = glm::vec3(1.0f, 5.0f, 1.0f);
	YAxis.XrotationDegrees = 0.0f;
	YAxis.YrotationDegrees = 0.0f;
	YAxis.ZrotationDegrees = 0.0f;
	YAxis.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 1.0f, centerZ + 0.0f);
	YAxis.ShaderColor = GreenColor;
	YAxis.MeshType = MESHLIST::Box;
	ShapeList.push_back(YAxis);

	//ZAxis location, rotation, shader and mesh information
	RenderData ZAxis;
	ZAxis.scaleXYZ = glm::vec3(1.0f, 1.0f, 5.0f);
	ZAxis.XrotationDegrees = 0.0f;
	ZAxis.YrotationDegrees = 0.0f;
	ZAxis.ZrotationDegrees = 0.0f;
	ZAxis.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 1.0f, centerZ + 0.0f);
	ZAxis.ShaderColor = BlueColor;
	ZAxis.MeshType = MESHLIST::Box;
	ShapeList.push_back(ZAxis);

	//render stack at given location
	RenderList(ShapeList);

}

/***********************************************************
 *  RenderPlane()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderPlane()
{
	//Stack containing objects for rendering shape
	std::vector<RenderData> ShapeList;

	//Plane location, rotation, shader and mesh information
	RenderData Plane;
	Plane.scaleXYZ = glm::vec3(40.0f, 1.0f, 40.0f);
	Plane.positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	Plane.XrotationDegrees = 0.0f;
	Plane.YrotationDegrees = 0.0f;
	Plane.ZrotationDegrees = 0.0f;
	Plane.ShaderColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	Plane.MeshType = MESHLIST::Plane;
	ShapeList.push_back(Plane);

	//render stack at given location
	RenderList(ShapeList);

}

/***********************************************************
 *  RenderWoodBase()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderWoodBase()
{

	//Stack containing objects for rendering shape
	std::vector<RenderData> ShapeList;

	//WoodBase location, rotation, shader and mesh information
	RenderData WoodBase;
	WoodBase.scaleXYZ = glm::vec3(20.0f, 1.0f, 20.0f);
	WoodBase.XrotationDegrees = 0.0f;
	WoodBase.YrotationDegrees = 0.0f;
	WoodBase.ZrotationDegrees = 0.0f;
	WoodBase.positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	WoodBase.ShaderTexture = "DecorativeBase";
	WoodBase.TextureUVScale = glm::vec2(1.0, 1.0);
	WoodBase.ShaderMaterial = "wood";
	WoodBase.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(WoodBase);

	//render stack at given location
	RenderList(ShapeList);

}

/***********************************************************
 *  RenderCoffeeCup()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderCoffeeCup()
{
	//Stack containing objects for rendering shape
	std::vector<RenderData> ShapeList;

	//Define Central Location of Object
	float centerX = -1.0f;
	float centerY = 0.1f;
	float centerZ = 12.0f;

	//CupPlate location, rotation, shader and mesh information
	RenderData CupPlate;
	CupPlate.scaleXYZ = glm::vec3(4.0f, 0.24f, 4.0f);
	CupPlate.XrotationDegrees = 0.0f;
	CupPlate.YrotationDegrees = 0.0f;
	CupPlate.ZrotationDegrees = 0.0f;
	CupPlate.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 1.0f,  centerZ + 0.0f);
	CupPlate.ShaderTexture = "lighttile";
	CupPlate.TextureUVScale = glm::vec2(1.0, 1.0);
	CupPlate.ShaderMaterial = "ceramic";
	CupPlate.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(CupPlate);


	//CupBase location, rotation, shader and mesh information
	RenderData CupBase;
	CupBase.scaleXYZ = glm::vec3(3.0f, 2.0f, 3.0f);
	CupBase.XrotationDegrees = 180.0f;
	CupBase.YrotationDegrees = 0.0f;
	CupBase.ZrotationDegrees = 0.0f;
	CupBase.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 3.25f, centerZ + 0.0f);
	CupBase.ShaderTexture = "darktile";
	CupBase.TextureUVScale = glm::vec2(1.0, 1.0);
	CupBase.ShaderMaterial = "ceramic";
	CupBase.MeshType = MESHLIST::TaperedCylinderNoBottom;
	ShapeList.push_back(CupBase);


	//CupTop location, rotation, shader and mesh information
	RenderData CupTop;
	CupTop.scaleXYZ = glm::vec3(3.0f, 2.0f, 3.0f);
	CupTop.XrotationDegrees = 0.0f;
	CupTop.YrotationDegrees = 0.0f;
	CupTop.ZrotationDegrees = 0.0f;
	CupTop.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 3.25f, centerZ + 0.0f);
	CupTop.ShaderTexture = "lighttile";
	CupTop.TextureUVScale = glm::vec2(0.5, 0.5);
	CupTop.ShaderMaterial = "ceramic";
	CupTop.MeshType = MESHLIST::CylinderOpen;
	ShapeList.push_back(CupTop);


	//CupHandle location, rotation, shader and mesh information
	RenderData CupHandle;
	CupHandle.scaleXYZ = glm::vec3(0.75f, 1.0f, 0.75f);
	CupHandle.XrotationDegrees = 0.0f;
	CupHandle.YrotationDegrees = 0.0f;
	CupHandle.ZrotationDegrees = 90.0f;
	CupHandle.positionXYZ = glm::vec3(centerX + -3.0f, centerY + 4.25f, centerZ + 0.0f);
	CupHandle.ShaderTexture = "darktile";
	CupHandle.TextureUVScale = glm::vec2(1.0, 1.0);
	CupHandle.ShaderMaterial = "ceramic";
	CupHandle.MeshType = MESHLIST::HalfTorus;
	ShapeList.push_back(CupHandle);
	
	//render stack at given location
	RenderList(ShapeList);

}


/***********************************************************
 *  RenderPitcher()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderPitcher()
{

	//Stack containing objects for rendering shape
	std::vector<RenderData> ShapeList;

	//Define Central Location of Object
	float centerX = 10.0f;
	float centerY = 1.1f;
	float centerZ = 7.0f;

	//Define Color for whole object - Bronze
	glm::vec4 BronzeColor = glm::vec4(0.75f,0.47f,0.14f,1.0f);



	//PitcherBody location, rotation, shader and mesh information
	RenderData PitcherBody;
	PitcherBody.scaleXYZ = glm::vec3(4.0f, 8.0f, 4.0f);
	PitcherBody.XrotationDegrees = 0.0f;
	PitcherBody.YrotationDegrees = 0.0f;
	PitcherBody.ZrotationDegrees = 0.0f;
	PitcherBody.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 0.0f, centerZ + 0.0f);
	PitcherBody.ShaderColor = BronzeColor;
	PitcherBody.ShaderMaterial = "steel";
	PitcherBody.MeshType = MESHLIST::TaperedCylinderNoTop;
	ShapeList.push_back(PitcherBody);

	//PitcherLid location, rotation, shader and mesh information
	RenderData PitcherLid;
	PitcherLid.scaleXYZ = glm::vec3(2.1f, 2.0f, 2.1f);
	PitcherLid.XrotationDegrees = 0.0f;
	PitcherLid.YrotationDegrees = 0.0f;
	PitcherLid.ZrotationDegrees = 0.0f;
	PitcherLid.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 7.9f, centerZ + 0.0f);
	PitcherLid.ShaderColor = BronzeColor;
	PitcherLid.ShaderMaterial = "steel";
	PitcherLid.MeshType = MESHLIST::HalfSphere;
	ShapeList.push_back(PitcherLid);


	//PitcherTop location, rotation, shader and mesh information
	RenderData PitcherTop;
	PitcherTop.scaleXYZ = glm::vec3(0.25f, 0.25f, 0.25f);
	PitcherTop.XrotationDegrees = 0.0f;
	PitcherTop.YrotationDegrees = 0.0f;
	PitcherTop.ZrotationDegrees = 0.0f;
	PitcherTop.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 10.15f, centerZ + 0.0f);
	PitcherTop.ShaderColor = BronzeColor;
	PitcherTop.ShaderMaterial = "steel";
	PitcherTop.MeshType = MESHLIST::Sphere;
	ShapeList.push_back(PitcherTop);


	//PitcherPourSpout location, rotation, shader and mesh information
	RenderData PitcherPourSpout;
	PitcherPourSpout.scaleXYZ = glm::vec3(1.5f, 1.0f, 1.5f);
	PitcherPourSpout.XrotationDegrees = 180.0f;
	PitcherPourSpout.YrotationDegrees = 0.0f;
	PitcherPourSpout.ZrotationDegrees = 0.0f;
	PitcherPourSpout.positionXYZ = glm::vec3(centerX + -2.0f, centerY + 7.5f, centerZ + -1.0f);
	PitcherPourSpout.ShaderColor = BronzeColor;
	PitcherPourSpout.ShaderMaterial = "steel";
	PitcherPourSpout.MeshType = MESHLIST::Pyramid3;
	ShapeList.push_back(PitcherPourSpout);


	//PitcherHandle location, rotation, shader and mesh information
	RenderData PitcherHandle;
	PitcherHandle.scaleXYZ = glm::vec3(1.5f, 2.0f, 1.5f);
	PitcherHandle.XrotationDegrees = 30.0f;
	PitcherHandle.YrotationDegrees = 0.0f;
	PitcherHandle.ZrotationDegrees = 280.0f;
	PitcherHandle.positionXYZ = glm::vec3(centerX + 2.3f, centerY + 5.5f, centerZ + 1.0f);
	PitcherHandle.ShaderColor = BronzeColor;
	PitcherHandle.ShaderMaterial = "steel";
	PitcherHandle.MeshType = MESHLIST::HalfTorus;
	ShapeList.push_back(PitcherHandle);

	//render stack at given location
	RenderList(ShapeList);

}


/***********************************************************
 *  RenderCarafe()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderCarafe()
{

	//Stack containing objects for rendering shape
	std::vector<RenderData> ShapeList;

	//Define Central Location of Object
	float centerX = 0.0f;
	float centerY = 1.1f;
	float centerZ = -5.0f;

	//Define Color for whole object - clear but tinted blue
	glm::vec4 GlassColor = glm::vec4(0.937f, 1.0f, 1.0f, 0.5f);



	//CarafeBase location, rotation, shader and mesh information
	RenderData CarafeBase;
	CarafeBase.scaleXYZ = glm::vec3(4.0f, 2.0f, 4.0f);
	CarafeBase.XrotationDegrees = 180.0f;
	CarafeBase.YrotationDegrees = 0.0f;
	CarafeBase.ZrotationDegrees = 0.0f;
	CarafeBase.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 2.0f, centerZ + 0.0f);
	CarafeBase.ShaderColor = GlassColor;
	CarafeBase.ShaderMaterial = "glass";
	CarafeBase.MeshType = MESHLIST::TaperedCylinderNoTop;
	ShapeList.push_back(CarafeBase);


	//CarafeMiddle location, rotation, shader and mesh information
	RenderData CarafeMiddle;
	CarafeMiddle.scaleXYZ = glm::vec3(4.0f, 4.0f, 4.0f);
	CarafeMiddle.XrotationDegrees = 0.0f;
	CarafeMiddle.YrotationDegrees = 0.0f;
	CarafeMiddle.ZrotationDegrees = 0.0f;
	CarafeMiddle.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 2.0f, centerZ + 0.0f);
	CarafeMiddle.ShaderColor = GlassColor;
	CarafeMiddle.ShaderMaterial = "glass";
	CarafeMiddle.MeshType = MESHLIST::TaperedCylinderOpen;
	ShapeList.push_back(CarafeMiddle);

	//CarafeTop location, rotation, shader and mesh information
	RenderData CarafeTop;
	CarafeTop.scaleXYZ = glm::vec3(4.0f, 3.0f, 4.0f);
	CarafeTop.XrotationDegrees = 180.0f;
	CarafeTop.YrotationDegrees = 0.0f;
	CarafeTop.ZrotationDegrees = 0.0f;
	CarafeTop.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 9.0f, centerZ + 0.0f);
	CarafeTop.ShaderColor = GlassColor;
	CarafeTop.ShaderMaterial = "glass";
	CarafeTop.MeshType = MESHLIST::TaperedCylinderOpen;
	ShapeList.push_back(CarafeTop);

	//CarafeHandleMiddle location, rotation, shader and mesh information
	RenderData CarafeHandleMiddle;
	CarafeHandleMiddle.scaleXYZ = glm::vec3(4.0f, 2.0f, 4.0f);
	CarafeHandleMiddle.XrotationDegrees = 0.0f;
	CarafeHandleMiddle.YrotationDegrees = 0.0f;
	CarafeHandleMiddle.ZrotationDegrees = 0.0f;
	CarafeHandleMiddle.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 4.0f, centerZ + 0.0f);
	CarafeHandleMiddle.ShaderTexture = "DecorativeBase";
	CarafeHandleMiddle.TextureUVScale = glm::vec2(0.2, 0.2);
	CarafeHandleMiddle.ShaderMaterial = "wood";
	CarafeHandleMiddle.MeshType = MESHLIST::TaperedCylinderOpen;
	ShapeList.push_back(CarafeHandleMiddle);

	//CarafeHandleTop location, rotation, shader and mesh information
	RenderData CarafeHandleTop;
	CarafeHandleTop.scaleXYZ = glm::vec3(4.0f, 1.5f, 4.0f);
	CarafeHandleTop.XrotationDegrees = 180.0f;
	CarafeHandleTop.YrotationDegrees = 0.0f;
	CarafeHandleTop.ZrotationDegrees = 0.0f;
	CarafeHandleTop.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 7.5f, centerZ + 0.0f);
	CarafeHandleTop.ShaderTexture = "DecorativeBase";
	CarafeHandleTop.TextureUVScale = glm::vec2(0.2, 0.2);
	CarafeHandleTop.ShaderMaterial = "wood";
	CarafeHandleTop.MeshType = MESHLIST::TaperedCylinderOpen;
	ShapeList.push_back(CarafeHandleTop);


	//render stack at given location
	RenderList(ShapeList);

}

/***********************************************************
 *  RenderKettle()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderKettle()
{

	//Stack containing objects for rendering shape
	std::vector<RenderData> ShapeList;

	//Define Central Location of Object
	float centerX = -11.0f;
	float centerY = 1.5f;
	float centerZ = -5.0f;

	//Define Color for whole object - silver
	glm::vec4 SilverColor = glm::vec4(0.753f, 0.753f, 0.753f, 1.0f);
	glm::vec4 BlackColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

	//KettleBase location, rotation, shader and mesh information
	RenderData KettleBase;
	KettleBase.scaleXYZ = glm::vec3(4.0f, 0.25f, 4.0f);
	KettleBase.XrotationDegrees = 0.0f;
	KettleBase.YrotationDegrees = 0.0f;
	KettleBase.ZrotationDegrees = 0.0f;
	KettleBase.positionXYZ = glm::vec3(centerX + 0.0f, centerY + -0.5f, centerZ + 0.0f);
	KettleBase.ShaderColor = SilverColor;
	KettleBase.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(KettleBase);

	//KettleLayerOne location, rotation, shader and mesh information
	RenderData KettleLayerOne;
	KettleLayerOne.scaleXYZ = glm::vec3(3.5f, 3.5f, 4.0f);
	KettleLayerOne.XrotationDegrees = 90.0f;
	KettleLayerOne.YrotationDegrees = 0.0f;
	KettleLayerOne.ZrotationDegrees = 0.0f;
	KettleLayerOne.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 0.25f, centerZ + 0.0f);
	KettleLayerOne.ShaderColor = SilverColor;
	KettleLayerOne.MeshType = MESHLIST::Torus;
	ShapeList.push_back(KettleLayerOne);

	//KettleLayerTwo location, rotation, shader and mesh information
	RenderData KettleLayerTwo;
	KettleLayerTwo.scaleXYZ = glm::vec3(3.25f, 3.25f, 4.0f);
	KettleLayerTwo.XrotationDegrees = 90.0f;
	KettleLayerTwo.YrotationDegrees = 0.0f;
	KettleLayerTwo.ZrotationDegrees = 0.0f;
	KettleLayerTwo.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 1.75f, centerZ + 0.0f);
	KettleLayerTwo.ShaderColor = SilverColor;
	KettleLayerTwo.MeshType = MESHLIST::Torus;
	ShapeList.push_back(KettleLayerTwo);

	//KettleLayerThree location, rotation, shader and mesh information
	RenderData KettleLayerThree;
	KettleLayerThree.scaleXYZ = glm::vec3(3.0f, 3.0f, 4.0f);
	KettleLayerThree.XrotationDegrees = 90.0f;
	KettleLayerThree.YrotationDegrees = 0.0f;
	KettleLayerThree.ZrotationDegrees = 0.0f;
	KettleLayerThree.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 3.25f, centerZ + 0.0f);
	KettleLayerThree.ShaderColor = SilverColor;
	KettleLayerThree.MeshType = MESHLIST::Torus;
	ShapeList.push_back(KettleLayerThree);

	//KettleLayerFour location, rotation, shader and mesh information
	RenderData KettleLayerFour;
	KettleLayerFour.scaleXYZ = glm::vec3(2.75f, 2.75f, 4.0f);
	KettleLayerFour.XrotationDegrees = 90.0f;
	KettleLayerFour.YrotationDegrees = 0.0f;
	KettleLayerFour.ZrotationDegrees = 0.0f;
	KettleLayerFour.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 4.75f, centerZ + 0.0f);
	KettleLayerFour.ShaderColor = SilverColor;
	KettleLayerFour.MeshType = MESHLIST::Torus;
	ShapeList.push_back(KettleLayerFour);

	//KettleLid location, rotation, shader and mesh information
	RenderData KettleLid;
	KettleLid.scaleXYZ = glm::vec3(2.75f, 0.5f, 2.75f);
	KettleLid.XrotationDegrees = 0.0f;
	KettleLid.YrotationDegrees = 0.0f;
	KettleLid.ZrotationDegrees = 0.0f;
	KettleLid.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 5.5f, centerZ + 0.0f);
	KettleLid.ShaderColor = SilverColor;
	KettleLid.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(KettleLid);

	//KettleLidHandleTube location, rotation, shader and mesh information
	RenderData KettleLidHandleTube;
	KettleLidHandleTube.scaleXYZ = glm::vec3(0.5f, 0.75f, 0.5f);
	KettleLidHandleTube.XrotationDegrees = 0.0f;
	KettleLidHandleTube.YrotationDegrees = 0.0f;
	KettleLidHandleTube.ZrotationDegrees = 0.0f;
	KettleLidHandleTube.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 6.0f, centerZ + 0.0f);
	KettleLidHandleTube.ShaderColor = SilverColor;
	KettleLidHandleTube.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(KettleLidHandleTube);


	//KettleLidHandleTop location, rotation, shader and mesh information
	RenderData KettleLidHandleTop;
	KettleLidHandleTop.scaleXYZ = glm::vec3(1.0f, 1.0f, 1.0f);
	KettleLidHandleTop.XrotationDegrees = 180.0f;
	KettleLidHandleTop.YrotationDegrees = 0.0f;
	KettleLidHandleTop.ZrotationDegrees = 0.0f;
	KettleLidHandleTop.positionXYZ = glm::vec3(centerX + 0.0f, centerY + 7.75f, centerZ + 0.0f);
	KettleLidHandleTop.ShaderColor = BlackColor;
	KettleLidHandleTop.ShaderMaterial = "wood";
	KettleLidHandleTop.MeshType = MESHLIST::TaperedCylinder;
	ShapeList.push_back(KettleLidHandleTop);


	//KettleHandleTop location, rotation, shader and mesh information
	RenderData KettleHandleTop;
	KettleHandleTop.scaleXYZ = glm::vec3(3.0f, 0.25f, 1.0f);
	KettleHandleTop.XrotationDegrees = 0.0f;
	KettleHandleTop.YrotationDegrees = 0.0f;
	KettleHandleTop.ZrotationDegrees = 0.0f;
	KettleHandleTop.positionXYZ = glm::vec3(centerX + -4.5f, centerY + 4.5f, centerZ + 0.0f);
	KettleHandleTop.ShaderColor = SilverColor;
	KettleHandleTop.MeshType = MESHLIST::Box;
	ShapeList.push_back(KettleHandleTop);

	//KettleHandleBottom location, rotation, shader and mesh information
	RenderData KettleHandleBottom;
	KettleHandleBottom.scaleXYZ = glm::vec3(0.25f, 4.0f, 1.0f);
	KettleHandleBottom.XrotationDegrees = 0.0f;
	KettleHandleBottom.YrotationDegrees = 0.0f;
	KettleHandleBottom.ZrotationDegrees = 0.0f;
	KettleHandleBottom.positionXYZ = glm::vec3(centerX + -6.125f, centerY + 2.625f, centerZ + 0.0f);
	KettleHandleBottom.ShaderColor = BlackColor;
	KettleHandleBottom.ShaderMaterial = "wood";
	KettleHandleBottom.MeshType = MESHLIST::Box;
	ShapeList.push_back(KettleHandleBottom);


	//KettleGooseNeckBottom location, rotation, shader and mesh information
	RenderData KettleGooseNeckBottom;
	KettleGooseNeckBottom.scaleXYZ = glm::vec3(0.25f, 1.25f, 0.25f);
	KettleGooseNeckBottom.XrotationDegrees = 0.0f;
	KettleGooseNeckBottom.YrotationDegrees = 0.0f;
	KettleGooseNeckBottom.ZrotationDegrees = 90.0f;
	KettleGooseNeckBottom.positionXYZ = glm::vec3(centerX + 5.0f, centerY + 0.75f, centerZ + 0.0f);
	KettleGooseNeckBottom.ShaderColor = SilverColor;
	KettleGooseNeckBottom.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(KettleGooseNeckBottom);

	//KettleGooseNeckMiddle location, rotation, shader and mesh information
	RenderData KettleGooseNeckMiddle;
	KettleGooseNeckMiddle.scaleXYZ = glm::vec3(0.25f, 4.0f, 0.25f);
	KettleGooseNeckMiddle.XrotationDegrees = 0.0f;
	KettleGooseNeckMiddle.YrotationDegrees = 0.0f;
	KettleGooseNeckMiddle.ZrotationDegrees = -20.0f;
	KettleGooseNeckMiddle.positionXYZ = glm::vec3(centerX + 4.85f, centerY + 0.65f, centerZ + 0.0f);
	KettleGooseNeckMiddle.ShaderColor = SilverColor;
	KettleGooseNeckMiddle.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(KettleGooseNeckMiddle);

	//KettleGooseNeckTop location, rotation, shader and mesh information
	RenderData KettleGooseNeckTop;
	KettleGooseNeckTop.scaleXYZ = glm::vec3(0.25f, 1.25f, 0.25f);
	KettleGooseNeckTop.XrotationDegrees = 0.0f;
	KettleGooseNeckTop.YrotationDegrees = 0.0f;
	KettleGooseNeckTop.ZrotationDegrees = 90.0f;
	KettleGooseNeckTop.positionXYZ = glm::vec3(centerX + 7.4f, centerY + 4.4f, centerZ + 0.0f);
	KettleGooseNeckTop.ShaderColor = SilverColor;
	KettleGooseNeckTop.MeshType = MESHLIST::Cylinder;
	ShapeList.push_back(KettleGooseNeckTop);

	//render stack at given location
	RenderList(ShapeList);

}