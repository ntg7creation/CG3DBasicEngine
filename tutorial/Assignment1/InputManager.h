#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Assignment1.h"
#include "imgui/imgui.h"


using namespace std;


SelectedCoefficient selectedCoefficient = NONE;

struct ScreenCoordinates {
	float x;
	float y;
} startingMousePosition;


void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
	Assignment1* scn = (Assignment1*)rndr->GetScene();
	if (action == GLFW_PRESS)
	{
		double x2, y2;
			
		glfwGetCursorPos(window, &x2, &y2);
		rndr->UpdatePress(x2, y2);
		if (rndr->Picking((int)x2, (int)y2))
		{
			//cout << "[glfw_mouse_callback] if" << endl;

			rndr->UpdatePosition(x2, y2);
			if(button == GLFW_MOUSE_BUTTON_LEFT)
				rndr->Pressed();
		}
		else
		{
			startingMousePosition.x = x2;
			startingMousePosition.y = y2;

			cout << "[glfw_mouse_callback] started holding mouse at coords: " << startingMousePosition.x << "," << startingMousePosition.y << endl;

			rndr->UnPick(2);
		}
		
	}
	else
	{
		scn->lockScreenOffset();
		cout << "[glfw_mouse_callback] stopped holding mouse" << endl;

		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		rndr->UnPick(2);
	}
}

void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
	Assignment1* scn = (Assignment1*)rndr->GetScene();

	rndr->UpdatePosition((float)xpos, (float)ypos);

	if (rndr->CheckViewport(xpos, ypos, 0))
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			scn->setTempScreenOffset(xpos - startingMousePosition.x, -(ypos - startingMousePosition.y));
			cout << "[glfw_cursor_position_callback] moving while holding left mouse, offset is: " << (xpos - startingMousePosition.x) << "," << (ypos - startingMousePosition.y) << endl;

			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany()) {

			//cout << "[glfw_cursor_position_callback] ELIF2" << endl;
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}

	}
}
	
void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
	Assignment1* scn = (Assignment1*)rndr->GetScene();
		
	if (rndr->IsPicked())
	{
		rndr->UpdateZpos((int)yoffset);
		rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
	}
	else
	{
		scn->zoomScreen( -yoffset );
		//rndr->MoveCamera(0, rndr->zTranslate, (float)yoffset);
	}
		
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

    rndr->resize(window,width,height);
		
}
	
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
	Assignment1* scn = (Assignment1*)rndr->GetScene();
	//rndr->FreeShapes(2);

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_1:
			selectedCoefficient = A;
			cout << "selected coeff A" << endl;
			break;

		case GLFW_KEY_2:
			selectedCoefficient = B;
			cout << "selected coeff B" << endl;
			break;

		case GLFW_KEY_3:
			selectedCoefficient = C;
			cout << "selected coeff C" << endl;
			break;

		case GLFW_KEY_4:
			selectedCoefficient = D;
			cout << "selected coeff D" << endl;
			break;

		case GLFW_KEY_UP:
			if (selectedCoefficient != NONE) {
				scn->incrementCoefficient(selectedCoefficient);
			}

			//scn->iteration++;
			//rndr->MoveCamera(0, scn->xRotate, 0.05f);
			break;

		case GLFW_KEY_DOWN:
			if (selectedCoefficient != NONE) {
				scn->decrementCoefficient(selectedCoefficient);
			}

			//if (scn->iteration > 0)
			//	scn->iteration--;
			//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
			//cout<< "down: "<<endl;
			//rndr->MoveCamera(0, scn->xRotate, -0.05f);
			break;

		case GLFW_KEY_LEFT:
			scn->decreaseIterationNum();

			//rndr->MoveCamera(0, scn->yRotate, 0.05f);
			break;

		case GLFW_KEY_RIGHT:

			scn->increaseIterationNum();

			//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
			//cout<< "down: "<<endl;
			//rndr->MoveCamera(0, scn->yRotate, -0.05f);
			break;


		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
				
		//case GLFW_KEY_SPACE:
		//	if (scn->IsActive())
		//		scn->Deactivate();
		//	else
		//		scn->Activate();
		//	break;

		//case GLFW_KEY_U:
		//	//rndr->MoveCamera(0, scn->yTranslate, 0.25f);

		//	break;
		//case GLFW_KEY_D:
		//	//rndr->MoveCamera(0, scn->yTranslate, -0.25f);

		//	break;
		//case GLFW_KEY_L:
		//	rndr->MoveCamera(0, scn->xTranslate, -0.25f);
		//	break;
		//	
		//case GLFW_KEY_R:
		//	rndr->MoveCamera(0, scn->xTranslate, 0.25f);
		//	break;
		//	
		//case GLFW_KEY_B:
		//	rndr->MoveCamera(0, scn->zTranslate, 0.5f);
		//	break;
		//case GLFW_KEY_F:
		//	rndr->MoveCamera(0, scn->zTranslate, -0.5f);
		//	break;
		default:
			break;

		}
	}
}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    //menu->init(&display);
}
