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

class Bezier1D2
{
	std::vector<Eigen::Matrix4f> segments;
public:
	Bezier1D2(void);
	int numberOfPoints;
};



Project::Project()
{
	ticksCounter = 0;
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
	SetShapeMaterial(shaderID, 0);
	//TODO change colour 
	for (int i = 0; i < mesh.positions.size() - 1; i++)
	{
		data()->add_edges(mesh.positions[i].transpose(), mesh.positions[i + 1].transpose(), Eigen::RowVector3d(0, 0, 0));
	}

	return shapeID;
}

void Project::Init()
{
	Eigen::Vector4f vec = Eigen::Vector4f();
	vec << 0.2, 1.3, 3.2,0.1;


	Eigen::Matrix4f mat = Eigen::Matrix4f();
	mat <<	1, 2, 3, 4,
			5, 6, 7, 8, 
			9, 10, 11, 12, 
			13, 14, 15, 16;

	Bezier1D myBezier =  Bezier1D();

	std::cout<< (myBezier.GetControlPoint(0, 1).GetPos()->x())<<std::endl ;

	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };
	
	AddShader("shaders/pickingShader");
	AddShader("shaders/cubemapShader");
	AddShader("shaders/basicShader");
	AddShader("shaders/basicShader");
	int textureID = 0;
	textureID = AddTexture("textures/plane.png",2);
	textureID = AddTexture("textures/cubemaps/Daylight Box_", 3);
	textureID = AddTexture("textures/grass.bmp", 2);
	//AddTexture("../res/textures/Cat_bump.jpg", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);
	int shapeID = 0;
	//shapeID = AddShape(Cube, -2, TRIANGLES);
	LoadCubeMap(1);
	int cubeID =LoadMesh(Cube, 1, 2);
	int bezierlineID = LoadMesh(myBezier.GetLine(), 1, 2);
	selected_data_index = cubeID;
	ShapeTransformation(yTranslate, -3, 0);

	selected_data_index = LoadMesh(Octahedron, 0, 2);
	ShapeTransformation(scaleAll, 0.1, 0);
	int point = 0;
	ShapeTransformation(xTranslate, myBezier.GetControlPoint(0, point).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, myBezier.GetControlPoint(0, point).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, myBezier.GetControlPoint(0, point).GetPos()->z(), 0);

	selected_data_index = LoadMesh(Octahedron, 0, 2);
	ShapeTransformation(scaleAll, 0.1, 0);
	 point = 1;
	ShapeTransformation(xTranslate, myBezier.GetControlPoint(0, point).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, myBezier.GetControlPoint(0, point).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, myBezier.GetControlPoint(0, point).GetPos()->z(), 0);

	selected_data_index = LoadMesh(Octahedron, 0, 2);
	ShapeTransformation(scaleAll, 0.1, 0);
	 point = 2;
	ShapeTransformation(xTranslate, myBezier.GetControlPoint(0, point).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, myBezier.GetControlPoint(0, point).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, myBezier.GetControlPoint(0, point).GetPos()->z(), 0);

	selected_data_index = LoadMesh(Octahedron, 0, 2);
	ShapeTransformation(scaleAll, 0.1, 0);
	 point = 3;
	ShapeTransformation(xTranslate, myBezier.GetControlPoint(0, point).GetPos()->x(), 0);
	ShapeTransformation(yTranslate, myBezier.GetControlPoint(0, point).GetPos()->y(), 0);
	ShapeTransformation(zTranslate, myBezier.GetControlPoint(0, point).GetPos()->z(), 0);
	printline("pos");
	std::cout << *myBezier.GetControlPoint(0, point).GetPos() << std::endl;


}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
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
		//std::cout << "animate isactive" << std::endl;
		ticksCounter += 1;
		
		if(selected_data_index > 0 )
			data()->MyRotate(Eigen::Vector3d(0, 1, 0), 0.01);
	}

	//std::cout << "animate out of if" << std::endl;

	//if (ticksCounter)
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

