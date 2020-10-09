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
    quick_shapes_.DrawCube(ground_combo, view_matrix_, proj_matrix_, Color(0.8, 0.8, 0.8));

    float head_r = 0.8;
    float head_h = 4;
    float torso_r = 1;
    float torso_h = head_h - head_r - torso_r * .75;
    float feet_r = 1.2;
    float feet_h = torso_h - torso_r - feet_r*.75;

    Matrix4 S_head = Matrix4::Scale(Vector3(head_r, head_r, head_r));
    Matrix4 T_head = Matrix4::Translation(Vector3(0, head_h, 0));
    Matrix4 head_combo = T_head * S_head;
    quick_shapes_.DrawSphere(head_combo, view_matrix_, proj_matrix_, Color(1, 1, 1));

    Matrix4 S_torso = Matrix4::Scale(Vector3(torso_r, torso_r, torso_r));
    Matrix4 T_torso = Matrix4::Translation(Vector3(0, torso_h, 0));
    Matrix4 torso_combo = T_torso * S_torso;
    quick_shapes_.DrawSphere(torso_combo, view_matrix_, proj_matrix_, Color(1, 1, 1));

    Matrix4 S_feet = Matrix4::Scale(Vector3(feet_r, feet_r, feet_r));
    Matrix4 T_feet = Matrix4::Translation(Vector3(0, feet_h, 0));
    Matrix4 feet_combo = T_feet * S_feet;
    quick_shapes_.DrawSphere(feet_combo, view_matrix_, proj_matrix_, Color(1, 1, 1));

    Matrix4 nose_scale = Matrix4::Scale(Vector3(0.5, 0.5, 0.5));
    Matrix4 nose_rotation = Matrix4::RotationX(GfxMath::ToRadians(90.0));
    Matrix4 nose_trans = Matrix4::Translation(Vector3(0, head_h + head_r, head_r));
    Matrix4 nose_combo = nose_scale * nose_trans * nose_rotation;
    quick_shapes_.DrawCone(nose_combo, view_matrix_, proj_matrix_, Color(.6,.3,0));
}





