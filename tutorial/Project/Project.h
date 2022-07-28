#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "ObjectsData.h"
#include "BezierLine.h"





class Project : public igl::opengl::glfw::Viewer
{
	
public:
	int cubeMapShapeID;
	int InitCubeMap(int matID); 
	int numObjectsTextures;
	int numCubeMapTextures;

	int ticksCounter;
	int cubeID = -1;
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

	struct myMoveable
	{

		//std::vector<int> Control_Points_ID;// dont know if its needed
	public:
		//TODO add what object do i need to animate
		float time_start, time_end;
		int meshindex;// meshindex = the index of the bizer line in datalist
		float speed;
		std::vector<int> CPs; // loction = control num - value = CP_mesh_index
		Bezier1D* bezier; // change void* to bezier object
		myMoveable(int timeS, int timeE, Bezier1D* bezier, int objectindex, int CP1, int CP2, int CP3, int CP4);

	};



	void Connect_Controls(myMoveable);

	/// <summary>
	/// represent the Camera ID
	/// </summary>
	int current_Camera;
	int tempcontrolindex;
	float mytime = 0;
	const float tick = 0.00166*3; // asume 1 update a sec time =1 means 1 min
	myMoveable find_Control(int mesh_index);
	std::vector<myMoveable> bezierAnimations;
	/// <summary>
	/// load mesh
	/// </summary>
	/// <param name="path"></param>
	/// <returns>
	/// return loaded object id  and -1 if fail
	/// </returns>
	void LoadCubeMap(int matID);
	int LoadMesh(std::string path,int matID,int shaderID,int parent = -1);
	int LoadMesh(shapes Shape, int matID, int shaderID, int parent = -1);
	int LoadMesh(IndexedModel &mesh, int matID, int shaderID, int parent = -1);

	int editMesh(IndexedModel& mesh, int index);
	void translateControl(int type, float amt,int mesh_index,bool preserve);
	void translateControl(int mesh_index,bool preserve);
	void translateControl_no_update(int type, float amt, myMoveable obj,int cpnum, bool preserve);
	void connect_bezier_to_mesh(int meshindex, int animetionindex);
	int addbezier(int meshindex);

	void Animate_obj(int object_index, int animetionindex,float time);

	int addgridmesh(int resT);
	
	void changelayer(int layer,int objectindex);
	int hidelayer(int layer);
	int unhidelayer(int layer);

	int testloadcostomemesh();


	void moveCamera(Eigen::Vector3d pos);

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
	/// zoom in or out of current Camera 
	/// </summary>
	/// <param name="amount"></param>
	void zoom(float amount);
};


