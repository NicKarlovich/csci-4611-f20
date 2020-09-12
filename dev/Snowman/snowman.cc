/** CSci-4611 In-Class Example */

#include "snowman.h"

#include <iostream>
#include <sstream>



Snowman::Snowman() : GraphicsApp(1024,768, "Do You Want to Build a Snowman?") {
}


Snowman::~Snowman() {
}


void Snowman::UpdateSimulation(double dt)  {
}


void Snowman::InitOpenGL() {
    // Set up the camera in a good position to see the entire scene
    proj_matrix_ = Matrix4::Perspective(60, aspect_ratio(), 0.01, 100);
    view_matrix_ = Matrix4::LookAt(Point3(0,2,10), Point3(0,0,0), Vector3(0,1,0));
    glClearColor(0.2, 0.6, 1.0, 1);
}


void Snowman::DrawUsingOpenGL() {
    Matrix4 identity;
    
    // draws a set of axes at the world origin, since we are passing the identity
    // matrix for the "model" matrix.
    quick_shapes_.DrawAxes(identity, view_matrix_, proj_matrix_);
    

    Matrix4 S_ground = Matrix4::Scale(Vector3(40, 0.2, 40));
    Matrix4 T_ground = Matrix4::Translation(Vector3(0, -0.2, 0));
    Matrix4 ground_combo = T_ground * S_ground;
    quick_shapes_.DrawCube(ground_combo, view_matrix_, proj_matrix_, Color(0.8,0.8,0.8));

    
    Matrix4 S_head = Matrix4::Scale(Vector3(0.8, 0.8, 0.8));
    Matrix4 T_head = Matrix4::Translation(Vector3(0, 4, 0));
    Matrix4 head_combo = T_head * S_head;
    quick_shapes_.DrawSphere(head_combo, view_matrix_, proj_matrix_, Color(1,1,1));
    
}





