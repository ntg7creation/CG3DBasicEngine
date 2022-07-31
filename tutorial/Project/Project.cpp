#include "Project.h"
#include "igl/frustum.h"
#include <iostream>
#include "../../igl/file_dialog_open.h"
#include "../../igl/file_dialog_save.h"

#define M_PI 3.1415926535897932384626433832795
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
	//int cpnum= 0;

};
void Project::Connect_Controls(myMoveable object)
{
	Bezier1D* bezier = object.bezier;

	for (int cpnum = 0; cpnum < object.CPs.size(); cpnum++) {
		int CP_mesh_index = object.CPs[cpnum];
		selected_data_index = CP_mesh_index; 
		int segmentnum = (cpnum -1) / 3 ;
		int CP_in_segment = cpnum%3;
		if (segmentnum < 0)
			segmentnum = 0;
		if (CP_in_segment == 0 && cpnum != 0)
			CP_in_segment = 3;
		float temp = bezier->GetControlPoint(segmentnum, CP_in_segment).GetPos()->x();
		float temp2 = bezier->GetControlPoint(segmentnum, CP_in_segment).GetPos()->x() - data_list[CP_mesh_index]->GetPos().x();
		ShapeTransformation(xTranslate, bezier->GetControlPoint(segmentnum, CP_in_segment).GetPos()->x()- data_list[CP_mesh_index]->GetPos().x(), 0);
		ShapeTransformation(yTranslate, bezier->GetControlPoint(segmentnum, CP_in_segment).GetPos()->y() - data_list[CP_mesh_index]->GetPos().y(), 0);
		ShapeTransformation(zTranslate, bezier->GetControlPoint(segmentnum, CP_in_segment).GetPos()->z() - data_list[CP_mesh_index]->GetPos().z(), 0);
	}
	/*
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
	*/
}


Project::Project()
{
	ticksCounter = 0;
}


int Project::InitCubeMap(int matID)
{
	int shapeID = AddShape(Cube, -2, TRIANGLES);
	SetShapeMaterial(shapeID, matID);
	selected_data_index = shapeID;
	//float cylinderLen = 1.6f;
	float s = 160;
	ShapeTransformation(scaleAll, s, 0);
	SetShapeStatic(shapeID);
	return shapeID;
}


//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}
void Project::SetCubeMap(int matID) {
	SetShapeMaterial(cubeMapShapeID, matID + numObjectsTextures);
}
void Project::SetMaterialOfPickedObjs(int matID) {
	//std::cout << "inside SetMaterialOfPickedObjs()" << std::endl;

	pShapes.push_back(2);// TODO : remove debug line

	// for all picked Shapes (inside pShapes or pickedShapes)
	for (int index : pShapes) {
		std::cout << "calling SetMaterial() for shape id: " << index << std::endl;
		data_list[index]->SetMaterial(matID);
	}
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
	int temp = selected_data_index;
	selected_data_index = index;
	int	shapeID = index;
	data_list[shapeID]->clear_edges();
 
	for (int i = 0; i < mesh.positions.size() - 1; i++)
	{
		data()->add_edges(mesh.positions[i].transpose(), mesh.positions[i + 1].transpose(), Eigen::RowVector3d(0, 0, 0));
	}
	selected_data_index = temp;
	return shapeID;
}
void Project::Animate_obj(int object_index, int animetionindex, float time)
{
	myMoveable path = bezierAnimations[animetionindex];
	if (path.time_start > time || path.time_end < time|| path.bezier->segments.size() == 0){
		return;
	}

	float start = path.time_start;
	float end = path.time_end;
	float segmentcount = path.bezier->segments.size();

	float total_segment_prog = (time - start) / (end - start);
	float singel_segment_prog = total_segment_prog *segmentcount;
	int current_semgent = singel_segment_prog;

	Eigen::Vector3d* new_pos = path.bezier->GetVertex(current_semgent, singel_segment_prog - current_semgent).GetPos();
	Eigen::Vector3d old_pos = data_list[object_index]->GetPos();
	selected_data_index = object_index;
	ShapeTransformation(xTranslate, new_pos->x() - old_pos.x(), false);
	ShapeTransformation(yTranslate, new_pos->y() - old_pos.y(), false);
	ShapeTransformation(zTranslate, new_pos->z() - old_pos.z(), false);


}
int CP2;
int idBlend;
int Project::addbezier(int meshindex)
{
	int defaultmat = 0;
	int defaultsader = 2;
	if (data_list[meshindex]->animtoinindex < 0) {

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
		myMoveable* mv = &bezierAnimations[data_list[meshindex]->animtoinindex];
		int CProot = mv->CPs.size() - 1;
		int CP_mesh_index = LoadMesh(Octahedron, defaultmat, defaultsader);
		mv->CPs.push_back(CP_mesh_index);
		selected_data_index = CP_mesh_index;
		ShapeTransformation(scaleAll, 0.1, 0);
		data_list[CP_mesh_index]->iscontrolpoint = true;

		CP_mesh_index = LoadMesh(Octahedron, defaultmat, defaultsader);
		mv->CPs.push_back(CP_mesh_index);
		selected_data_index = CP_mesh_index;
		ShapeTransformation(scaleAll, 0.1, 0);
		data_list[CP_mesh_index]->iscontrolpoint = true;

		CP_mesh_index = LoadMesh(Octahedron, defaultmat, defaultsader);
		mv->CPs.push_back(CP_mesh_index);
		selected_data_index = CP_mesh_index;
		ShapeTransformation(scaleAll, 0.1, 0);
		data_list[CP_mesh_index]->iscontrolpoint = true;

		Eigen::Matrix4d mat = Eigen::Matrix4d();
		double x = data_list[mv->CPs[CProot]]->GetPos().x();
		double y = data_list[mv->CPs[CProot]]->GetPos().y();
		double z = data_list[mv->CPs[CProot]]->GetPos().z();
		mat << x, y, z, 0.0,
			x + 1, y+1, z, 0.0,
			x + 2, y-1, z, 0.0,
			x + 3, y, z+1, 0.0;


		mv->bezier->segments.push_back(mat);

		Connect_Controls(*mv);

		editMesh(mv->bezier->GetLine(), mv->meshindex);


		//mv.

	}
	return -1;
}

//we translate mesh i
//if animtoinindex >= 0 call this 
void Project::connect_bezier_to_mesh(int meshindex, int animetionindex) 
{
	int temp = selected_data_index;
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

	selected_data_index = temp;
	
}

void Project::fix_bezier_to_mesh(int meshindex)
{
	
	if (data_list[meshindex]->animtoinindex < 0)
		return;
	int temp = selected_data_index;
	
	myMoveable moveable = bezierAnimations[data_list[meshindex]->animtoinindex];
	Eigen::Vector3d new_pos = data_list[meshindex]->GetPos2() + data_list[meshindex]->GetPos();
	//Eigen::Vector3d new_pos = data_list[meshindex]->GetPos2() ;
	//Eigen::Vector3d new_pos = data_list[meshindex]->GetPos();
	//Eigen::Vector3d new_pos2 = data_list[meshindex]->GetPos2();
	Eigen::Vector3d old_pos = data_list[moveable.CPs[0]]->GetPos(); // pos of cp0

	//std::cout << "newpos:" << new_pos << " - oldpos:" << old_pos << std::endl;
	for (int i = 0; i < moveable.CPs.size(); i++)
	{
		selected_data_index = moveable.CPs[i];
		float tt = new_pos.y() - old_pos.y();
		ShapeTransformation(xTranslate, new_pos.x() - old_pos.x(), 0);
		ShapeTransformation(yTranslate, new_pos.y() - old_pos.y(), 0);
		ShapeTransformation(zTranslate, new_pos.z() - old_pos.z(), 0);
	}
	selected_data_index = moveable.meshindex;
	ShapeTransformation(xTranslate, new_pos.x() - old_pos.x(), 0);
	ShapeTransformation(yTranslate, new_pos.y() - old_pos.y(), 0);
	ShapeTransformation(zTranslate, new_pos.z() - old_pos.z(), 0);

	//editMesh(moveable.bezier->GetLine(), moveable.meshindex);
	//data_list[meshindex]->animtoinindex = animetionindex;

	selected_data_index = temp;

}

void Project::translateControl_no_update(int type, float amt, myMoveable obj, int cpnum, bool preserve)
{
	int mesh_index = obj.CPs[cpnum];
	selected_data_index = mesh_index;
	ShapeTransformation(type, amt, 0);

	Eigen::Vector4d posv4 = Eigen::Vector4d();
	Eigen::Vector3d pos = data_list[mesh_index]->GetPos();
	posv4 << pos.x(), pos.y(), pos.z(), 0;

	int segmentnum = (cpnum - 1) / 3;
	int CP_in_segment = cpnum % 3;
	if (segmentnum < 0)
		segmentnum = 0;
	if (CP_in_segment == 0 && cpnum != 0)
		CP_in_segment = 3;

	obj.bezier->MoveControlPoint(segmentnum, CP_in_segment, false, posv4);
	if (preserve)
	{
		//TODO
	}
}

int Project::get_CP0(int meshidx) {
	int animtoinindex = data_list[meshidx]->animtoinindex;
	if (animtoinindex < 0)
		return -1;
	return bezierAnimations[animtoinindex].CPs[0];
}

void Project::translateControl(int type, float amt,int mesh_index,bool preserve)
{
	selected_data_index = mesh_index;
	ShapeTransformation(type, amt, 0);
	myMoveable* mybez = NULL;
	int CP_num = -1;
	for(int i =0;i<bezierAnimations.size() && mybez == NULL;i++)
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
		//break;
	}



	Eigen::Vector4d posv4 = Eigen::Vector4d();
	//Eigen::Vector3d pos = data_list[mesh_index]->GetPos();
	Eigen::Vector3d pos = data_list[mesh_index]->GetPos()+ data_list[mesh_index]->GetPos2();
	posv4 << pos.x(), pos.y(), pos.z(), 0;


	int segmentnum = (CP_num - 1) / 3;
	int CP_in_segment = CP_num % 3;
	if (segmentnum < 0)
		segmentnum = 0;
	if (CP_in_segment == 0 && CP_num != 0)
		CP_in_segment = 3;
	mybez->bezier->MoveControlPoint(segmentnum, CP_in_segment, false, posv4);
	if (segmentnum +1 < (int)(mybez->CPs.size() / 3) && CP_in_segment == 3)
	{
		mybez->bezier->MoveControlPoint(segmentnum+1, 0, false, posv4);
	}
	if (preserve)
	{
		//TODO
	}
	editMesh(mybez->bezier->GetLine(),mybez->meshindex);
	//selected_data_index = mesh_index;
}
void Project::translateControl( int mesh_index, bool preserve)
{

	myMoveable* mybez = NULL;
	int CP_num = -1;
	for (int i = 0; i < bezierAnimations.size() && mybez == NULL; i++)
	{
		for (CP_num = 0; CP_num < bezierAnimations[i].CPs.size(); CP_num++)
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
		//break;
	}



	Eigen::Vector4d posv4 = Eigen::Vector4d();
	Eigen::Vector3d pos = data_list[mesh_index]->GetPos()+ data_list[mesh_index]->GetPos2();
	posv4 << pos.x(), pos.y(), pos.z(), 0;


	int segmentnum = (CP_num - 1) / 3;
	int CP_in_segment = CP_num % 3;
	if (segmentnum < 0)
		segmentnum = 0;
	if (CP_in_segment == 0 && CP_num != 0)
		CP_in_segment = 3;
	mybez->bezier->MoveControlPoint(segmentnum, CP_in_segment, false, posv4);
	if (segmentnum + 1 < (int)(mybez->CPs.size() / 3) && CP_in_segment == 3)
	{
		mybez->bezier->MoveControlPoint(segmentnum + 1, 0, false, posv4);
	}
	if (preserve)
	{
		//TODO
	}
	editMesh(mybez->bezier->GetLine(), mybez->meshindex);
	//selected_data_index = mesh_index;
	
	/*
	//selected_data_index = mesh_index;
	//ShapeTransformation(type, amt, 0);
	myMoveable* mybez = NULL;
	int CP_num = -1;
	for (int i = 0; i < bezierAnimations.size() && mybez == NULL; i++)
	{
		for (CP_num = 0; CP_num < bezierAnimations[i].CPs.size(); CP_num++)
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
		//break;
	}



	Eigen::Vector4d posv4 = Eigen::Vector4d();
	Eigen::Vector3d pos = data_list[mesh_index]->GetPos();
	posv4 << pos.x(), pos.y(), pos.z(), 0;


	int segmentnum = (CP_num - 1) / 3;
	int CP_in_segment = CP_num % 3;
	if (segmentnum < 0)
		segmentnum = 0;
	if (CP_in_segment == 0 && CP_num != 0)
		CP_in_segment = 3;

	mybez->bezier->MoveControlPoint(segmentnum, CP_in_segment, false, posv4);
	if (preserve)
	{
		//TODO
	}
	editMesh(mybez->bezier->GetLine(), mybez->meshindex);*/
}

//not used
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


void Project::addLayer() {
	numOfLayers += 1;
}


int Project::hidelayer(int layer)
{
	int count = 0;
	for each (igl::opengl::ViewerData* var in data_list)
	{
		if (var->layer == layer)
		{
			count++;
			var->hide = true;
		}
	}
	return 0;
}
int Project::unhidelayer(int layer)
{
	int count = 0;
	for each (igl::opengl::ViewerData * var in data_list)
	{
		if (var->layer == layer)
		{
			count++;
			var->hide = false;
		}
	}
	return 0;
}
void Project::hide_editor() {
	for (int mesh = 0; mesh < Cameras.size(); mesh++)
		data_list[abs(Cameras[mesh])]->hide = true;
	for (int mesh = 0; mesh < bezierAnimations.size(); mesh++) {
		myMoveable* bez = &bezierAnimations[mesh];
		data_list[bez->meshindex]->hide = true;
		
		for (int i = 0; i < bez->CPs.size(); i++)
			data_list[bez->CPs[i]]->hide = true;
	}
		
}
void Project::unhide_editor() {
	for (int mesh = 0; mesh < Cameras.size(); mesh++)
		data_list[abs(Cameras[mesh])]->hide = false;
	for (int mesh = 0; mesh < bezierAnimations.size(); mesh++) {
		myMoveable* bez = &bezierAnimations[mesh];
		data_list[bez->meshindex]->hide = false;

		for (int i = 0; i < bez->CPs.size(); i++)
			data_list[bez->CPs[i]]->hide = false;
	}
}
void Project::changelayer(int layer, int objectindex)
{
	data_list[objectindex]->layer = layer;
}


int Project::addCamera(Eigen::Vector3f pos) {
	int temp = selected_data_index;
	int Camera = LoadMesh(Cube, 3, 2);
	selected_data_index = Camera;
	//ShapeTransformation(scaleAll, 0.3, 0);
	ShapeTransformation(xTranslate, pos.x(), 0);
	ShapeTransformation(yTranslate, pos.y(), 0);
	ShapeTransformation(zTranslate, pos.z(), 0); 
	Cameras.push_back(Camera);
	selected_data_index = temp;
	return Camera;
}
int Project::addCamera2(Eigen::Vector3f pos) {
	int temp = selected_data_index;
	int Camera = LoadMesh(Cube, 4, 2);
	selected_data_index = Camera;
	//ShapeTransformation(scaleAll, 0.3, 0);
	ShapeTransformation(xTranslate, pos.x(), 0);
	ShapeTransformation(yTranslate, pos.y(), 0);
	ShapeTransformation(zTranslate, pos.z(), 0); 
	Cameras.push_back(-Camera);
	selected_data_index = temp;
	return Camera;
}

void Project::moveCamera(Eigen::Vector3d newpos)
{
	int temp = selected_data_index;
	int Camera_index = Cameras[current_Camera];
	if (Camera_index < 0)
		Camera_index *= -1;
	Movable* Camera_mesh = data_list[Camera_index];
	selected_data_index = Camera_index;
	Eigen::Vector3d oldpos = data_list[Camera_index]->GetPos();
	Eigen::Vector3d diffpos = newpos - oldpos;
	ShapeTransformation(xTranslate, diffpos.x(), 0);
	ShapeTransformation(yTranslate, diffpos.y(), 0);
	ShapeTransformation(zTranslate, diffpos.z(), 0);

	selected_data_index = temp;
}


void Project::Init()
{
	
	unsigned int objectTextureIDs[] = {
		AddTexture("textures/plane.png", 2),
		AddTexture("textures/grass.bmp", 2),
		AddTexture("textures/water.bmp", 2),
		AddTexture("textures/Camera.png", 2),
		AddTexture("textures/Camera2.png", 2),
	};

	unsigned int cubeMapTextureIDs[] = { 
		AddTexture("textures/cubemaps/Daylight Box_", 3),
		AddTexture("textures/cubemaps/cube3/3_", 3),
		//AddTexture("textures/cubemaps/cube2/output_skybox_", 3),
	};

	numObjectsTextures = sizeof(objectTextureIDs) / sizeof(unsigned int);
	numCubeMapTextures = sizeof(cubeMapTextureIDs) / sizeof(unsigned int);

	int numSlots = numCubeMapTextures + numObjectsTextures;
	unsigned int *slots = new unsigned int[numSlots];

	for (int i = 0; i < numSlots; ++i) {
		slots[i] = i;
	}
	for (int i = 0; i < numObjectsTextures; ++i) {
		AddMaterial(objectTextureIDs + i, slots + i, 1);
	}
	for (int i = 0, j = numObjectsTextures; i < numCubeMapTextures; ++i) {
		AddMaterial(cubeMapTextureIDs + i, slots + j, 1);
	}

	cubeMapShapeID = InitCubeMap(cubeMapTextureIDs[0]);

	AddShader("shaders/pickingShader");
	AddShader("shaders/cubemapShader");
	int basicshader =AddShader("shaders/basicShader");
	watershader = AddShader("shaders/waterShader");
	transparentshader = AddShader("shaders/transparent");

	//add Camera on start
	addCamera(Eigen::Vector3f(0, 0, 10));
	current_Camera = Cameras.size() - 1;

	//add camera 2
	selected_data_index = addCamera2(Eigen::Vector3f(5, 0, 0));

	//add camera with bezier
	int camera3 = addCamera(Eigen::Vector3f(-4, 3, 0));
	temp = camera3;
	selected_data_index = camera3;

	//add cube with bezier 
	cubeID = LoadMesh(Cube, 0, 2);
	selected_data_index = cubeID;
	ShapeTransformation(yTranslate, 3, 0);
	//add bezier 
	addbezier(cubeID);
	//connect_bezier_to_mesh(cubeID, data_list[cubeID]->animtoinindex); DO NOT delete find bug in case you call it 2 time but it not a bug if called out side of function
	//add segment
	addbezier(cubeID);
	addbezier(cubeID);

//yadern

	//add a plane for multipick
	//int id2 = AddShape(Plane, -2, TRIANGLES, 1);
	//SetShapeShader(id2, transparentshader);
	//SetShapeMaterial(id2, 0);
	//
	//selected_data_index = id2;
	//ShapeTransformation(zTranslate, -1.1, 1);
 //   	SetShapeStatic(id2);


	//// attempt to load a transparent object
	//int id3 = AddShape(Plane, -1, TRIANGLES);
	//idBlend = id3;
	////data_list[id3]->AddViewport(3);
	//SetShapeShader(id3, 2);
	//SetShapeMaterial(id3, 0);
	//selected_data_index = id3;
	//ShapeTransformation(xTranslate, 2, 1);
	//ShapeTransformation(yTranslate, 2, 1);
	////ShapeTransformation(zTranslate, -1.1, 1);
	////std::cout << idBlend << std::endl;

	//int id4 = AddShape(Cube, -1, TRIANGLES);
	//SetShapeShader(id4, 2);
	//SetShapeMaterial(id4, 0);
	//selected_data_index = id4;
	//ShapeTransformation(xTranslate, -2, 1);
	//ShapeTransformation(yTranslate, -2, 1);
	

//natai    
	addbezier(camera3);
	connect_bezier_to_mesh(camera3, data_list[camera3]->animtoinindex);



	//addgrid
	//int id = addgridmesh(10);
	//add water plane
	int map = 10;
	int sizeofmesh = 7;
	float scale = 0.5;
	for (int i = 0; i < map; i++)
	{
		for (int j = 0; j < map; j++)
		{
			int id = LoadMesh("./data/planegrid.obj", 2, watershader);
			selected_data_index = id;
			//ShapeTransformation(scaleAll, scale, 0);
			ShapeTransformation(xTranslate, (i - map / 2) * sizeofmesh, 0);
			ShapeTransformation(yTranslate, -7, 0);
			ShapeTransformation(zTranslate, -(j - map / 2) * sizeofmesh - 20, 0);
		}
	}

	

}


void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{


	//Animate_obj(cubeID,bezierAnimations[0], mytime);
	Shader* s = shaders[shaderIndx];
	int r = ((shapeIndx + 1) & 0x000000FF) >> 0;
	int g = ((shapeIndx + 1) & 0x0000FF00) >> 8;
	int b = ((shapeIndx + 1) & 0x00FF0000) >> 16;
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
		s->SetUniform4f("lightColor",  r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else if (shaderIndx == watershader)// water shader
	{
		s->SetUniform4f("lightColor", 1, 1, 1, 0.0f);
		s->SetUniform1f("time", mytime * 3); 
		
		//float scale = 2;
		s->SetUniform4f("tranlasion", data_list[shapeIndx]->GetPos().x(), data_list[shapeIndx]->GetPos().y(),  data_list[shapeIndx]->GetPos().z(), 0);
	}
	else if (shaderIndx == transparentshader)// transparent shader
	{
		s->SetUniform1f("alpha", 0.5);
	}
	else
		s->SetUniform4f("lightColor",  4 / 100.0f, 60 / 100.0f, 55* 99 / 100.0f, 0.5f);
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

	//int temp = selected_data_index;
	//mytime += tick;
	//for (int i = 0; i < data_list.size(); i++)
	//	if (data_list[i]->animtoinindex >= 0)
	//		Animate_obj(i, data_list[i]->animtoinindex, mytime);

	//translateControl(yTranslate, -0.01, CP2, false);
	//selected_data_index = temp;

    if(isActive)
	{

		int temp = selected_data_index;
		selected_data_index = -1;
		mytime += tick;
		for (int i = 0; i < data_list.size(); i++)
			if (data_list[i]->animtoinindex >= 0)
				Animate_obj(i, data_list[i]->animtoinindex, mytime);

		
		//std::cout << "animate isactive" << std::endl;
		ticksCounter += 1;
		
		//if(selected_data_index > 0 )
			//data()->MyRotate(Eigen::Vector3d(0, 1, 0), 0.01);
		selected_data_index = temp;
	}

	//std::cout << "animate out of if" << std::endl;

	//if (ticksCounter)
}


void Project::changeTime(float time)
{
	mytime = time;
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


void Project::openDialogLoadSceneInfo() {
	std::string fname = igl::file_dialog_open("", "Scene file (*.scn)\0*.scn\0All files (*.*)\0*.*");

	if (fname.length() == 0)
		return;

	this->loadSceneFromFile(fname.c_str());
	//load_scene(fname);
}

void Project::openDialogSaveSceneInfo() {
	char buf[32];
	sprintf(buf, "scene%d.scn", numOfSaves + 1);
	std::string fname = igl::file_dialog_save(buf, "Scene file (*.scn)\0*.scn\0All files (*.*)\0*.*");

	if (fname.length() == 0)
		return;

	this->saveSceneToFile(fname.c_str());
	//save_scene(fname);
	numOfSaves += 1;
}


void Project::loadSceneFromFile(const std::string& sceneInfoFileName) {
	
}


void Project::saveSceneToFile(const std::string& sceneInfoFileName) {
	
}