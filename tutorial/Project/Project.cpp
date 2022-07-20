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
}




//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

void Project::Init()
{
	Eigen::Vector3f vec = Eigen::Vector3f();
	vec << 0.2, 1.3, 3.2;
	std::cout << vec << std::endl;


	Eigen::Matrix4f mat = Eigen::Matrix4f();
	mat <<	1, 2, 3, 4,
			5, 6, 7, 8, 
			9, 10, 11, 12, 
			13, 14, 15, 16;

	Bezier1D myBezier =  Bezier1D();
	int lineres = 50;
	IndexedModel line = myBezier.GetLine(lineres);
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
	shapeID = AddShape(Cube, -2, TRIANGLES);
	shapeID = AddShape(Cube, -1, TRIANGLES);
	
	//shapeID = AddShape(zCylinder, -1, TRIANGLES);
	//shapeID = AddShape(zCylinder, -1, TRIANGLES);
	//shapeID = AddShape(zCylinder, -1, TRIANGLES);
	//shapeID = AddShape(Axis, -1, LINES);
	   
	//AddShapeFromFile("./data/cube.obj",-1, LINES);

	Bezier1D bez = Bezier1D::Bezier1D();
	//AddShapeFromFile("../res/objs/Cat_v1.obj", -1, TRIANGLES);
	
	SetShapeShader(1, 2);
	//SetShapeShader(2, 2);
	//SetShapeShader(3, 2);
	//SetShapeShader(4, 2);


	SetShapeMaterial(0, 1);
	SetShapeMaterial(1, 1);
	//SetShapeMaterial(1, 0);
	//SetShapeMaterial(2, 0);	
	//SetShapeMaterial(3, 0);	
	//SetShapeMaterial(4, 0);

	selected_data_index = 0;
	float cylinderLen = 1.6f;
	float s = 60;
	ShapeTransformation(scaleAll, s,0);
	SetShapeStatic(0);
	//selected_data_index = 1;
	//data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	ShapeTransformation(zTranslate, cylinderLen / 2.0, 1);
	//ShapeTransformation(yTranslate, 2.0, 1);
	//
	//selected_data_index = 2;
	//ShapeTransformation(zTranslate, cylinderLen , 1);
	//data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	//ShapeTransformation(xTranslate, -2.0, 1);

	//selected_data_index = 3;
	//ShapeTransformation(zTranslate, cylinderLen, 1);
	//data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	//ShapeTransformation(yTranslate, -2.0, 1);
	
	//data_list[1]->set_points(Eigen::Vector3d(-3, -3, -3).transpose(), Eigen::RowVector3d(0, 0, 1));
	//data_list[1]->set_edges(Eigen::Vector3d(-3, -3, -3).transpose(), Eigen::Vector3d(10, 3, 3).transpose(), Eigen::RowVector3d(1, 1, 0));
	for (int i = 0; i < lineres-1; i++)
	{
		//Eigen::RowVector3d temp; temp << line.positions[i].x(), line.positions[i].y(), line.positions[i].z();
		//data_list[1]->add_points(temp, Eigen::RowVector3d(1, 0, 0));
		
		//data_list[1]->add_edges(line.positions[i], line.positions[i + 1], Eigen::RowVector3d(1, 0, 0));
		
		int shapeindex = AddShape(Octahedron, -1, TRIANGLES);
		SetShapeShader(shapeindex, 2);
		SetShapeMaterial(shapeindex, 0);
		selected_data_index = shapeindex;

		

		ShapeTransformation(xTranslate, line.positions[i].x(), 0);
		ShapeTransformation(yTranslate, line.positions[i].y(), 0);
		ShapeTransformation(zTranslate, line.positions[i].z(), 0);
		ShapeTransformation(scaleAll, 0.1, 0);
		

	}





	

	//SetShapeViewport(6, 1);
//	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;


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

