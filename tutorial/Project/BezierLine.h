#pragma once
#include "igl/opengl/glfw/Viewer.h"



class IndexedModel
{
public:
	std::vector<Eigen::Vector3d> positions;
	std::vector<Eigen::Vector2f> texCoords;
	std::vector<Eigen::Vector3f> normals;
	std::vector<Eigen::Vector3f> colors;
	std::vector<Eigen::Vector3f> weights;
	//std::vector<glm::ivec3> joint_indices;
	std::vector<unsigned int> indices;

	void CalcNormals();
	inline void* GetData(int indx) const {
		switch (indx) {
		case 0: return (void*)&positions[0];
		case 1: return (void*)&colors[0];
		case 2: return (void*)&normals[0];
		case 3: return (void*)&weights[0];
		case 4: return (void*)&texCoords[0];
		case 5: return (void*)&indices[0];

		default: return (void*)0;
		}
	}

};

struct LineVertex
{
public:
	LineVertex(const Eigen::Vector3d& pos, const Eigen::Vector3f& color)
	{
		this->pos = pos;
		this->color = color;
	}

	Eigen::Vector3d* GetPos() { return &pos; }
	Eigen::Vector3f* GetColor() { return &color; }


private:
	Eigen::Vector3d pos;
	Eigen::Vector3f color;
};


//i remember there is a deconstractor problome 
class Bezier1D
{
	//each line in the matrix represents x,y,z,w of a control point
	std::vector<Eigen::Matrix4d> segments;
public:
	Bezier1D(void);
	int numberOfPoints;
	
	IndexedModel GetLine(int resT=50);						//generates model for rendering using MeshConstructor::initLin
	
	LineVertex GetVertex(int segment, float t);			//returns point on curve in the requested segment for value of t
	void MoveControlPoint(int segment, int indx, bool preserveC1, Eigen::Vector4d newPosition); //change the positon of one control point. when preserveC1 is true it may affect other  control points

	//currently not working											
	Eigen::Vector3d GetVelosity(int segment, float t);			//returns the derivative of the curve in the requested segment for value of t

	


	
	LineVertex GetControlPoint(int segment, int indx);	//returns a control point in the requested segment. indx will be 0,1,2,3, for p0,p1,p2,p3

};
