#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "ObjectsData.h"

class Project : public igl::opengl::glfw::Viewer
{
	
public:
	
	Project();
//	Project(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
	
	~Project(void);

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	std::vector<Layer> layers;
	/// <summary>
	/// represent the Camera ID
	/// </summary>
	int current_Camera;

	/// <summary>
	/// load mesh from path
	/// </summary>
	/// <param name="path"></param>
	/// <returns>
	/// return loaded object id  and -1 if fail
	/// </returns>
	int LoadMesh(std::string path);

	/// <summary>
	/// change object to a diffrent layer 
	/// if Layer dont exist create one
	/// if Layer is empty delete it
	/// </summary>
	/// <param name="layerNumber"></param>
	/// <returns>return old layer ID</returns>
	int changeLayer(int layerNumber);

	/// <summary>
	/// adds view camera at pos
	/// </summary>
	/// <param name="pos"></param>
	/// <returns>Camera ID</returns>
	int addCamera(Eigen::Vector3f pos);

	/// <summary>
	/// change view to a diffrent camera
	/// </summary>
	/// <param name="CameraID"></param>
	/// <returns>old Camera ID</returns>
	int changeCamera(int CameraID);

	/// <summary>
	/// change animtion start time
	/// for now view will always be at time = 0 due to picking
	/// </summary>
	/// <param name="time"></param>
	void changeTime(int time);

	/// <summary>
	/// change object apperance
	/// texture, Matiral 
	/// </summary>
	/// <param name="objectID"></param>
	/// <param name="TextureID"></param>
	/// <param name="ShaderID"></param>
	/// <param name="LayerID"></param>
	/// <returns>true on success and false on fail</returns>
	bool editobject(int objectID, int TextureID = -1, int MatiralID = -1);

	/// <summary>
	/// zoom in or out of current Camera 
	/// </summary>
	/// <param name="amount"></param>
	void zoom(float amount);
};


