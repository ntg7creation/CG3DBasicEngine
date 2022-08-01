#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "imgui/imgui.h"


	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		
		if (action == GLFW_PRESS)
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			Project* scn = (Project*)rndr->GetScene();
			double x2, y2;
			
			glfwGetCursorPos(window, &x2, &y2);
			rndr->UpdatePress(x2, y2);
			if (rndr->Picking((int)x2, (int)y2))
			{
				rndr->UpdatePosition(x2, y2);
				if(button == GLFW_MOUSE_BUTTON_LEFT)
					rndr->Pressed();
			}
			else
			{
				rndr->UnPick(2);
				
			}
		
		}
		else
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			rndr->UnPick(2);
		}

	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		
		if (rndr->IsPicked())
		{
			rndr->UpdateZpos((int)yoffset);
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
		}
		else
		{
			rndr->MoveCamera(0, rndr->zTranslate, (float)yoffset);
		}
		
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);

		if (rndr->CheckViewport(xpos,ypos, 0))
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{

				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany())
					rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);

		}
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

        rndr->resize(window,width,height);
		
	}
	

	static void printMat(const Eigen::Matrix3d& mat)
	{
		std::cout << " matrix:" << std::endl;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				std::cout << mat(j, i) << " ";
			std::cout << std::endl;
		}
	}

	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		bool temp;
		int camera_mesh_indx;
		Eigen::Vector3d temppos ;
		//rndr->FreeShapes(2);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{

			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;

			case GLFW_KEY_SPACE:
				if (scn->IsActive())
					scn->Deactivate();
				else
					scn->Activate();
				break;

			case GLFW_KEY_UP:
				rndr->MoveCamera(0, rndr->xRotate, 0.05f);

				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				rndr->MoveCamera(0, rndr->xRotate, -0.05f);
				break;
			case GLFW_KEY_LEFT:
				rndr->MoveCamera(0, rndr->yRotate, 0.05f);
				break;
			case GLFW_KEY_RIGHT:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				rndr->MoveCamera(0, rndr->yRotate, -0.05f);
				break;
			case GLFW_KEY_W:
				rndr->ZoomCamera(0, rndr->yTranslate, 0.25f);
				scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;
			case GLFW_KEY_S:
				rndr->ZoomCamera(0, rndr->yTranslate, -0.25f);
				scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;
			case GLFW_KEY_A:
				rndr->ZoomCamera(0, rndr->xTranslate, -0.25f);
				scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;

			case GLFW_KEY_D:
				rndr->ZoomCamera(0, rndr->xTranslate, 0.25f);
				scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;

			case GLFW_KEY_Q:
				rndr->ZoomCamera(0, scn->zTranslate, 0.5f);
				scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;
			case GLFW_KEY_E:
				rndr->ZoomCamera(0, scn->zTranslate, -0.5f);
				scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;

			case GLFW_KEY_Z:
				//TODO change cube ID to pickshape
				rndr->ZoomCamera(0, scn->data_list[scn->cubeID]->GetPos());
				//scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;
			case GLFW_KEY_X:
				//rndr->HardZoomCamera(0, scn->data_list[scn->cubeID]->GetPos(), scn->data_list[scn->cubeID]->GetRotation());
				//scn->moveCamera(rndr->cameras[0]->GetPos2());
				break;
			case GLFW_KEY_F:

				scn->current_Camera++;
				if (scn->current_Camera >= scn->Cameras.size())
					scn->current_Camera = 0;
				camera_mesh_indx = scn->Cameras[scn->current_Camera];
				rndr->HardZoomCamera(0, scn->data_list[abs(camera_mesh_indx)]->GetPos(), scn->data_list[abs(camera_mesh_indx)]->GetRotation2());
				if (camera_mesh_indx < 0)
					scn->hide_editor();
				else
					scn->unhide_editor();
				break;

			case GLFW_KEY_C:
				scn->changeTime(0);
				temp = scn->isActive;
				scn->isActive = true;
				scn->Animate();
				scn->isActive = temp;
				break;
				//case GLFW_KEY_X:
				//	rndr->ZoomCamera(0, scn->zTranslate, 0.5f);
				//	break;

			case GLFW_KEY_1:
				std::cout << "picked 1\n";
				scn->selected_data_index = 1;
				break;
			case GLFW_KEY_2:
				std::cout << "picked 2\n";
				scn->selected_data_index = 2;
				break;
			case GLFW_KEY_3:
				std::cout << "picked 3\n";
				scn->selected_data_index = 3;
				break;
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
    menu->init(&display);
}
