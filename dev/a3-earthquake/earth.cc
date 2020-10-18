/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();
    
    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init consts
    const int nslices = 10;
    const int nstacks = 10;
    

    //important variables
    std::vector<unsigned int> indices;
    std::vector<Point3> vertices;
    std::vector<Point2> tex_coords_;
    std::vector<Vector3> normals;
    

    // defining triangle steps
    double horizontal_step = (M_PI * 2) / nslices;
    double vertical_step = M_PI / nstacks;

    // < nslices + 1 because fence post problem, 4 slices means 5 boundries.
    // going by row then column.
    for (int i = 0; i < nstacks + 1; i++) {
        for (int j = 0; j < nslices + 1; j++) {
            vertices.push_back(Point3(-M_PI + j * horizontal_step, (M_PI / 2) - i * vertical_step, 0));
            tex_coords_.push_back(Point2(1 / (float) nslices * j, 1 / (float) nstacks * i));
            normals.push_back(Vector3(0, 0, 1).ToUnit());
        }
    }
    
    // remember to push back indicies in counter clockwise fashion!!
    //add bottom-left triangle, by row then column -> top left, bottom left, bottom right
    for (int i = 0; i < nstacks; i++) {
        for (int j = 0; j < nslices; j++) {
            indices.push_back(i * (nslices + 1) + j);
            indices.push_back(i * (nslices + 1) + (nslices + 1) + j);
            indices.push_back(i * (nslices + 1) + (nslices + 1) + j + 1);
        }
    }

    //add top-right triangle by row then column -> bottom right, top right, top left.
    for (int i = 0; i < nstacks; i++) {
        for (int j = 0; j < nslices; j++) {
            indices.push_back(i * (nslices + 1) + (nslices + 1) + j + 1);
            indices.push_back(i * (nslices + 1) + j + 1);
            indices.push_back(i * (nslices + 1) + j);
        }
    }

    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords_);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.UpdateGPUMemory();
}

void Earth::InitGlobe(const std::vector<std::string>& search_path) {
    // init shader program
    shader_.Init();

    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init consts
    const int nslices = 10;
    const int nstacks = 10;

    //important variables
    std::vector<unsigned int> indices;
    std::vector<Point3> vertices;
    std::vector<Point2> tex_coords_;
    std::vector<Vector3> normals;

    //incrementers from 0 to 1 for mapping textures
    double tex_slice_increment = 1 / (double)(nslices + 1);
    double tex_stack_increment = 1 / (double)(nstacks);

    // more consts
    const double MASTER_RADIUS = 1.00;
    const double MASTER_DIAMETER = MASTER_RADIUS * 2;
    double radius = MASTER_RADIUS;
    
    double width_init = -M_PI; // from -pi to pi
    double width_increment = M_PI * 2 / (nslices + 1); // how much each slice increments counterclockwise around y-axis
    double vertical_increment = MASTER_RADIUS / 2 / nstacks;

    double height_init = M_PI / 2; // from pi/2 to -pi/2
    double scale_factor = 1 / height_init;
    height_init = height_init * scale_factor; //normalized to be 1 length so it matches horizontal axis.
    double height_increment = M_PI / nstacks;
    height_increment = height_increment * scale_factor;
    
    // nslices + 1 and nstacks + 1 because fence post problem, 4 slices means 5 boundries.
    // going by row then column.
    for (int i = 0; i < nstacks + 1; i++) { // height location (y) (on a 2d plane)
        int j = 0;
        double ha, va, ht, vt, x, z;
        
        double y_inc = MASTER_DIAMETER / nstacks;  // height of each stack
        double y_pos = MASTER_RADIUS - y_inc * i;  // which y-pos triangle's are generated at.
        
        radius = sqrt(pow(MASTER_RADIUS,2) - pow(y_pos,2));  // solving for radius, x^2 + y^2 = r^2

        for (j = 0; j < nslices + 1; j++) { // witdth location (x) (on a 2d plane)
            ha = width_init + j * width_increment; //ha - horizontal radian location.
            va = height_init - i * height_increment; //va - vertical radian location.
            ht = 0 + j * tex_slice_increment; //ht - horizontal tex location
            vt = 0 + i * tex_stack_increment; //vt - vertical tex location
            double x = radius * cos(ha);
            double z = radius * -1.0 * sin(ha);
            Point3 temp = Point3(x, va, z);
            vertices.push_back(temp);
            Point2 texTemp = Point2(ht, vt);
            tex_coords_.push_back(texTemp);
            normals.push_back(Vector3(x, va, z).ToUnit());
        }
        // closing the "loop" with points 0 and nslices + 1 overlapping. along rotation axis.
        // this means that there are even more vertices/texcoords/normals than it has when it's on a 2d plane
        x = radius * cos(ha + width_increment);
        z = radius * -1.0 * sin(ha + width_increment);
        vertices.push_back(Point3(x, va, z));
        tex_coords_.push_back(Point2(ht + tex_slice_increment, vt));
        normals.push_back(Vector3(x, va, z).ToUnit());
    }

    // remember to push back indicies in counter clockwise fashion!!
    //add bottom-left triangle, by row then column -> top left, bottom left, bottom right
    for (int i = 0; i < nstacks; i++) {
        int j = 0;
        for (j = 0; j < nslices + 1; j++) {
            indices.push_back(i * (nslices + 2) + j);
            indices.push_back(i * (nslices + 2) + (nslices + 2) + j);
            indices.push_back(i * (nslices + 2) + (nslices + 2) + j + 1);
        }
    }

    //add top-right triangle by row then column. -> bottom right, top right, top left.
    for (int i = 0; i < nstacks; i++) {
        int j = 0;
        for (j = 0; j < nslices + 1; j++) {
            indices.push_back(i * (nslices + 2) + (nslices + 2) + j + 1);
            indices.push_back(i * (nslices + 2) + j + 1);
            indices.push_back(i * (nslices + 2) + j);
        }
    }
    
    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords_);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.UpdateGPUMemory();
}

void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    longitude = longitude + 90; //90 degree offset to match rotation of earth texture initially.
    latitude = GfxMath::ToRadians(latitude);
    longitude = GfxMath::ToRadians(longitude);
    return Point3(cos(latitude) * sin(longitude), sin(latitude), cos(latitude) * cos(longitude));
}

// Literally just convert from degrees to radians.
Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    return Point3(GfxMath::ToRadians(longitude), GfxMath::ToRadians(latitude),0);
}

void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_verticesReadOnly(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertexReadOnly(indices[0]));
        loop.push_back(earth_mesh_.vertexReadOnly(indices[1]));
        loop.push_back(earth_mesh_.vertexReadOnly(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}

