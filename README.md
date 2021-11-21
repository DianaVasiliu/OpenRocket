# OpenRocket

## Basic idea

A rocket on a mission in space must escape a wave of dangerous asteroids. To finish the mission and get home safe, the rocket can either avoid asteroids or destroy them with its weapon. If the rocket collides with an asteroid, it loses one life, but if it remains without lives, it explodes and the game ends.

## Gameplay

<p align="center">
  <img src="demo.gif" alt="Demo" width="80%"/>
</p>

## Libraries

To run the application, the following libraries must be installed:

-   [FreeGLUT](https://www.transmissionzero.co.uk/software/freeglut-devel/)
-   [GLEW](http://glew.sourceforge.net/)
-   [glm](https://glm.g-truc.net/0.9.9/)
-   [GLFW](https://www.glfw.org/download)
-   [SOIL](https://github.com/DianaVasiliu/OpenRocket/tree/master/OpenRocket/OpenRocket/SOIL)

## Installation steps for Windows and Visual Studio

Locate the installation directory for Visual Studio. The default path should look something like this:

`C:\Program Files (x86)\Microsoft Visual Studio\`

After that, navigate to your version folder.

`...\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\<your_version>`

In this folder, you should have `bin`, `include` and `lib` folders.

After downloading the library files:

-   for **FreeGLUT, GLEW and GLFW**: copy all the files to the coresponding directories. For FreeGLUT and GLEW, keep the `GL` folder when copying to `include`

-   for **glm**: extract the `zip` in the project folder. Make sure to keep the parent `glm` folder, in which you can find multiple other files and folders.

-   for **SOIL**: move `SOIL.lib` to the coresponding directory

In Visual Studio, the libraries must be linked to the project.

Go to `Project -> Properties -> Configuration Properties -> Linker -> Input -> Additional Dependencies` and add

-   `freeglut.lib`
-   `glew32.lib`
-   `opengl32.lib`
-   `glfw3.lib`
-   `<complete_path_to_VS_include_folder>\SOIL.lib`

Done!
