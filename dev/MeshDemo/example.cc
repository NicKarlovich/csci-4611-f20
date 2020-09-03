/** CSci-4611 In-Class Example */

#include "Example.h"

#include <iostream>
#include <sstream>



Example::Example() : GraphicsApp(1024,768, "MinGfx Example") {
}


Example::~Example() {
}


void Example::UpdateSimulation(double dt)  {
}


void Example::InitOpenGL() {
    // Set up the camera in a good position to see the entire scene
    proj_matrix_ = Matrix4::Perspective(60, aspect_ratio(), 0.01, 100);
    view_matrix_ = Matrix4::LookAt(Point3(0,0,3), Point3(0,0,0), Vector3(0,1,0));
    glClearColor(1,1,1,1);
    
    
    // EXAMPLE 1:  Load the mesh from a file
    teapot_mesh_.LoadFromOBJ(Platform::FindMinGfxDataFile("teapot.obj"));

    
    
    
    // EXAMPLE 2:  Create the mesh by adding triangles one at a time
    int tri_id;
    // add a first triangle
    tri_id = triangles_mesh_.AddTriangle(Point3(0,0,0), Point3(1,0,0), Point3(1,1,0));
    // set attributes for the vertices
    triangles_mesh_.SetNormals(tri_id, Vector3(0,0,1), Vector3(0,0,1), Vector3(0,0,1));
    // add a second triangle and attributes
    tri_id = triangles_mesh_.AddTriangle(Point3(0,0,0), Point3(1,1,0), Point3(0,1,0));
    triangles_mesh_.SetNormals(tri_id, Vector3(0,0,1), Vector3(0,0,1), Vector3(0,0,1));
    // call this when done to save to the GPU
    triangles_mesh_.UpdateGPUMemory();

    
     
    
     
    // EXAMPLE 3:  Create the mesh by setting the vertex and index arrays directly
    std::vector<unsigned int> indices;
    std::vector<Point3> vertices;
    std::vector<Vector3> normals;
    
    // four vertices, each requires 3 floats: (x,y,z)
    vertices.push_back(Point3(0,0,0));
    vertices.push_back(Point3(1,0,0));
    vertices.push_back(Point3(1,1,0));
    vertices.push_back(Point3(0,1,0));
    
    // four normals, each requires 3 floats: (x,y,z)
    normals.push_back(Vector3(0,0,1));
    normals.push_back(Vector3(0,0,1));
    normals.push_back(Vector3(0,0,1));
    normals.push_back(Vector3(0,0,1));
    
    // indices into the arrays above for the first triangle
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    
    // indices for the second triangle, note some are reused
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    
    indexed_tris_mesh_.SetVertices(vertices);
    indexed_tris_mesh_.SetNormals(normals);
    indexed_tris_mesh_.SetIndices(indices);
    indexed_tris_mesh_.UpdateGPUMemory();
    
}


void Example::DrawUsingOpenGL() {
    // draws a set of axes at the world origin, since we are passing the identity
    // matrix for the "model" matrix.
    Matrix4 identity;
    quick_shapes_.DrawAxes(identity, view_matrix_, proj_matrix_);
    
    

    Matrix4 teapot_transform = Matrix4::Translation(Vector3(-1.5,0,0));
    shader_.Draw(teapot_transform, view_matrix_, proj_matrix_, &teapot_mesh_, DefaultShader::MaterialProperties());

    Matrix4 mesh2_transform = Matrix4::Translation(Vector3(-0.5,-0.5,0));
    shader_.Draw(mesh2_transform, view_matrix_, proj_matrix_, &triangles_mesh_, DefaultShader::MaterialProperties());

    Matrix4 mesh3_transform = Matrix4::Translation(Vector3(1,-0.5,0));
    shader_.Draw(mesh3_transform, view_matrix_, proj_matrix_, &indexed_tris_mesh_, DefaultShader::MaterialProperties());
}





