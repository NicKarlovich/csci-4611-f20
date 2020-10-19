/** CSci-4611 Assignment 3:  Earthquake
 */

#include "quake_app.h"
#include "config.h"
#include "earth.h"

#include <iostream>
#include <sstream>
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

// Number of seconds in 1 year (approx.)
const int PLAYBACK_WINDOW = 12 * 28 * 24 * 60 * 60;
std::chrono::high_resolution_clock::time_point last_update;
const int TIME_SCALE_FACTOR = 200;

using namespace std;

QuakeApp::QuakeApp() : GraphicsApp(1280, 720, "Earthquake"),
    playback_scale_(15000000.0 ), debug_mode_(false), globe_mode_(false)
{
    // Define a search path for finding data files (images and earthquake db)
    search_path_.push_back(".");
    search_path_.push_back("./data");
    search_path_.push_back(DATA_DIR_INSTALL);
    search_path_.push_back(DATA_DIR_BUILD);
    
    quake_db_ = EarthquakeDatabase(Platform::FindFile("earthquakes.txt", search_path_));
    current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();
    last_idx_ = quake_db_.min_index();
    visibleQuakes.push_back(quake_db_.earthquake(last_idx_));
    visibleQuakesRemainingTime.push_back(quake_db_.earthquake(last_idx_).magnitude()/2);
    last_update = std::chrono::high_resolution_clock::now();
 }


QuakeApp::~QuakeApp() {
}


void QuakeApp::InitNanoGUI() {
    // Setup the GUI window
    nanogui::Window *window = new nanogui::Window(screen(), "Earthquake Controls");
    window->setPosition(Eigen::Vector2i(10, 10));
    window->setSize(Eigen::Vector2i(400,200));
    window->setLayout(new nanogui::GroupLayout());
    
    date_label_ = new nanogui::Label(window, "Current Date: MM/DD/YYYY", "sans-bold");
    
    globe_btn_ = new nanogui::Button(window, "Globe");
    globe_btn_->setCallback(std::bind(&QuakeApp::OnGlobeBtnPressed, this));
    globe_btn_->setTooltip("Toggle between map and globe.");
    
    new nanogui::Label(window, "Playback Speed", "sans-bold");
    
    nanogui::Widget *panel = new nanogui::Widget(window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    nanogui::Slider *slider = new nanogui::Slider(panel);
    slider->setValue(0.5f);
    slider->setFixedWidth(120);
    
    speed_box_ = new nanogui::TextBox(panel);
    speed_box_->setFixedSize(Eigen::Vector2i(60, 25));
    speed_box_->setValue("50");
    speed_box_->setUnits("%");
    slider->setCallback(std::bind(&QuakeApp::OnSliderUpdate, this, std::placeholders::_1));
    speed_box_->setFixedSize(Eigen::Vector2i(60,25));
    speed_box_->setFontSize(20);
    speed_box_->setAlignment(nanogui::TextBox::Alignment::Right);
    
    nanogui::Button* debug_btn = new nanogui::Button(window, "Toggle Debug Mode");
    debug_btn->setCallback(std::bind(&QuakeApp::OnDebugBtnPressed, this));
    debug_btn->setTooltip("Toggle displaying mesh triangles and normals (can be slow)");
    
    screen()->performLayout();
}

void QuakeApp::OnLeftMouseDrag(const Point2 &pos, const Vector2 &delta) {
    // Optional: In our demo, we adjust the tilt of the globe here when the
    // mouse is dragged up/down on the screen.
}


void QuakeApp::OnGlobeBtnPressed() {
    // TODO: This is where you can switch between flat earth mode and globe mode
    if (!globe_mode_) {
        earth_.InitGlobe(search_path_);
    } else {
        earth_.Init(search_path_);
    }
    globe_mode_ = !globe_mode_;
}

void QuakeApp::OnDebugBtnPressed() {
    debug_mode_ = !debug_mode_;
}

void QuakeApp::OnSliderUpdate(float value) {
    speed_box_->setValue(std::to_string((int) (value * 100)));
    playback_scale_ = 30000000.0*value;
}


void QuakeApp::UpdateSimulation(double dt)  {
    // Advance the current time and loop back to the start if time is past the last earthquake
    current_time_ += playback_scale_ * dt;
    if (current_time_ > quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds()) {
        current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();
    }
    if (current_time_ < quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds()) {
        current_time_ = quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds();
    }
    
    Date d(current_time_);
    stringstream s;
    s << "Current date: " << d.month()
        << "/" << d.day()
        << "/" << d.year();
    date_label_->setCaption(s.str());
    
    // TODO: Any animation, morphing, rotation of the earth, or other things that should
    // be updated once each frame would go here.
}


void QuakeApp::InitOpenGL() {
    // Set up the camera in a good position to see the entire earth in either mode
    proj_matrix_ = Matrix4::Perspective(60, aspect_ratio(), 0.1, 50);
    view_matrix_ = Matrix4::LookAt(Point3(0,0,3.5), Point3(0,0,0), Vector3(0,1,0));
    glClearColor(0.0, 0.0, 0.0, 1);
    
    // Initialize the earth object
    earth_.Init(search_path_);

    // Initialize the texture used for the background image
    stars_tex_.InitFromFile(Platform::FindFile("iss006e40544.png", search_path_));
}

void QuakeApp::DrawUsingOpenGL() {
    quick_shapes_.DrawFullscreenTexture(Color(1,1,1), stars_tex_);
    
    // You can leave this as the identity matrix and we will have a fine view of
    // the earth.  If you want to add any rotation or other animation of the
    // earth, the model_matrix is where you would apply that.
    //Matrix4 model_matrix;
    
    Matrix4 model_matrix;
    
    // Draw the earth
    earth_.Draw(model_matrix, view_matrix_, proj_matrix_);
    if (debug_mode_) {
        earth_.DrawDebugInfo(model_matrix, view_matrix_, proj_matrix_);
    }
    
    
    // TODO: You'll also need to draw the earthquakes.  It's up to you exactly
    // how you wish to do that.

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_span = now - last_update;
    double dt = time_span.count();
    last_update = now;


    int idx = quake_db_.FindMostRecentQuake(Date(current_time_));
    if (idx == last_idx_) {
        //do nothing.
    } else {
        // loop to ensure that if on any timestep it skipped multiple earthquakes, all of them will be included.
        for (last_idx_ = last_idx_ + 1; last_idx_ < idx; last_idx_++) {
            visibleQuakes.push_back(quake_db_.earthquake(last_idx_));
            visibleQuakesRemainingTime.push_back(ceil(visibleQuakes.back().magnitude()) * TIME_SCALE_FACTOR);
        }
        visibleQuakes.push_back(quake_db_.earthquake(idx));
        visibleQuakesRemainingTime.push_back(ceil(visibleQuakes.back().magnitude()) * TIME_SCALE_FACTOR);
        last_idx_ = idx;
    }
    
    for (int i = 0; i < visibleQuakes.size(); i++) {
        Earthquake temp = visibleQuakes[i];
        int timeRemaining = visibleQuakesRemainingTime[i];
        DrawQuake(temp.longitude(), temp.latitude(), temp.magnitude(), timeRemaining);
        timeRemaining = timeRemaining - (int)dt;
        if (timeRemaining <= 0) {            
            //remove quakes that will no longer be visible.
            visibleQuakes.erase(visibleQuakes.begin() + i);
            visibleQuakesRemainingTime.erase(visibleQuakesRemainingTime.begin() + i);
            i--;
        }
        else {
            visibleQuakesRemainingTime[i] = timeRemaining;
        }
    }
}

void QuakeApp::DrawQuake(float lon, float lat, float mag, int stage) {
    float scaleMag = mag * mag * mag * mag / 50000; //using exponents to make bigger earthquakes more "different" than smaller ones.
    float max_stage = mag * TIME_SCALE_FACTOR;
    float increment = 1 / max_stage;
    float subtractFactor = increment * stage * scaleMag;
    Matrix4 quake = Matrix4::Scale(Vector3(scaleMag, scaleMag, scaleMag) -
        Vector3(subtractFactor, subtractFactor, subtractFactor));
    Point3 radCoords;
    Matrix4 translate;
    if (globe_mode_) {
        radCoords = earth_.LatLongToSphere(lat, lon);
        translate = Matrix4::Translation(Vector3(radCoords.x(), radCoords.y(), radCoords.z()));
    } else {
        radCoords = earth_.LatLongToPlane(lat, lon);
        translate = Matrix4::Translation(Vector3(radCoords.x(), radCoords.y(), 0.01));
    }
    

    Matrix4 combo = translate * quake;
    Color magColor = DecideColor(mag);
    quick_shapes_.DrawSphere(combo, view_matrix_, proj_matrix_, magColor);
}

Color QuakeApp::DecideColor(float mag) {
    Color danger = Color(.56, .078, .086); //dark red
    Color high = Color(.886, .3, .11); // red
    Color medium = Color(.85, .46, .207); //orange
    Color low = Color(1, .86, .368); //yellow

    if (mag > 8.0) {
        return danger;
    } else if (mag > 7.5) {
        return high;
    } else if (mag > 6.5) {
        return medium;
    } else {
        return low;
    }
}

bool QuakeApp::GetGlobeMode() {
    return globe_mode_;
}
