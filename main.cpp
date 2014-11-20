#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

#ifdef _WIN32
#include <windows.h>
#include "glWindow.h"
#else
#include "glxwindow.h"
#endif

#include "globject.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{
#else
int main(int argc, char** argv)
{
#endif
    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;
    const int windowFullscreen = false;

#ifdef _WIN32
    //This is our window
    GLWindow programWindow(hInstance);
#else
    SimpleGLXWindow programWindow;
#endif

    //The example OpenGL code
    GLObject globject;

    //Attach our example to our window
    programWindow.attachGLObject(&globject);

    //Attempt to create the window
    if (!programWindow.create(windowWidth, windowHeight, windowBPP, windowFullscreen))
    {
        //If it fails
#ifdef _WIN32
        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
#endif
        programWindow.destroy(); //Reset the display and exit
        return 1;
    }

    if (!globject.init()) //Initialize our example
    {
#ifdef _WIN32
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
#endif
        programWindow.destroy(); //Reset the display and exit
        return 1;
    }

    //This is the mainloop, we render frames until isRunning returns false
    while(programWindow.isRunning())
    {
        programWindow.processEvents(); //Process any window events

        //We get the time that passed since the last frame
        float elapsedTime = programWindow.getElapsedSeconds();

        globject.prepare(elapsedTime * 2); //Do any pre-rendering logic
        globject.render(); //Render the scene

        programWindow.swapBuffers();
    }

    globject.shutdown(); //Free any resources
    programWindow.destroy(); //Destroy the program window

    return 0; //Return success
}
