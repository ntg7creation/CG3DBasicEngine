#include "Project.h"
#include <iostream>


static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

static void printMat(const Eigen::Matrix4f& mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat(j, i) << " ";
		std::cout << std::endl;
	}
}
static void printline(char* str)
{
	std::cout<<str << std::endl;
}

static void printline(int str)
{
	std::cout << str << std::endl;
}



Project::myMoveable::myMoveable(int timeS, int timeE, Bezier1D* bezier, int objectindex, int CP0, int CP1, int CP2, int CP3)
{
	CPs.push_back(CP0);
	CPs.push_back(CP1);
	CPs.push_back(CP2);
	CPs.push_back(CP3);
	this->meshindex = objectindex;
	time_start = timeS;
	time_end = timeE;
	this->bezier = bezier;
	int cpnum= 0;

};
void Project::Connect_Controls(myMoveable object)
{
	Bezier1D* bezier = object.bezier;

	int cpnum = 0;
	int CP = object.CPs[cpnum];
	selected_data_index = CP;
	ShapeTransformation(xTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->z(), 0);
	cpnum = 1;
	CP = object.CPs[cpnum];
	selected_data_index = CP;
	ShapeTransformation(xTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->z(), 0);
	cpnum = 2;
	CP = object.CPs[cpnum];
	selected_data_index = CP;
	ShapeTransformation(xTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->z(), 0);
	cpnum = 3;
	CP = object.CPs[cpnum];
	selected_data_index = CP;
	ShapeTransformation(xTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, bezier->GetControlPoint(0, cpnum).GetPos()->z(), 0);
}


Project::Project()
{
}



//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}



int Project::LoadCubeMap(int matID)
{
	int shapeID = AddShape(Cube, -2, TRIANGLES);
	SetShapeMaterial(shapeID, 1);
	selected_data_index = shapeID;
	//float cylinderLen = 1.6f;
	float s = 60;
	ShapeTransformation(scaleAll, s, 0);
	SetShapeStatic(shapeID);
	return shapeID;
}
int Project::LoadMesh(shapes Shape, int matID, int shaderID, int parent)
{
	int	shapeID = AddShape(Shape, parent, TRIANGLES);
	SetShapeShader(shapeID, shaderID);
	SetShapeMaterial(shapeID, matID);
	return shapeID;
}
int Project::LoadMesh(std::string path, int matID, int shaderID, int parent)
{
	int	shapeID = AddShapeFromFile(path, parent, TRIANGLES);
	SetShapeShader(shapeID, shaderID);
	SetShapeMaterial(shapeID, matID);
	return shapeID;
}
int Project::LoadMesh(IndexedModel &mesh, int matID, int shaderID, int parent)
{
	int	shapeID = AddShape(Cube, parent, LINES);
	//SetShapeShader(shapeID, shaderID);
	//SetShapeMaterial(shapeID, matID);
	selected_data_index = shapeID;
	data()->show_faces = 0;
	data()->show_lines = 0;
	data()->show_overlay = 0xFF;
	SetShapeShader(shapeID, shaderID);
	SetShapeMaterial(shapeID, matID);
	//TODO change colour 
	for (int i = 0; i < mesh.positions.size() - 1; i++)
	{
		data()->add_edges(mesh.positions[i].transpose(), mesh.positions[i + 1].transpose(), Eigen::RowVector3d(0, 0, 0));
	}

	return shapeID;
}


int Project::editMesh(IndexedModel& mesh, int index)
{
	selected_data_index = index;
	int	shapeID = index;
	data_list[shapeID]->clear_edges();
	//SetShapeShader(shapeID, shaderID);
	//SetShapeMaterial(shapeID, matID);
	//data()->show_faces = 0;
	//data()->show_lines = 0;
	//data()->show_overlay = 0xFF;
	//SetShapeShader(shapeID, 2);
	//SetShapeMaterial(shaderID, 0);
	//SetShapeShader(shapeID, 2);
	//SetShapeMaterial(shaderID, 0);
	//TODO change colour 
	for (int i = 0; i < mesh.positions.size() - 1; i++)
	{
		data()->add_edges(mesh.positions[i].transpose(), mesh.positions[i + 1].transpose(), Eigen::RowVector3d(0, 0, 0));
	}

	return shapeID;
}
void Project::Animate_obj(int object_index, int animetionindex, float time)
{
	myMoveable path = bezierAnimations[animetionindex];
	if (path.time_start > time || path.time_end < time) {
		return;
	}
	float start = path.time_start;
	float end = path.time_end;
	float progress = (time - start) / end;

	Eigen::Vector3d* new_pos = path.bezier->GetVertex(0,progress).GetPos();
	Eigen::Vector3d old_pos = data_list[object_index]->GetPos();
	selected_data_index = object_index;
	ShapeTransformation(xTranslate, new_pos->x() - old_pos.x(), false);
	ShapeTransformation(yTranslate, new_pos->y() - old_pos.y(), false);
	ShapeTransformation(zTranslate, new_pos->z() - old_pos.z(), false);


}
int CP2;
int Project::addbezier(int meshindex)
{
	if (data_list[meshindex]->animtoinindex < 0) {
		int defaultmat = 0;
		int defaultsader = 2;
		//controls int
		Bezier1D* myBezier = new Bezier1D(); // check not destoryed
		int CP0 = LoadMesh(Octahedron, defaultmat, defaultsader);
		data_list[CP0]->iscontrolpoint = true;
		selected_data_index = CP0;
		ShapeTransformation(scaleAll, 0.1, 0);
		int CP1 = LoadMesh(Octahedron, defaultmat, defaultsader);
		data_list[CP1]->iscontrolpoint = true;
		selected_data_index = CP1;
		ShapeTransformation(scaleAll, 0.1, 0);
		CP2 = LoadMesh(Octahedron, defaultmat, defaultsader);
		data_list[CP2]->iscontrolpoint = true;
		selected_data_index = CP2;
		ShapeTransformation(scaleAll, 0.1, 0);
		int CP3 = LoadMesh(Octahedron, defaultmat, defaultsader);
		data_list[CP3]->iscontrolpoint = true;
		selected_data_index = CP3;
		ShapeTransformation(scaleAll, 0.1, 0);


		int bezierlineID = LoadMesh(myBezier->GetLine(), 1, 2);
		myMoveable tempmove = myMoveable(0, 1, myBezier, bezierlineID, CP0, CP1, CP2, CP3);
		bezierAnimations.push_back(tempmove);
		int animtionindex = bezierAnimations.size() - 1;
		Connect_Controls(tempmove);


		connect_bezier_to_mesh(meshindex, animtionindex);
		return animtionindex;
	}
	else
	{
		//TODO add segment
	}
	return -1;
}
//we translate mesh i
//if animtoinindex >= 0 call this 
void Project::connect_bezier_to_mesh(int meshindex, int animetionindex) 
{
	myMoveable moveable = bezierAnimations[animetionindex];
	Eigen::Vector3d new_pos = data_list[meshindex]->GetPos();
	//Eigen::Vector3d new_pos2 = data_list[meshindex]->GetPos2();
	Eigen::Vector3d old_pos = data_list[moveable.CPs[0]]->GetPos(); // pos of cp0

	for (int i = 0; i < moveable.CPs.size(); i++)
	{
		translateControl_no_update(xTranslate, new_pos.x() - old_pos.x(), moveable, i, false);
		translateControl_no_update(yTranslate, new_pos.y() - old_pos.y(), moveable, i, false);
		translateControl_no_update(zTranslate, new_pos.z() - old_pos.z(), moveable, i, false);
	}

	editMesh(moveable.bezier->GetLine(), moveable.meshindex);
	data_list[meshindex]->animtoinindex = animetionindex;
	
}
void Project::translateControl_no_update(int type, float amt, myMoveable obj, int cpnum, bool preserve)
{
	int mesh_index = obj.CPs[cpnum];
	selected_data_index = mesh_index;
	ShapeTransformation(type, amt, 0);

	Eigen::Vector4d posv4 = Eigen::Vector4d();
	Eigen::Vector3d pos = data_list[mesh_index]->GetPos();
	posv4 << pos.x(), pos.y(), pos.z(), 0;
	obj.bezier->MoveControlPoint(0, cpnum, false, posv4);
	if (preserve)
	{
		//TODO
	}
}
void Project::translateControl(int type, float amt,int mesh_index,bool preserve)
{
	selected_data_index = mesh_index;
	ShapeTransformation(type, amt, 0);
	myMoveable* mybez;
	int CP_num = -1;
	for(int i =0;i<bezierAnimations.size();i++)
	{
		for(CP_num = 0; CP_num < bezierAnimations[i].CPs.size(); CP_num++)
		{
			if (bezierAnimations[i].CPs[CP_num] == mesh_index)
			{
				mybez = &bezierAnimations[i];
				break;
			}
		}
		if (i == bezierAnimations.size())
		{
			// if we got here that means that mesh_index is not a control point;
			// maybe we need to change it to add a flag to mesh_index
			return;
		}
		break;
	}




	Eigen::Vector4d posv4 = Eigen::Vector4d();
	Eigen::Vector3d pos = data_list[mesh_index]->GetPos();
	posv4 << pos.x(), pos.y(), pos.z(), 0;
	mybez->bezier->MoveControlPoint(0, CP_num, false, posv4);
	if (preserve)
	{
		//TODO
	}
	editMesh(mybez->bezier->GetLine(),mybez->meshindex);
	selected_data_index = mesh_index;
}

int Project::addgridmesh(int resT)
{
	IndexedModel mesh = IndexedModel();
	Eigen::Vector3d pos = Eigen::Vector3d();
	pos << 0, 0, 0;
	for (int i = 0; i < resT; i++)
	{
		for (int j = 0; j < resT; j++)
		{
			pos << i, 0, j;
			mesh.positions.push_back(pos);
		}
	}
	int MatID = 0;
	int watershader = 3;

	int	shapeID = AddShape(Cube, -1, TRIANGLES);

	selected_data_index = shapeID;
	data()->show_faces = 0;
	data()->show_lines = 0;
	data()->show_overlay = 0xFF;
	SetShapeShader(shapeID, watershader);
	SetShapeMaterial(shapeID, MatID);
	for (int i = 0; i < resT; i++)
	{
		for (int j = 0; j < resT; j++)
		{
			if(j<resT-1)
				data()->add_edges(mesh.positions[i * resT + j].transpose(), mesh.positions[i * resT + j + 1].transpose(), Eigen::RowVector3d(0, 0, 0));
			if (i < resT - 1)
				data()->add_edges(mesh.positions[i * resT + j].transpose(), mesh.positions[i * resT + j + resT].transpose(), Eigen::RowVector3d(1, 0, 0));
		}
	}

	return shapeID;
	return LoadMesh(mesh,0,2);


}

void Project::Init()
{
	int cubeID = -1;

	//basic int
	{
		unsigned int texIDs[3] = { 0 , 1, 2 };
		unsigned int slots[3] = { 0 , 1, 2 };
		AddShader("shaders/pickingShader");
		AddShader("shaders/cubemapShader");
		AddShader("shaders/basicShader");
		AddShader("shaders/waterShader");
		AddShader("shaders/pickingShader");
		int textureID = 0;
		textureID = AddTexture("textures/plane.png", 2);
		textureID = AddTexture("textures/cubemaps/Daylight Box_", 3);
		textureID = AddTexture("textures/grass.bmp", 2);
		textureID = AddTexture("textures/water.jpg", 2);
		//AddTexture("../res/textures/Cat_bump.jpg", 2);

		AddMaterial(texIDs, slots, 1);
		AddMaterial(texIDs + 1, slots + 1, 1);
		AddMaterial(texIDs + 2, slots + 2, 1);
		AddMaterial(texIDs + 3, slots + 3, 1);
		int shapeID = 0;
		LoadCubeMap(1);
	}
	//load cube
	{
		cubeID = LoadMesh(Cube, 0, 2);
		selected_data_index = cubeID;
		ShapeTransformation(yTranslate, 1, 0);
	}
	//add bezier 
	addbezier(cubeID);

	//moving mesh that is not a control point
	selected_data_index = cubeID;
	ShapeTransformation(yTranslate,2, 0);
	connect_bezier_to_mesh(cubeID, data_list[cubeID]->animtoinindex);

	//addgrid
	int id = addgridmesh(10);
	selected_data_index = id;
	ShapeTransformation(yTranslate, -1.5, 0);

	//add a plane for multipick
	int id2 = AddShape(Plane, -2, TRIANGLES, 1);
	SetShapeShader(id2, 4);
	SetShapeMaterial(id2, 0);
	

	selected_data_index = id2;
	ShapeTransformation(zTranslate, -1.1, 1);

	SetShapeStatic(id2);

}


void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{


	//Animate_obj(cubeID,bezierAnimations[0], mytime);
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;
	//rgb -> id

		s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 60 / 100.0f, 99 / 100.0f, 0.5f);
	//textures[0]->Bind(0);

	
	

	//s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else 
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}


void Project::WhenRotate()
{
}

void Project::WhenTranslate()
{
}

void Project::Animate() {

	/*int temp = selected_data_index;
	mytime += tick;
	for (int i = 0; i < data_list.size(); i++)
		if (data_list[i]->animtoinindex >= 0)
			Animate_obj(i, data_list[i]->animtoinindex, mytime);

	translateControl(yTranslate, -0.01, CP2, false);
	selected_data_index = temp;*/

    if(isActive)
	{
		int temp = selected_data_index;
		mytime += tick;
		for (int i = 0; i < data_list.size(); i++)
			if (data_list[i]->animtoinindex >= 0)
				Animate_obj(i, data_list[i]->animtoinindex, mytime);
		/*if(selected_data_index > 0 )
			data()->MyRotate(Eigen::Vector3d(0, 1, 0), 0.01);*/
		selected_data_index = temp;
	}
}

void Project::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Project::~Project(void)
{
}

