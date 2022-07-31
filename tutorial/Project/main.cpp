#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "InputManager.h"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const int MENU_WIDTH = 350;
	const int SCENE_WIDTH = DISPLAY_WIDTH - MENU_WIDTH;

	const float CAMERA_ANGLE = 45.0f;
	const float NEAR = 1.0f;
	const float FAR = 120.0f;
	//const int infoIndx = 2;

	std::list<int> x, y;
	x.push_back(MENU_WIDTH);
	x.push_back(DISPLAY_WIDTH);
	y.push_back(DISPLAY_HEIGHT);

    Display disp = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OPENGL");
    igl::opengl::glfw::imgui::ImGuiMenu* menu = new igl::opengl::glfw::imgui::ImGuiMenu();
    Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)SCENE_WIDTH/(float)DISPLAY_HEIGHT, NEAR, FAR);

	Project *scn = new Project();  //initializing scene
	scn->rndr = rndr;

    Init(disp,menu); //adding callback functions
	scn->Init();    //adding shaders, textures, shapes to scene
    rndr->Init(scn,x,y,1,menu); // adding scene and viewports to the renderer
    disp.SetRenderer(rndr);


	// viewport for highlight
	rndr->AddViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	rndr->CopyDraw(1, rndr->viewport, 1);
	rndr->ClearDrawFlag(2, rndr->toClear | rndr->stencilTest);
	rndr->SetDrawFlag(2, rndr->blend | rndr->inAction2 | rndr->scissorTest);

	// viewport for picked shapes
	rndr->AddViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	rndr->CopyDraw(1, rndr->viewport, 2);
	rndr->ClearDrawFlag(3, rndr->toClear | rndr->stencilTest);
	rndr->SetDrawFlag(3, rndr->clearStencil);

	
	

	rndr->AddDraw(2, 0, 4, 0, rndr->stencil2 | rndr->stencilTest | rndr->depthTest | rndr->scaleAbit | rndr->onPicking);
	rndr->AddDraw(1, 0, 0, 0, rndr->depthTest | rndr->stencilTest);

	// viewport for transparancy
	rndr->AddViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	rndr->CopyDraw(1, rndr->viewport, 3);
	rndr->SetDrawFlag(6, rndr->blend);
	rndr->ClearDrawFlag(6, rndr->toClear);



    disp.launch_rendering(rndr);

	delete scn;
	delete rndr;
	delete menu;
	
	return 0;
}

