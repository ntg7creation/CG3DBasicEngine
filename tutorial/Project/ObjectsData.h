#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include <tutorial/Project/BezierLine.h>

//note to self create proper decontractor for everthing 
enum object_type {Mesh,Camera,bezier_body,bezier_point };

/// <summary>
/// a moveable path with 1 bezier (bezier can be more than one segment)
/// </summary>
class myMoveable
{

	//std::vector<int> Control_Points_ID;// dont know if its needed
public:
	int time_start, time_end, objectindex;
	float speed;

	Bezier1D* bezier; // change void* to bezier object
	myMoveable(int timeS, int timeE, Bezier1D* bezier, int objectindex);
};



/// <summary>
/// a single object in 3D and all his data
/// </summary>
class ObjectData
{
	int objectID, TextureID, MatiralID, ShaderID, LayerID;
	//layerID -1 is invis
	//layerID -2 is cameras
	std::vector<myMoveable> path;
public:

	ObjectData();
	
	bool add_move(int timeS, int timeE, void* bezier);
	Eigen::Matrix4f getLoction(int time);
};
