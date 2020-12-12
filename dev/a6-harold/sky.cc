/** CSci-4611 Assignment 6: Harold
 */

#include "sky.h"


Sky::Sky() {
    
}


Sky::~Sky() {
    
}

void Sky::Init(ShaderProgram *stroke3d_shaderprog) {
    stroke3d_shaderprog_ = stroke3d_shaderprog;
}


/// Projects a 2D normalized screen point (e.g., the mouse position in normalized
/// device coordinates) to a 3D point on the "sky", which is really a huge sphere
/// (radius = 1500) that the viewer is inside.  This function should always return
/// true since any screen point can successfully be projected onto the sphere.
/// sky_point is set to the resulting 3D point.  Note, this function only checks
/// to see if the ray passing through the screen point intersects the sphere; it
/// does not check to see if the ray hits the ground or anything else first.
bool Sky::ScreenPtHitsSky(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                        const Point2 &normalized_screen_pt, Point3 *sky_point)
{
    Matrix4 invertedView = view_matrix.Inverse();
    Point3 eye = invertedView.ColumnToPoint3(3);

    Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, normalized_screen_pt);
    Ray eyeThroughMouse = Ray(eye, mouseIn3d - eye);

    float t;

    bool intersects = eyeThroughMouse.IntersectSphere(Point3(0, 0, 0), 1500, &t, sky_point);
    // TODO: Stitch together your worksheet implementation of this method
    return intersects;
}

/// Creates a new sky stroke mesh by projecting each vertex of the 2D mesh
/// onto the sky dome and saving the result as a new 3D mesh.
void Sky::AddSkyStroke(const Matrix4& view_matrix, const Matrix4& proj_matrix,
    const Mesh& stroke2d_mesh, const Color& stroke_color)
{
    // TODO: Create a new SkyStroke and add it to the strokes_ array.
    Mesh m = stroke2d_mesh;
    std::vector<Point3> outputVec;
    if (m.num_vertices() < 3) {
        return;
    }
    for (int i = 0; i < m.num_vertices(); i++) {
        
        Point3 temp = m.vertexReadOnly(i);
        Point2 tri_vert = Point2(temp.x(), temp.y());
        ScreenPtHitsSky(view_matrix, proj_matrix, tri_vert, &temp);
        outputVec.push_back(temp);
    }
    m.SetVertices(outputVec);
    SkyStroke temp = { m, stroke_color };
    strokes_.push_back(temp);
}

/*
/// Creates a new sky stroke mesh by projecting each vertex of the 2D mesh
/// onto the sky dome and saving the result as a new 3D mesh.
void Sky::AddSkyStroke(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                       const Mesh &stroke2d_mesh, const Color &stroke_color)
{
    // TODO: Create a new SkyStroke and add it to the strokes_ array.
    Mesh m = stroke2d_mesh;
    std::vector<Point3> outputVec;
    for (int i = 0; i < m.num_vertices(); i++) {
        std::vector<unsigned int> tri_verts = m.triangle_verticesReadOnly(i);
        Point3 temp0 = m.vertexReadOnly(tri_verts[0]);
        Point3 temp1 = m.vertexReadOnly(tri_verts[1]);
        Point3 temp2 = m.vertexReadOnly(tri_verts[2]);
        Point2 tri_vert0 = Point2(temp0.x(), temp0.y());
        Point2 tri_vert1 = Point2(temp1.x(), temp1.y());
        Point2 tri_vert2 = Point2(temp2.x(), temp2.y());
        ScreenPtHitsSky(view_matrix, proj_matrix, tri_vert0, &temp0);
        ScreenPtHitsSky(view_matrix, proj_matrix, tri_vert1, &temp1);
        ScreenPtHitsSky(view_matrix, proj_matrix, tri_vert2, &temp2);

        m.UpdateTriangle(i, temp0, temp1, temp2);
    }

    SkyStroke temp;
    temp.mesh = m;
    temp.color = stroke_color;

    strokes_.push_back(temp);
}
*/

/// Draws all of the sky strokes
void Sky::Draw(const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {

    // Precompute matrices needed in the shader
    Matrix4 model_matrix; // identity
    Matrix4 modelview_matrix = view_matrix * model_matrix;
    
    // Draw sky meshes
    stroke3d_shaderprog_->UseProgram();
    stroke3d_shaderprog_->SetUniform("modelViewMatrix", modelview_matrix);
    stroke3d_shaderprog_->SetUniform("projectionMatrix", proj_matrix);
    for (int i=0; i<strokes_.size(); i++) {
        stroke3d_shaderprog_->SetUniform("strokeColor", strokes_[i].color);
        strokes_[i].mesh.Draw();
    }
    stroke3d_shaderprog_->StopProgram();
}

