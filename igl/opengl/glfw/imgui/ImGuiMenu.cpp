// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2018 Jérémie Dumas <jeremie.dumas@ens-lyon.org>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
////////////////////////////////////////////////////////////////////////////////
//#include "ImGuiMenu.h"
//#include "ImGuiHelpers.h"
#include <igl/project.h>
#include "ImGuiHelpers.h"

#include "ImGuiMenu.h"
#include "../imgui.h"
#include "igl/opengl/glfw/imgui/imgui_impl_glfw.h"
#include "igl/opengl/glfw/imgui/imgui_impl_opengl3.h"

//#include <imgui_fonts_droid_sans.h>
//#include <GLFW/glfw3.h>
#include <iostream>
#include "../renderer.h"
#include "../../../../tutorial/Project/Project.h"
////////////////////////////////////////////////////////////////////////////////

namespace igl
{
	namespace opengl
	{
		namespace glfw
		{
			namespace imgui
			{

				IGL_INLINE void ImGuiMenu::init(Display* disp)
				{
					// Setup ImGui binding
					if (disp->window)
					{
						IMGUI_CHECKVERSION();
						if (!context_)
						{
							// Single global context by default, but can be overridden by the user
							static ImGuiContext* __global_context = ImGui::CreateContext();
							context_ = __global_context;
						}
						const char* glsl_version = "#version 150";

						ImGui_ImplGlfw_InitForOpenGL(disp->window, true);
						ImGui_ImplOpenGL3_Init(glsl_version);
						ImGui::GetIO().IniFilename = nullptr;
						ImGui::StyleColorsDark();
						ImGuiStyle& style = ImGui::GetStyle();
						style.FrameRounding = 5.0f;
						reload_font();
					}

					//pickedLayerIndex = 0;
					animationDuration = 5;
				}

				IGL_INLINE void ImGuiMenu::reload_font(int font_size)
				{
					hidpi_scaling_ = hidpi_scaling();
					pixel_ratio_ = pixel_ratio();
					ImGuiIO& io = ImGui::GetIO();
					io.Fonts->Clear();
					// io.Fonts->AddFontFromMemoryCompressedTTF(droid_sans_compressed_data,
					//   droid_sans_compressed_size, font_size * hidpi_scaling_);
					io.FontGlobalScale = 1.0 / pixel_ratio_;
				}

				IGL_INLINE void ImGuiMenu::shutdown()
				{
					// Cleanup
					ImGui_ImplOpenGL3_Shutdown();
					ImGui_ImplGlfw_Shutdown();
					// User is responsible for destroying context if a custom context is given
					// ImGui::DestroyContext(*context_);
				}

				IGL_INLINE bool ImGuiMenu::pre_draw()
				{
					glfwPollEvents();

					// Check whether window dpi has changed
					float scaling = hidpi_scaling();
					if (std::abs(scaling - hidpi_scaling_) > 1e-5)
					{
						reload_font();
						ImGui_ImplOpenGL3_DestroyDeviceObjects();
					}

					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					return false;
				}

				IGL_INLINE bool ImGuiMenu::post_draw()
				{
					//draw_menu(viewer,core);
					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
					return false;
				}

				IGL_INLINE void ImGuiMenu::post_resize(int width, int height)
				{
					if (context_)
					{
						ImGui::GetIO().DisplaySize.x = float(width);
						ImGui::GetIO().DisplaySize.y = float(height);
					}
				}

				// Mouse IO
				IGL_INLINE bool ImGuiMenu::mouse_down(GLFWwindow* window, int button, int modifier)
				{
					ImGui_ImplGlfw_MouseButtonCallback(window, button, GLFW_PRESS, modifier);
					std::cout << "mouse_down of imgui";
					return ImGui::GetIO().WantCaptureMouse;
				}

				IGL_INLINE bool ImGuiMenu::mouse_up(GLFWwindow* window, int button, int modifier)
				{
					//return ImGui::GetIO().WantCaptureMouse;
					// !! Should not steal mouse up
					return false;
				}

				IGL_INLINE bool ImGuiMenu::mouse_move(GLFWwindow* window, int mouse_x, int mouse_y)
				{
					return ImGui::GetIO().WantCaptureMouse;
				}

				IGL_INLINE bool ImGuiMenu::mouse_scroll(GLFWwindow* window, float delta_y)
				{
					ImGui_ImplGlfw_ScrollCallback(window, 0.f, delta_y);
					return ImGui::GetIO().WantCaptureMouse;
				}

				// Keyboard IO
				IGL_INLINE bool ImGuiMenu::key_pressed(GLFWwindow* window, unsigned int key, int modifiers)
				{
					ImGui_ImplGlfw_CharCallback(nullptr, key);
					return ImGui::GetIO().WantCaptureKeyboard;
				}

				IGL_INLINE bool ImGuiMenu::key_down(GLFWwindow* window, int key, int modifiers)
				{
					ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_PRESS, modifiers);
					return ImGui::GetIO().WantCaptureKeyboard;
				}

				IGL_INLINE bool ImGuiMenu::key_up(GLFWwindow* window, int key, int modifiers)
				{
					ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_RELEASE, modifiers);
					return ImGui::GetIO().WantCaptureKeyboard;
				}



				IGL_INLINE void ImGuiMenu::draw_viewer_menu(igl::opengl::glfw::Viewer* viewer, std::vector<igl::opengl::Camera*>& camera, Eigen::Vector4i& viewWindow, std::vector<DrawInfo*> drawInfos)
				{
					bool* p_open = NULL;
					static bool no_titlebar = false;
					static bool no_scrollbar = false;
					static bool no_menu = true;
					static bool no_move = true; //false;
					static bool no_resize = true; //false;
					static bool no_collapse = true; //false;
					static bool no_close = false;
					static bool no_nav = false;
					static bool no_background = false;
					static bool no_bring_to_front = false;

					ImGuiWindowFlags window_flags = 0;
					if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
					if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
					if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
					if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
					if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
					if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
					if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
					if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
					if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;


					//ImGui::ShowDemoWindow(); // TODO : remove debug

					scn = ((Project*)(viewer));

					ImGui::Begin(
						"Menu", p_open,
						window_flags
					);

					ImGui::SetWindowPos(ImVec2((float)0, (float)0), ImGuiCond_Always);
					ImGui::SetWindowSize(ImVec2((float)350, (float)800), ImGuiCond_Always);

					// Load an Object

					// Add a layer

					// Add a camera to the animation mode and when to switch cameras

					// time slider

					// play button to play the animation

					// add or change the material of a picked object
						// ??? use Viewer::AddMaterial and Viewer::BindMaterial ???

					// ??? zoom in to a chosen area ???

					// [not sure if that's how the transparency is supposed to be handled] slider for alpha of an object to make it more or less transparent

					// ??? choose a cubemap for the background ???

					// ??? open bezier creator to move the picked camera ???

					// swith to camera X

					ImGui::Indent();

					if (!((Project*)(viewer))->isActive) {
						if (ImGui::Button("Play animation", ImVec2(-1, 30))) {
							((Project*)(viewer))->ticksCounter = 0;
							((Project*)(viewer))->Activate();

						}
					}
					else {
						if (ImGui::Button("Stop animation", ImVec2(-1, 30))) {
							((Project*)(viewer))->Deactivate();
						}
					}

					//ImGui::Separator();
					//ImGui::Separator();
					//ImGui::NewLine();

					if (ImGui::Button("Load an Object", ImVec2(-1, 30))) {

						int savedIndx = viewer->selected_data_index;
						// viewer->selected_data_index = viewer->parents.size();
						// viewer->AddShape(viewer->xCylinder,-1,viewer->TRIANGLES);

						//LoadFrom
						viewer->open_dialog_load_mesh();
						if (viewer->data_list.size() > viewer->parents.size())
						{

							viewer->parents.push_back(-1);
							viewer->SetShapeViewport(viewer->selected_data_index, 0);
							viewer->SetShapeShader(viewer->selected_data_index, 2);
							viewer->SetShapeMaterial(viewer->selected_data_index, 0);
							//viewer->data_list.back()->set_visible(false, 1);
							//viewer->data_list.back()->set_visible(true, 2);
							viewer->data_list.back()->UnHide();
							viewer->data_list.back()->show_faces = 3;
							viewer->data()->mode = viewer->TRIANGLES;
							viewer->selected_data_index = savedIndx;
						}
					}

					ImGui::Unindent();

					ImGui::NewLine();
					ImGui::Separator();
					ImGui::Separator();
					ImGui::NewLine();

					if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_CollapsingHeader/*ImGuiTreeNodeFlags_DefaultOpen*/)) {

						ImGui::Indent();

						if (ImGui::Button("Load scene from file", ImVec2(-1, 30))) {
							((Project*)(viewer))->openDialogLoadSceneInfo();
						}

						if (ImGui::Button("Save current scene to file", ImVec2(-1, 30))) {
							((Project*)(viewer))->openDialogSaveSceneInfo();
						}

						ImGui::Unindent();
					}

					if (ImGui::CollapsingHeader("CubeMap", ImGuiTreeNodeFlags_CollapsingHeader/*ImGuiTreeNodeFlags_DefaultOpen*/)) {

						ImGui::Indent();

						static int selectedCubeMapMaterial = 0; //-1; // Here we store our selection data as an index.
						//const char* combo_label = "";  // Label to preview before opening the combo (technically it could be anything)
						if (ImGui::BeginCombo("##cubemap index", (selectedCubeMapMaterial == -1 ? "" : std::to_string(selectedCubeMapMaterial).c_str())))
						{
							for (int n = 0; n < ((Project*)(viewer))->numCubeMapTextures; n++)
							{
								const bool is_selected = (selectedCubeMapMaterial == n);

								char buf[32];
								sprintf(buf, "Cubemap %d", n);

								if (ImGui::Selectable(buf, is_selected)) {
									selectedCubeMapMaterial = n;
								}

								// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}

						if (ImGui::Button("Set CubeMap", ImVec2(-1, 0))) {
							((Project*)(viewer))->SetCubeMap(selectedCubeMapMaterial);
						}

						ImGui::Unindent();
					}
					/*ImGui::Text("CubeMap Index");
					ImGui::SameLine();
					ImGui::InputInt("", &materialIndex);
					ImGui::Text("out of");
					ImGui::SameLine();
					ImGui::Text(std::to_string(((Project*)(viewer))->numCubeMapTextures).c_str());*/


					if (ImGui::CollapsingHeader("Picked Object Config", ImGuiTreeNodeFlags_CollapsingHeader/*ImGuiTreeNodeFlags_DefaultOpen*/)) {

						ImGui::Indent();

						if (ImGui::Button("Add bezier to object", ImVec2(-1, 0))) {
							scn->addbezier(scn->lastPickedIndex);
						}

						static Eigen::Vector3d preZoomPos = scn->data_list[scn->current_Camera]->GetPos();
						static bool zoomedIn = false;

						if (zoomedIn) {
							if (ImGui::Button("Zoom out to default state", ImVec2(-1, 0))) {
								//std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;

								// ???
								scn->rndr->ZoomCamera(0, preZoomPos);
								zoomedIn = !zoomedIn;
							}
						}
						else {
							if (ImGui::Button("Zoom into picked object", ImVec2(-1, 0))) {
								preZoomPos = scn->data_list[scn->current_Camera]->GetPos();
								//std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
								scn->rndr->ZoomCamera(0, scn->data_list[scn->lastPickedIndex]->GetPos());
								zoomedIn = !zoomedIn;
							}
						}


						//scn->moveCamera(rndr->cameras[0]->GetPos2());
						if (ImGui::Button("Hard zoom into picked object", ImVec2(-1, 0))) {
							scn->rndr->HardZoomCamera(0, scn->data_list[scn->cubeID]->GetPos(), scn->data_list[scn->lastPickedIndex]->GetRotation());
						}

						//if (ImGui::SliderInt("Time slider", &timeSliderValue, 0, 100)) {

						ImGui::Text("Object on layer: ");
						ImGui::SameLine();

						if (((Project*)(viewer))->lastPickedIndex != -1) {

							int tempLayer = ((Project*)(viewer))->data_list[((Project*)(viewer))->lastPickedIndex]->layer;

							ImGui::Text(
								tempLayer != -1 ?
								std::to_string(tempLayer).c_str() :
								"N/A"
							);

						}

						static int newLayerIndex = 0;

						ImGui::NewLine();
						ImGui::InputInt("new layer index", &newLayerIndex);
						if (ImGui::Button("Set new layer", ImVec2(-1, 0))) {
							((Project*)(viewer))->changelayer(newLayerIndex, ((Project*)(viewer))->lastPickedIndex);
						}

						ImGui::NewLine();
						ImGui::InputInt("Animation Duration", &animationDuration);
						if (ImGui::SliderFloat("Animation delay slider", &timeSliderValue, 0.f, 100.f)) {
							std::cout << "Time slider changed to value " << timeSliderValue << " but not implemented" << std::endl;
						}

						ImGui::NewLine();

						//ImGui::InputInt("Material Index", &materialIndex);

						//if (ImGui::TreeNode("Selection State: Single Selection"))
						//{
							/*static int selectedObjMaterial = -1;
							for (int n = 0; n < ((Project*)(viewer))->numObjectsTextures; n++) {
								char buf[32];
								sprintf(buf, "Material %d", n);
								if (ImGui::Selectable(buf, selectedObjMaterial == n))
									selectedObjMaterial = n;
							}*/
							//ImGui::TreePop();
						//}

						//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
						static int selectedObjMaterial = 0; //-1; // Here we store our selection data as an index.
						//const char* combo_label = "";  // Label to preview before opening the combo (technically it could be anything)
						if (ImGui::BeginCombo("##material index", (selectedObjMaterial == -1 ? "" : std::to_string(selectedObjMaterial).c_str())))
						{
							for (int n = 0; n < ((Project*)(viewer))->numObjectsTextures; n++)
							{
								const bool is_selected = (selectedObjMaterial == n);

								char buf[32];
								sprintf(buf, "Material %d", n);

								if (ImGui::Selectable(buf, is_selected)) {
									selectedObjMaterial = n;
								}

								// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}
						if (ImGui::Button("Set Material", ImVec2(-1, 0))) {
							//std::cout << "Change Material of Picked Object clicked but not implemented" << std::endl;
							if (selectedObjMaterial != -1) {
								((Project*)(viewer))->SetMaterialOfPickedObjs(selectedObjMaterial);
							}
						}

						ImGui::NewLine();

						bool transparencyPlaceholder = false;
						ImGui::Checkbox("Transparent", &transparencyPlaceholder);

						if (ImGui::Button("Set transparency", ImVec2(-1, 0))) {
							std::cout << "Set transparency of Picked Object clicked but not implemented" << std::endl;
						}
						ImGui::SliderFloat("Transp. Value", &transparencySliderValue, 0.f, 100.f);

						ImGui::Unindent();
					}


					if (ImGui::CollapsingHeader("Cameras", ImGuiTreeNodeFlags_CollapsingHeader))
					{
						ImGui::Indent();

						static int camera_mesh_indx = -1;

						ImGui::Text("Number of cameras: "); ImGui::SameLine(); ImGui::Text(std::to_string((((Project*)(viewer)))->Cameras.size()).c_str());

						ImGui::Text("Current camera index: ");
						ImGui::SameLine();
						ImGui::Text(
							std::to_string(scn->current_Camera).c_str()
						);

						if (ImGui::Button("Move to next camera", ImVec2(-1, 0))) {

							scn->current_Camera++;
							if (scn->current_Camera >= scn->Cameras.size())
								scn->current_Camera = 0;
							camera_mesh_indx = scn->Cameras[scn->current_Camera];
							((Project*)(viewer))->rndr->HardZoomCamera(0, scn->data_list[abs(camera_mesh_indx)]->GetPos(), scn->data_list[abs(camera_mesh_indx)]->GetRotation2());
							if (camera_mesh_indx < 0)
								scn->hide_editor();
							else
								scn->unhide_editor();
						}

						ImGui::NewLine();

						//static int selectedCamera = 0; //-1; // Here we store our selection data as an index.
						static float pos[3] = { 0 };

						ImGui::InputFloat3("position of new camera", pos);
						if (ImGui::Button("Add an animation Camera", ImVec2(-1, 0))) {
							//std::cout << "Add camera clicked but not implemented" << std::endl;
							//((Project*)(viewer))->rndr->AddCamera(Eigen::Vector3d(0, 0, 3), CAMERA_ANGLE, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT / 2, NEAR, FAR);

							/*const int DISPLAY_WIDTH = 1200;
							const int DISPLAY_HEIGHT = 800;
							const int MENU_WIDTH = 350;
							const int SCENE_WIDTH = DISPLAY_WIDTH - MENU_WIDTH;

							const float CAMERA_ANGLE = 45.0f;
							const float NEAR = 1.0f;
							const float FAR = 120.0f;

							((Project*)(viewer))->rndr->AddCamera(
								Eigen::Vector3d(0, 0, 3),
								CAMERA_ANGLE,
								(float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT / 2,
								NEAR, FAR
							);

							selectedCamera = ((Project*)(viewer))->rndr->cameras.size() - 1;*/

							scn->addCamera(Eigen::Vector3f(pos));
						}

						//const char* combo_label = "";  // Label to preview before opening the combo (technically it could be anything)
						//if (ImGui::BeginCombo("##camera index", (selectedCamera == -1 ? "" : std::to_string(selectedCamera).c_str())))
						//{
						//	for (int n = 0; n < ((Project*)(viewer))->rndr->cameras.size(); n++)
						//	{
						//		const bool is_selected = (selectedCamera == n);

						//		char buf[32];
						//		sprintf(buf, "Camera %d", n);

						//		if (ImGui::Selectable(buf, is_selected)) {
						//			selectedCamera = n;
						//		}

						//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						//		if (is_selected)
						//			ImGui::SetItemDefaultFocus();
						//	}
						//	ImGui::EndCombo();
						//}
						//if (ImGui::Button("Switch to camera", ImVec2(-1, 0))) {
						//	std::cout << "Switch to previous camera clicked but not implemented" << std::endl;
						//}

						ImGui::Unindent();
					}


					if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_CollapsingHeader))
					{
						static int selectedLayer = 0;//-1; // Here we store our selection data as an index.

						ImGui::Indent();
						//ImGui::Text("Number of layers: <NOT IMPLEMENTED>");
						//if (ImGui::Button("Add a Layer", ImVec2(-1, 0))) {
						//	//std::cout << "Add layer clicked but not implemented" << std::endl;
						//	selectedLayer = ((Project*)(viewer))->numOfLayers;
						//	((Project*)(viewer))->addLayer();

						//}

						ImGui::InputInt("layer index", &selectedLayer);
						ImGui::NewLine();

						if (ImGui::Button("Hide Layer", ImVec2(-1, 0))) {
							// std::cout << "Add layer clicked but not implemented" << std::endl;
							(((Project*)(viewer)))->hidelayer(selectedLayer);
						}

						if (ImGui::Button("Unhide Layer", ImVec2(-1, 0))) {
							//std::cout << "Add layer clicked but not implemented" << std::endl;
							(((Project*)(viewer)))->unhidelayer(selectedLayer);
						}

						//ImGui::InputInt("Layer Number", &pickedLayerIndex);


						//const char* combo_label = "";  // Label to preview before opening the combo (technically it could be anything)
						//if (ImGui::BeginCombo("##layer index", (selectedLayer == -1 ? "" : std::to_string(selectedLayer).c_str())))
						//{
						//	for (int n = 0; n < ((Project*)(viewer))->numOfLayers; n++)
						//	{
						//		const bool is_selected = (selectedLayer == n);

						//		char buf[32];
						//		sprintf(buf, "Layer %d", n);

						//		if (ImGui::Selectable(buf, is_selected)) {
						//			selectedLayer = n;
						//		}

						//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						//		if (is_selected)
						//			ImGui::SetItemDefaultFocus();
						//	}
						//	ImGui::EndCombo();
						//}

						/*ImGui::InputInt2();
						ImGui::InputInt3();
						ImGui::InputInt4();*/

						/*char buf[256];
						ImGui::InputTextWithHint("", "", buf, 256);*/
						ImGui::Unindent();

					}


					ImGui::NewLine();

					ImGui::Separator();
					ImGui::Separator();

					ImGui::NewLine();

					ImGui::Indent();

					/*if (ImGui::Button("btn 1", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 2", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 3", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 4", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 5", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 6", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 7", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 8", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 9", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}
					if (ImGui::Button("btn 10", ImVec2(-1, 0))) {
						std::cout << "Choose Area to Zoom into clicked but not implemented" << std::endl;
					}*/

					ImGui::Unindent();

					ImGui::End();




					/// ********************************** PREVIOUS CODE ********************************

										//ImGui::Begin(
										//	"Menu", p_open,
										//	window_flags
										//);

										//ImGui::SetWindowPos(ImVec2((float)0, (float)0), ImGuiCond_Always);
										//ImGui::SetWindowSize(ImVec2((float)0, (float)0), ImGuiCond_Always);
										//ImGui::End();
										//no_move = true;
										//no_resize = true;

										//ImGui::Begin(
										//	"Menu", p_open,
										//	window_flags
										//);

										//// Mesh
										//if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
										//{
										//	float w = ImGui::GetContentRegionAvailWidth();
										//	float p = ImGui::GetStyle().FramePadding.x;
										//	if (ImGui::Button("Load##Mesh", ImVec2((w - p) / 2.f, 0)))
										//	{
										//		int savedIndx = viewer->selected_data_index;
										//		// viewer->selected_data_index = viewer->parents.size();
										//		// viewer->AddShape(viewer->xCylinder,-1,viewer->TRIANGLES);
										//		viewer->open_dialog_load_mesh();
										//		if (viewer->data_list.size() > viewer->parents.size())
										//		{

										//			viewer->parents.push_back(-1);
										//			viewer->SetShapeViewport(viewer->selected_data_index, 0);
										//			viewer->SetShapeShader(viewer->selected_data_index, 2);
										//			viewer->SetShapeMaterial(viewer->selected_data_index, 0);
										//			//viewer->data_list.back()->set_visible(false, 1);
										//			//viewer->data_list.back()->set_visible(true, 2);
										//			viewer->data_list.back()->UnHide();
										//			viewer->data_list.back()->show_faces = 3;
										//			viewer->data()->mode = viewer->TRIANGLES;
										//			viewer->selected_data_index = savedIndx;
										//		}
										//	}
										//	ImGui::SameLine(0, p);
										//	if (ImGui::Button("Save##Mesh", ImVec2((w - p) / 2.f, 0)))
										//	{
										//		viewer->open_dialog_save_mesh();
										//	}
										//}

										//// Viewing options
										//if (ImGui::CollapsingHeader("Viewing Options", ImGuiTreeNodeFlags_DefaultOpen))
										//{
										//	if (ImGui::Button("Center object", ImVec2(-1, 0)))
										//	{
										//		std::cout << "not implemented yet" << std::endl;
										//		//      core[1].align_camera_center(viewer->data().V, viewer->data().F); TODO: add function like this to camera
										//	}
										//	//if (ImGui::Button("Snap canonical view", ImVec2(-1, 0)))
										//	//{
										//	//  core[1].snap_to_canonical_quaternion();
										//	//}

										//	// Zoom
										//	ImGui::PushItemWidth(80 * menu_scaling());
										//	if (camera[0]->_ortho)
										//		ImGui::DragFloat("Zoom", &(camera[0]->_length), 0.05f, 0.1f, 20.0f);
										//	else
										//		ImGui::DragFloat("Fov", &(camera[0]->_fov), 0.05f, 30.0f, 90.0f);

										//	// Select rotation type
										//	static Eigen::Quaternionf trackball_angle = Eigen::Quaternionf::Identity();
										//	static bool orthographic = true;

										//	// Orthographic view
										//	ImGui::Checkbox("Orthographic view", &(camera[0]->_ortho));
										//	if (camera[0]->_ortho) {
										//		camera[0]->SetProjection(0, camera[0]->_relationWH);
										//	}
										//	else {
										//		camera[0]->SetProjection(camera[0]->_fov > 0 ? camera[0]->_fov : 45, camera[0]->_relationWH);
										//	}

										//	ImGui::PopItemWidth();
										//}

										//// Helper for setting viewport specific mesh options
										//auto make_checkbox = [&](const char* label, unsigned int& option)
										//{
										//	return ImGui::Checkbox(label,
										//		[&]() {
										//			return drawInfos[1]->is_set(option);
										//		},
										//		[&](bool value) {
										//			return drawInfos[1]->set(option, value);
										//		}
										//		);
										//};
										//ImGui::ColorEdit4("Background", drawInfos[1]->Clear_RGBA.data(),
										//	ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);

										//// Draw options
										//if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen))
										//{
										//	if (ImGui::Checkbox("Face-based", &(viewer->data()->face_based)))
										//	{
										//		viewer->data()->dirty = MeshGL::DIRTY_ALL;
										//	}
										//	//
										//	//    make_checkbox("Show texture", viewer->data().show_texture);
										//	//    if (ImGui::Checkbox("Invert normals", &(viewer->data().invert_normals)))
										//	//    {
										//	//      viewer->data().dirty |= igl::opengl::MeshGL::DIRTY_NORMAL;
										//	//    }
										//	make_checkbox("Show overlay", viewer->data()->show_overlay);
										//	make_checkbox("Show overlay depth", viewer->data()->show_overlay_depth);

										//	ImGui::ColorEdit4("Line color", viewer->data()->line_color.data(),
										//		ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
										//	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
										//	ImGui::DragFloat("Shininess", &(viewer->data()->shininess), 0.05f, 0.0f, 100.0f);
										//	ImGui::PopItemWidth();
										//}

										//// Overlays
										//if (ImGui::CollapsingHeader("Overlays", ImGuiTreeNodeFlags_DefaultOpen))
										//{
										//	make_checkbox("Wireframe", viewer->data()->show_lines);
										//	make_checkbox("Fill", viewer->data()->show_faces);

										//}
										//ImGui::End();
				}



				IGL_INLINE float ImGuiMenu::pixel_ratio()
				{
					// Computes pixel ratio for hidpi devices
					int buf_size[2];
					int win_size[2];
					GLFWwindow* window = glfwGetCurrentContext();
					glfwGetFramebufferSize(window, &buf_size[0], &buf_size[1]);
					glfwGetWindowSize(window, &win_size[0], &win_size[1]);
					return (float)buf_size[0] / (float)win_size[0];
				}

				IGL_INLINE float ImGuiMenu::hidpi_scaling()
				{
					// Computes scaling factor for hidpi devices
					float xscale, yscale;
					GLFWwindow* window = glfwGetCurrentContext();
					glfwGetWindowContentScale(window, &xscale, &yscale);
					return 0.5 * (xscale + yscale);
				}

			} // end namespace
		} // end namespace
	} // end namespace
} // end namespace
