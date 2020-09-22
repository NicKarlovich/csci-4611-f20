# Assignment 2 (Car Soccer) Worksheet

## Definitions

Use the following C++ style pseudocode definitions for Q1 and Q2:

```
/* Use this Point3 class to store x,y,z values that define a mathematical
 * point (i.e., a position) in 3-space.
 */
class Point3 {
    float x;
    float y;
    float z;
};

/* Use this Vector3 class to store x,y,z values that define a vector in
 * 3-space.  Remember, mathematically, a vector is quite different than
 * a point.  It has a direction and a magnitude but no position!
 * For vectors it is often useful to be able to compute the length,
 * also known as the magnitude, of the vector.
 */
class Vector3  {
    float x;
    float y;
    float z;
    
    // returns the length (i.e., magnitude) of the vector
    float Length() {
        return sqrt(x*x + y*y + z*z);
    }
};


/* In C++ and other languages we can define operators so we can use
 * the +, -, =, *, / operations on custom classes.  Like many graphics
 * libraries, this is what MinGfx does to make it easy to work with
 * points and vectors in code.  For example, recall from class that
 * if we have a point A (Coffman Union) and we add a vector (direction
 * and magnitude) to this, we arrive at a new point B (e.g., Murphy Hall).
 * Conceptually, a point + a vector = a new point.  Mathematically, it
 * does not make sense to add two points, but it does make sense to 
 * subtract two points.  The "difference" between the Murphy and Coffman 
 * points is a vector that tells us the direction and magnitude we would
 * need to walk from Coffman to get to Murphy.  Here's how we can write
 * that in code using Point3, Vector3, and operators like + and -.
 *
 *   Point3 murphy = Point3(5, 8, 0);
 *   Point3 coffman = Point3(4, 6, 0);
 *   Vector3 toMurphy = murphy - coffman; 
 *
 *   // or, if we were given coffman and toMurphy we could find
 *   // the point "murphy" by starting at point "coffman" and adding
 *   // the vector "toMurphy".
 *   Point3 murphy2 = coffman + toMurhpy; 
 *
 * The code that defines these opertors looks something like this:
*/

// a point + a vector = a new point
Point3 operator+(Point3 p, Vector3 v) { 
   return Point3(p.x + v.x, p.y + v.y, p.z + v.z); 
}

// a point - a point = a vector 
// the dir and magnitude needed to go from point point B to point A
Vector3 operator-(Point3 A, Point3 B) {
    return Vector3(A.x - B.x, A.y - B.y, A.z - B.z);
}

// a vector * a scalar = a new vector with scaled magnitude
Vector3 operator*(Vector3 v, float s) {
    return Vector3(v.x * s, v.y * s, v.z * s);
}



/* Given all these tools, we can define additional classes for geometries
 * that are useful in graphics.  For example, we can represent a sphere
 * using a Point3 for the position of the center point of the sphere and
 * a float for the sphere's radius.
 */
class Sphere {
    Point3 position;
    float radius;
};
```

## Q1: Eulerian Integration

In computer graphics and animation, there are many forms of integration that
are used. For simple physics models like we have in Car Soccer, Eulerian
Integration is good enough. Eulerian Integration uses velocity and position
information from the current frame, and the elapsed time to produce a position
for the next frame. Write pseudocode for determining the position of the sphere in the
next frame:

*Hint: think back to the motion equations from introductory physics. Or, look
around in the assignment handout.*

```
Vector3 velocity = Vector3(1.0, 1.0, 1.0);
float dt = 20; // milliseconds

Sphere s = Sphere {
    position: Point3(0.0, 0.0, 0.0),
    radius: 5.0,
};

s.position = /* --- Fill in the next frame position computation here --- */
```



## Q2: Sphere Intersection

In this assignment, you will need to test intersections between spheres and
other objects. Using the information contained within each sphere class,
write pseudocode to determine whether or not two spheres are intersecting
(which you can use for car/ball intersections):

```
bool sphereIntersection(Sphere s1, Sphere s2) {
    /* --- Fill in your sphere intersection code here --- */
    

}
```

To check that your intersections work, try working through the math by hand for the
following two cases.  You can write out the math on a scrap piece of paper.   You do
not need to include that detail in this worksheet.  But, do change the lines below where
it says "Fill in expected output" to indicate whether True or False would be returned:

```
Sphere s1 = Sphere {
    position: Point3(0.0, 1.0, 0.0),
    radius: 1.0,
};

Sphere s2 = Sphere {
    position: Point3(3.0, 0.0, 0.0),
    radius: 1.0,
};

Sphere s3 = Sphere {
    position: Point3(1.0, 1.0, 0.0),
    radius: 2.0,
};

print(sphereIntersection(s1, s2));
/* --- Fill in expected output (True or False) --- */

print(sphereIntersection(s1, s3));
/* --- Fill in expected output (True or False) --- */
```
