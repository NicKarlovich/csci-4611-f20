# CSci-4611 Assignment 3:  Earthquake

## Some notes on the mingfx::Mesh class

### When you use a Mesh

:x: Instead of using: 

```cpp
my_mesh_.vertex()
my_mesh_.normal()
my_mesh_.color()
my_mesh_.tex_coords()
my_mesh_.triangle_vertices()
```

:heavy_check_mark: Use:

```cpp
my_mesh_.vertexReadOnly()
my_mesh_.normalReadOnly()
my_mesh_.colorReadOnly()
my_mesh_.tex_coordsReadOnly()
my_mesh_.triangle_verticesReadOnly()
```

### Background info

Recently, in the **mingfx::Mesh** class, we changed some function names to make it clear that these functions can only be used as getters but not setters. These functions are:

```cpp
vertexReadOnly
normalReadOnly
colorReadOnly
tex_coordsReadOnly
triangle_verticesReadOnly
```

These changes may have not been updated on the MinGfx API documentation website, so if you look at the [mingfx::Mesh Class Reference webpage](https://ivlab.github.io/MinGfx/classmingfx_1_1_mesh.html), you may still see these functions listed without the `ReadOnly` part in their names.

# Program Details

Notable Program Details:
* The has two different initalization functions based on if it is in globe form or plane form.
  * The globe form has more vertices, normals, and tex_coords in order to account for needing to "close the loop" on the cylicndrical part of the globe.
  * This fact makes it so I did not and was not able to easily do the "transform" from plane to sphere form and back
* My program also doesn't currently show all the earth quakes as described in the writeup.  I instead decided to expand on the idea by animating each earthquake as it appears on the map.
  * Each sphere is colored depending if it is in my defined "low, medium, high, or dangerous" classification.
  * Each classification has a different color that colors the earthquake which are "yellow, orange, red, dark red"
    * This is described in the DecideColor() function in quake_app.cc
  * The size the earthquakes sphere grows to and how long it is on the screen is determined by it's magnitude.
  * The time the earthquake sphere is on the screen is done in real time, so how long the animation goes for is not affected by frame rate
* One other small detailed change I made to make my simulation work is I had to rotate all LatLong coordiantes by 90 degrees around the Y-axis because of how I orientated my texture originally.

