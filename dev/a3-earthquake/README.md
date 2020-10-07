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
