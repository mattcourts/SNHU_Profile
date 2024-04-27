#include "AssetLoader.h"

void AssetLoader::RenderAssets(std::vector<GeometricShape> AssetList, std::function<void(glm::vec3, float, float, float, glm::vec3)> SetTransformations, std::function<void(float, float, float, float)> SetShaderColor, std::function<void(std::string)> SetShaderTexture, std::function<void(float, float)> SetTextureUVScale, std::function<void(std::string)> SetShaderMaterial, ShapeMeshes* m_basicMeshes, glm::vec3 CenterPoint)
{
	for (GeometricShape Asset : AssetList)
	{
		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			Asset.scaleXYZ,
			Asset.XrotationDegrees,
			Asset.YrotationDegrees,
			Asset.ZrotationDegrees,
			Asset.positionXYZ + CenterPoint
		);

		//Set Color, Texture, UVScale, and Material
		if (Asset.ShaderColor != glm::vec4{}) { SetShaderColor(Asset.ShaderColor.r, Asset.ShaderColor.g, Asset.ShaderColor.b, Asset.ShaderColor.a); }
		if (Asset.ShaderTexture != "") { SetShaderTexture(Asset.ShaderTexture); }
		if (Asset.TextureUVScale != glm::vec2{}) { SetTextureUVScale(Asset.TextureUVScale.x, Asset.TextureUVScale.y); }
		if (Asset.ShaderMaterial != "") { SetShaderMaterial(Asset.ShaderMaterial); }

		ShapeMeshes * ShapeMeshRender = m_basicMeshes;

		switch (Asset.MeshType)
		{
		case MESHLIST::Cone: ShapeMeshRender.DrawConeMesh();
		case MESHLIST::ConeNoBottom: ShapeMeshRender.DrawConeMesh(false);
		case MESHLIST::Cylinder: ShapeMeshRender.DrawCylinderMesh();
		case MESHLIST::CylinderNoTop: ShapeMeshRender.DrawCylinderMesh(false, true, true);
		case MESHLIST::CylinderNoBottom: ShapeMeshRender.DrawCylinderMesh(true, false, true);
		case MESHLIST::CylinderOpen: ShapeMeshRender.DrawCylinderMesh(false, false, true);
		case MESHLIST::Plane: ShapeMeshRender.DrawPlaneMesh();
		case MESHLIST::Prism: ShapeMeshRender.DrawPrismMesh();
		case MESHLIST::Pyramid3: ShapeMeshRender.DrawPyramid3Mesh();
		case MESHLIST::Pyramid4: ShapeMeshRender.DrawPyramid4Mesh();
		case MESHLIST::Sphere: ShapeMeshRender.DrawSphereMesh();
		case MESHLIST::HalfSphere: ShapeMeshRender.DrawHalfSphereMesh();
		case MESHLIST::TaperedCylinder: ShapeMeshRender.DrawTaperedCylinderMesh();
		case MESHLIST::TaperedCylinderNoTop: ShapeMeshRender.DrawTaperedCylinderMesh(false, true, true);
		case MESHLIST::TaperedCylinderNoBottom: ShapeMeshRender.DrawTaperedCylinderMesh(true, false, true);
		case MESHLIST::TaperedCylinderOpen: ShapeMeshRender.DrawTaperedCylinderMesh(false, false, true);
		case MESHLIST::Torus: ShapeMeshRender.DrawTorusMesh();
		case MESHLIST::HalfTorus: ShapeMeshRender.DrawHalfTorusMesh();
		}
	}
}
