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
	SetShapeShader(shapeID, 2);
	SetShapeMaterial(matID, 0);
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


Project::ControlPoint Project::AddControlPoints(shapes type,int matID,int shaderID,int point,int bezIndex)
{
	Bezier1D* bezier = bezierAnimations[bezIndex].bezier;
	selected_data_index = LoadMesh(Octahedron, matID, shaderID);
	ShapeTransformation(scaleAll, 0.1, 0);
	ShapeTransformation(xTranslate, bezier->GetControlPoint(0, point).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, bezier->GetControlPoint(0, point).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, bezier->GetControlPoint(0, point).GetPos()->z(), 0);
	ControlPoint output = ControlPoint();
	output.bezierID = bezIndex;
	output.ControlID = selected_data_index;
	output.control_num = point;
	return output;
}

void Project::translateControl(int type, float amt,ControlPoint CP,bool preserve)
{
	myMoveable* mybez = &bezierAnimations[CP.bezierID];
	selected_data_index = CP.ControlID;
	ShapeTransformation(type, amt, 0);
	Eigen::Vector4d temp = Eigen::Vector4d();
	Eigen::Vector3d pos = data_list[CP.ControlID]->GetPos();
	temp << pos.x(), pos.y(), pos.z(), 0;
	mybez->bezier->MoveControlPoint(0, CP.control_num, false, temp);
	//data_list[mybez->objectindex]->clear();
	//data_list[CP.bezierID].
	if (preserve)
	{

	}
}

void Project::Init()
{
	Eigen::Vector4d vec = Eigen::Vector4d();
	vec << 1.5, 3.0, 0.0, 0.0;



	Eigen::Matrix4f mat = Eigen::Matrix4f();
	mat << 1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16;

	 myBezier = Bezier1D(); // check not destoryed



	std::cout << (myBezier.GetControlPoint(0, 1).GetPos()->x()) << std::endl;

	unsigned int texIDs[3] = { 0 , 1, 2 };
	unsigned int slots[3] = { 0 , 1, 2 };

	AddShader("shaders/pickingShader");
	AddShader("shaders/cubemapShader");
	AddShader("shaders/basicShader");
	AddShader("shaders/basicShader");
	int textureID = 0;
	textureID = AddTexture("textures/plane.png", 2);
	textureID = AddTexture("textures/cubemaps/Daylight Box_", 3);
	textureID = AddTexture("textures/grass.bmp", 2);
	//AddTexture("../res/textures/Cat_bump.jpg", 2);

	AddMaterial(texIDs, slots, 1);
	AddMaterial(texIDs + 1, slots + 1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);
	int shapeID = 0;
	//shapeID = AddShape(Cube, -2, TRIANGLES);
	LoadCubeMap(1);
	int cubeID = LoadMesh(Cube, 1, 2);
	//data_list[cubeID]->clear_edges();
	//data_list[cubeID]->clear_points();
	data_list[cubeID]->Hide();
	selected_data_index = cubeID;
	ShapeTransformation(yTranslate, -3, 0);

	std::cout << data_list[cubeID]->GetPos() << std::endl;

	myBezier.MoveControlPoint(0, 3, false, vec);
	bezierlineID = LoadMesh(myBezier.GetLine(), 1, 2);
	myMoveable a = myMoveable(0, 5, &myBezier, bezierlineID);
	bezierAnimations.push_back(a);


	int bezierindex = bezierAnimations.size() - 1;
	int  controlpoint = 0;
	ControlPoint CP0 =  AddControlPoints(Octahedron, 0, 2, controlpoint, bezierindex);
	  controlpoint = 1;
	  ControlPoint CP1 = AddControlPoints(Octahedron, 0, 2, controlpoint, bezierindex);
	  controlpoint = 2;
	  ControlPoint CP2 = AddControlPoints(Octahedron, 0, 2, controlpoint, bezierindex);
	  controlpoint = 3;
	 CP3 = AddControlPoints(Octahedron, 0, 2, controlpoint, bezierindex);
	 CP3 = CP2;

	/*selected_data_index = CP3.ControlID;
	translateControl(yTranslate, -2, CP3,false);
	editMesh(myBezier.GetLine(), bezierlineID);*/



}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{

	selected_data_index = CP3.ControlID;
	translateControl(yTranslate, -0.005, CP3,false);
	editMesh(bezierAnimations[CP3.bezierID].bezier->GetLine(), bezierlineID);

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
    if(isActive)
	{
		if(selected_data_index > 0 )
			data()->MyRotate(Eigen::Vector3d(0, 1, 0), 0.01);
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

