/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef CAR_H_
#define CAR_H_

#include <mingfx.h>

/// Small data structure for a car
class Car {
public:

    /// The constructor sets the static properties of the car, like its size,
    /// and then calls Reset() to reset the position, velocity, and any other
    /// dynamic variables that change during game play.
    Car() : size_(3, 2, 4), collision_radius_(2.5), orientation_(0,0) {
        Reset();
    }

    /// Nothing special needed in the constructor
    virtual ~Car() {}

    /// Resets all the dynamic variables, so if you call this after a goal, the
    /// car will go back to its starting position.
    void Reset() {
        position_ = Point3(0, size_[1] / 2, 45);
        orientation_ = Vector2(0, 0);
    }

    float collision_radius() { return collision_radius_; }

    Vector3 size() { return size_; }

    Point3 position() { return position_; }
    Vector2 get_orientation() { return orientation_; }
    void set_position(const Point3 &p) { position_ = p; }
    void set_orienation(const Vector2 &v) { orientation_ = v; }
    
    void set_rotation(const int &r) { orientation_[0] = r; }
    void set_speed(const double &s) { orientation_[1] = s; }

    int get_rotation() { return orientation_[0]; }
    double get_speed() { return orientation_[1]; }
    int get_max_speed() { return MAX_SPEED; }
    
private:
    // You will probably need to store some additional data here, e.g., speed.
    
    Vector3 size_;
    float collision_radius_;
    Point3 position_;
    Vector2 orientation_; // 2d vector, first element is rotation, second element is speed.
    //max is 151, but we divide by 5 in every location in order to make speed up slower (takes longer to get to 150 than 30)
    // but since we divide by 5, max speed the car can actually reach is 30, this is why we divide speed by 5 in most calculations.
    const int MAX_SPEED = 151; 
};

#endif
