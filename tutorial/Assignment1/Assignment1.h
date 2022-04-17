#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "GLFW/glfw3.h"

enum SelectedCoefficient {
	A = 0,
	B,
	C,
	D,
	NONE
};

struct ScreenModification {
	float xOffset;
	float yOffset;
	float scaling;

	float xTempOffset;
	float yTempOffset;
};


class Assignment1 : public igl::opengl::glfw::Viewer
{
	float time;
	Eigen::Vector3cf FindRootsOfReduceEquation(Eigen::Vector2cf reduceCoeffs);
	std::complex<float> Assignment1::NewtonCubicRoot(std::complex<float> num);
	int iterationNum;
	const float COEFF_INC;
	ScreenModification screenMod;
	int screenHeight;
	int screenWidth;
	void updateWindowTitle(void);
	void updateRoots(void);
	GLFWwindow *window;
	
public:
	void zoomScreen(double offset);
	void setTempScreenOffset(double xoffset, double yoffset);
	void lockScreenOffset(void);

	void increaseIterationNum(void);
	void decreaseIterationNum(void);

	void incrementCoefficient(SelectedCoefficient sc);
	void decrementCoefficient(SelectedCoefficient sc);

	Eigen::Vector4cf coeffs;
	float root1_x, root1_y, root2_x, root2_y, root3_x, root3_y;
	int iteration =0;
	Eigen::Vector3cf roots;
	float x, y;
	Assignment1(GLFWwindow *_window);
	//	Assignment1(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);

	Eigen::Vector3cf FindCubicRoots();

	~Assignment1(void);
};

