
# Computer Graphics Project

In this Project, we try to implement a few basic Graphics concepts creating a 3D space
that allows editing and adding of mesh and animation 


## Features

- Moving objects using the Bezier curve
- Multiple cameras - green for the animation camera, red for the editor (while in green camera can't see editing obj such as bezier) 
- Picking objecting using color picking (Multipicing is bugged and is turned off)
- object texture change including the background cube-map
- a highlight of the picked object
- Menu which including
    - load object from the file system
    - add objects to layers and hide layers
    - split screen (not implemented)
    - adding cameras
    - TimeSlider 
    - play the button for animation
    - editing mesh material
    - zoom in on the picked object
-  Transparent objects
Extras
- we implemented  Gerstner waves shader (required the creating a .obj file)
## Run Locally

Clone the project

```bash
  git clone https://github.com/ntg7creation/CG3DBasicEngine/tree/finalproject
```

open the project using Visual studio with CmakeExtention and build the project

note! you should copy and paste the "texture" "data" and "shader" folder from the tutorial into out\build\debug\
or the project won't find the textures and shaders needed
## Inputs
Camera movment

Camera movement

- WASD move Camera  (with relation to the facing direction)
- arrow_keys rotate Camera in x and y axis
- spacebar start and stop animtion
- F key will switch camera - note that it only changes pos of the camera and not rotation due to a problome with the engine 
## Authors
- [@natai](https://github.com/ntg7creation)
- [@anton](https://github.com/tohanov)
- [@Jorden](https://github.com/yarden4815)

## Lessons Learned

What did you learn while building this project?
 What challenges did you face and how did you overcome them?


## Acknowledgements
this project was built on top of the course engine
 - [CG3DBasicEngine](https://github.com/tamirgrBGU/CG3DBasicEngine)
 - [complete ocean simulator](https://www.youtube.com/watch?v=kGEqaX4Y4bQ)
 - [gerstner waves totorial](https://catlikecoding.com/unity/tutorials/flow/waves/)
