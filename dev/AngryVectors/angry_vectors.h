/** CSci-4611 In-Class Example */

#ifndef ANGRY_VECTORS_H_
#define ANGRY_VECTORS_H_

#include <mingfx.h>
using namespace mingfx;

#include <string>
#include <vector>

class AngryVectors : public GraphicsApp {
public:
  
    // Creates the App
    AngryVectors();
    
    // Cleans up when the App shuts down
    virtual ~AngryVectors();
    
    // Note a Run() function is inherited from GraphicsApp, that's what
    // actually starts up the App.
    
    // This is a callback, a function that gets called when the user presses
    // the Pause button in the GUI.
    void OnPauseBtnPressed();
    
    // This calculates the bird position given a time value.
    Point3 CalcBirdPos(double time);

    // This calculates the bird velocity given a time value, this is the routine
    // we want to check to make sure it is working properly
    Vector3 CalcBirdVel(double time);
    
    // This gets called once each frame.  Note that dt (a.k.a., "delta time") is
    // the amount of time (in seconds) that has passed since the last frame.
    void UpdateSimulation(double dt);
    
    // This is where we initialize the on-screen GUI, which is implemented using
    // an open source library called NanoGUI.  It gets called once
    // when the program starts up.
    void InitNanoGUI();
    
    // This is where we initialize any OpenGL data, like textures or meshes that
    // need to be loaded from files and setup in OpenGL.  It gets called once
    // when the program starts up.
    void InitOpenGL();
    
    // This gets called once each frame, and this is where you draw the latest
    // version of your 3D graphics scene.
    void DrawUsingOpenGL();
    
    
private:    
    
    // Time of flight for our "bird"
    float time_;
    
    // Sets up the computer graphics camera
    Matrix4 view_matrix_;
    Matrix4 proj_matrix_;
    
    // A helper class for drawing some simple shapes (cubes, spheres, 3D arrows)
    QuickShapes quick_shapes_;
    
    // Gui elements
    nanogui::Button  *pauseBtn_;
    bool playing_;
};

#endif