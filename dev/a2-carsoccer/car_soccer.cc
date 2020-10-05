/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT)) {
        dir[0]--;
    }
    if (IsKeyDown(GLFW_KEY_RIGHT)) {
        dir[0]++;
    }
    if (IsKeyDown(GLFW_KEY_UP)) {
        dir[1]++;
    }
    if (IsKeyDown(GLFW_KEY_DOWN)) {
        dir[1]--;
    }
    return dir;
}

void CarSoccer::ResetSimulation() {
    //generate number between 0 and RAND_MAX, divide to it's between 0 and 1, multiply by to make it 0 to 2, then -1 to make it -1 to 1
    //for x and z this gives us a random direction in both positive and negative directions, can always scale value later.
    double x = ((((double)(std::rand())) / RAND_MAX) * 2) - 1;
    double y = ((double)(std::rand())) / RAND_MAX;
    double z = ((((double)(std::rand())) / RAND_MAX) * 2) - 1;
    // Here's where you could call some form of launch_ball();

    //scale
    double SCALE_ = .2;
    x = x * SCALE_;
    y = y * SCALE_ * 2;
    z = z * SCALE_;

    ball_.Reset();
    ball_.set_velocity(Vector3(x, y, z));
    car_.Reset();
}


void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        ResetSimulation();
    }
}


void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
    Point3 ball_location = ball_.position();
    Vector3 new_velocity = ball_.get_velocity();
    if (ball_location[1] - ball_.radius() <= 0) { // if on ground don't add gravity acceleration.
        new_velocity[1] = new_velocity[1];
    } else {
        new_velocity[1] = new_velocity[1] - gravity_ * timeStep;
    }
    ball_.set_velocity(new_velocity);
    ball_.set_position(ball_location + new_velocity);

    UpdateCarLocation(timeStep);
    FixCarCollision(timeStep);
    FixBallCollision(timeStep);
    if (InGoal()) {
        ResetSimulation();
    }
}

//updates car's location
void CarSoccer::UpdateCarLocation(double timeStep) {
    Vector2 orient = joystick_direction(); //first variable is left right, 2nd variable is forwards backwards.
    int car_speed = car_.get_speed();
    const int MAX_SPEED = car_.get_max_speed();
    //Update Car's Speed 
    std::cout << "car speed: " << car_speed << "  Max: " << MAX_SPEED << std::endl;
    if (orient[1] > 0 && car_speed > (-MAX_SPEED) && car_speed < MAX_SPEED - 1) { //allow for raising of speed
        car_.set_speed(orient[1] + car_.get_speed());
    }
    if (orient[1] < 0 && car_speed < MAX_SPEED && car_speed > (-MAX_SPEED) + 1) { //allow for lowering of speed
        car_.set_speed(orient[1] + car_.get_speed());
    }

    //set car's rotation from 0 to 359.
    int rotation = car_.get_rotation();
    car_speed = car_.get_speed();
    if (car_speed != 0) {
        if (rotation <= 0 && orient[0] < 0) {
            car_.set_rotation(359);
        }
        else if (rotation >= 359 && orient[0] > 0) {
            car_.set_rotation(0);
        }
        else {
            car_.set_rotation(((int)orient[0] + car_.get_rotation()) % 360); //keep orientation between 0 and 359, to keep understanding easy
        }
    }
    
    Point3 curr = car_.position();
    rotation = car_.get_rotation();
    Point3 translation;
    double const SCALE_ = .001;

    // computing how the car is moved,  There is some weird math here (like multiplying by -1)
    // that we do so that the coordinate system we expect matches what the game's is.
    // We devide the car's speed by 5 here, explanation given in car.h
    translation[0] = (sin((rotation) * 3.14159/180) * car_.get_speed() / 5 * timeStep) + curr[0];  // x
    translation[1] = 1; // y
    translation[2] = -1 * (cos((rotation) * 3.14159 / 180) * car_.get_speed() / 5 * timeStep) + curr[2]; // z

    car_.set_position(translation);
}

//checking if car is out of bounds.
void CarSoccer::FixCarCollision(double timeStep) {
    Point3 location = car_.position();
    float radius = car_.collision_radius();
    Vector2 orientation = car_.get_orientation();

    if (std::abs(location[0]) > 40 - radius) {
        //std::cout << "x out" << std::endl;
        if (location[0] < 0) {
            location[0] = -40 + radius;
        }
        else {
            location[0] = 40 - radius;
        }
    }

    if (std::abs(location[2]) > 50 - radius) {
        //std::cout << "z out" << std::endl;
        if (location[2] < 0) {
            location[2] = -50 + radius;
        }
        else {
            location[2] = 50 - radius;
        }
    }
    car_.set_position(location);
}

// checks if ball is in the goal.
bool CarSoccer::InGoal() {
    Point3 location = ball_.position();
    //std::cout << location << std::endl;
    float radius = ball_.radius();
    const int GOAL_OFFSET = 0;
    if (location[0] <= 10 && location[0] >= -10) {
        if (location[1] <= 10) {
            if (location[2] >= 50 - radius - GOAL_OFFSET || location[2] <= -50 + radius + GOAL_OFFSET) {
                std::cout << "GOAL!" << std::endl;
                return true;
            }
        }
    }
    return false;
}

// fix if the ball is outside bounds or if it collides with car.
void CarSoccer::FixBallCollision(double timeStep) {
    Point3 location = ball_.position();
    float radius = ball_.radius();
    Vector3 velocity = ball_.get_velocity();
    
    if (std::abs(location[0]) > 40 - radius) {
        if (location[0] < 0) {
            location[0] = -40 + radius;
        }
        else {
            location[0] = 40 - radius;
        }
        velocity[0] = -velocity[0];
        ball_.set_velocity(velocity * 0.8);
    }

    if (std::abs(location[1]) > 35 - radius) {
        location[1] = 35 - radius;
        velocity[1] = -velocity[1];
        ball_.set_velocity(velocity * 0.8);
    }

    if (location[1] < radius + 0) {
        location[1] = 0 + radius;
        velocity[1] = -velocity[1];
        ball_.set_velocity(velocity * 0.8);
    }

    if (std::abs(location[2]) > 50 - radius) {
        if (location[2] < 0) {
            location[2] = -50 + radius;
        } else {
            location[2] = 50 - radius;
        }
        velocity[2] = -velocity[2];
        ball_.set_velocity(velocity * 0.8);
    }

    velocity = ball_.get_velocity();
    Vector3 normal = location - car_.position();
    float len_of_normal = sqrt(normal.Dot(normal));
    if (len_of_normal <= radius + car_.collision_radius()) {
        //there was a collision between ball and car

        //move ball so it's not colliding with car
        float intrusion_distance = radius + car_.collision_radius() - len_of_normal;

        Vector3 carVelocity;
        float rotation = car_.get_rotation();
        carVelocity[0] = (sin((rotation) * 3.14159 / 180) * car_.get_speed() / 5 * timeStep);
        carVelocity[1] = 0; 
        carVelocity[2] = -1 * (cos((rotation) * 3.14159 / 180) * car_.get_speed() / 5 * timeStep);
        Vector3 relativeVelocity = velocity - carVelocity;
        Vector3 bounceVelocity = (((relativeVelocity - (2 * relativeVelocity.Dot(normal) * normal)) * timeStep) + carVelocity);
        bounceVelocity[1] += 0.1; //give it a bit of vertical velocity

        //divide by 5 in order to account for max speed offset (explained in car.h)
        //divide by 20 in order to account for max speed magnitude
        bounceVelocity = bounceVelocity * car_.get_speed() / 5 / 20 ; 

        ball_.set_velocity(bounceVelocity);

        //moving the ball out of bounds.
        Vector3 test = location - car_.position();
        while (sqrt(test.Dot(test)) <= radius + car_.collision_radius()) {
            location = location + bounceVelocity;
            test = location - car_.position();
        }
    }
    ball_.set_position(location);
}

void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
    //gravity_ = 0.981; //setup global
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);
    
    // Draw the field with the field texture on it.
    Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Rotation(car_.position(), Vector3(0, -1, 0), (car_.get_rotation()) * 3.14159 / 180) * //this line rotates car
        Matrix4::Translation(car_.position() - Point3(0, 0, 0)) *
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5, 0.5, 0.5));
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);
    
    
    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);
    
    
    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);
    
    
    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()

    Color outline(1, 1, 1);
    Color orangeGoal(1, .51, 0);
    Color blueGoal(.1, .83, 1);
    std::vector<Point3> playArea = { Point3(-40, 0, 50), Point3(-40, 35, 50), Point3(-40, 35, -50), Point3(-40, 0, -50), Point3(40, 0, -50), Point3(40, 35, -50), Point3(40, 35, 50), Point3(40, 0, 50) };
    std::vector<Point3> playArea2 = { Point3(40, 35, 50), Point3(-40, 35, 50), Point3(-40, 0, 50), Point3(-40, 0, -50), Point3(-40, 35, -50), Point3(40, 35, -50), Point3(40, 0, -50), Point3(40, 0, 50) };
    std::vector<Point3> orangeGoalArea = { Point3(10, 10, 50), Point3(10, 0, 50), Point3(8, 10, 50), Point3(8, 0, 50), Point3(6, 10, 50), Point3(6, 0, 50), Point3(4, 10, 50), Point3(4, 0, 50),
        Point3(2, 10, 50), Point3(2, 0, 50), Point3(0, 10, 50), Point3(0, 0, 50), Point3(-2, 10, 50), Point3(-2, 0, 50), Point3(-4, 10, 50), Point3(-4, 0, 50), Point3(-6, 10, 50),
        Point3(-6, 0, 50), Point3(-8, 10, 50), Point3(-8, 0, 50), Point3(-10, 10, 50), Point3(-10, 0, 50),
        Point3(10, 0, 50), Point3(-10, 0, 50), Point3(10, 2, 50), Point3(-10, 2, 50), Point3(10, 4, 50), Point3(-10, 4, 50), Point3(10, 6, 50), Point3(-10, 6, 50), Point3(10, 8, 50), Point3(-10, 8, 50), Point3(10, 10, 50), Point3(-10, 10, 50),
    };

    std::vector<Point3> blueGoalArea = { Point3(10, 10, -50), Point3(10, 0, -50), Point3(8, 10, -50), Point3(8, 0, -50), Point3(6, 10, -50), Point3(6, 0, -50), Point3(4, 10, -50), Point3(4, 0, -50),
        Point3(2, 10, -50), Point3(2, 0, -50), Point3(0, 10, -50), Point3(0, 0, -50), Point3(-2, 10, -50), Point3(-2, 0, -50), Point3(-4, 10, -50), Point3(-4, 0, -50), Point3(-6, 10, -50),
        Point3(-6, 0, -50), Point3(-8, 10, -50), Point3(-8, 0, -50), Point3(-10, 10, -50), Point3(-10, 0, -50),
        Point3(10, 0, -50), Point3(-10, 0, -50), Point3(10, 2, -50), Point3(-10, 2, -50), Point3(10, 4, -50), Point3(-10, 4, -50), Point3(10, 6, -50), Point3(-10, 6, -50), Point3(10, 8, -50), Point3(-10, 8, -50), Point3(10, 10, -50), Point3(-10, 10, -50),
    };


    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, outline, playArea, QuickShapes::LinesType::LINE_LOOP, .1);
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, outline, playArea2, QuickShapes::LinesType::LINE_LOOP, .1);
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, orangeGoal, orangeGoalArea, QuickShapes::LinesType::LINES, .2);
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, blueGoal, blueGoalArea, QuickShapes::LinesType::LINES, .2);

}
