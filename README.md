# OpenRocket

## Basic idea

-   the player will control a rocket using the keyboard
-   there will be asteroids, that must be avoided by the player
-   the asteroids will move along one axis
-   (extra) the rocket will have an attack and can destroy the asteroids

## Libraries

To run the application, the following libraries must be installed:

-   [FreeGLUT](https://www.transmissionzero.co.uk/software/freeglut-devel/)
-   [GLEW](http://glew.sourceforge.net/)
-   [glm](https://glm.g-truc.net/0.9.9/)
-   [GLFW](https://www.glfw.org/download)

## Installation steps for Windows and Visual Studio

Locate the installation directory for Visual Studio. The default path should look something like this:

`C:\Program Files (x86)\Microsoft Visual Studio\`

After that, navigate to your version folder.

`...\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\<your_version>`

In this folder, you should have `bin`, `include` and `lib` folders.

After downloading the library files:

-   for **FreeGLUT, GLEW and GLFW**: copy all the files to the coresponding directories. For FreeGLUT and GLEW, keep the `GL` folder when copying to `include`

-   for **glm**, extract the `zip` in the project folder. Make sure to keep the parent `glm` folder, in which you can find multiple other files and folders.

In Visual Studio, the libraries must be linked to the project.

Go to `Project -> Properties -> Configuration Properties -> Linker -> Input -> Additional Dependencies` and add

-   `freeglut.lib`
-   `glew32.lib`
-   `opengl32.lib`
-   `glfw3.lib`

Done!
