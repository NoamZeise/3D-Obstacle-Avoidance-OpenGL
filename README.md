# 3D-Obstacle-Avoidance-OpenGL
a short project to help me familiarize myself with the basics of OpenGL


I began learning a little about OpenGL at the beginning of the week. Beforehand I was not too familar with model/view/projection matricies. It was a good experience having to do that manually, as well as using vertex buffer objects to send vertex data to the GPU and vertex array objects to quicky reference the stored verticies. Having to write and apply shaders was also entirely new to me.

I wrote a small shader class to load a vertex and fragment shader from a source file, and link them through a program.

I wrote a shape class which takes vertices as input and stores information such as a reference to the VAO and the amount of triangles a shape needs.

I basically took the logic of my SDL2 Project and applied it to the 3D realm. The collision detection is still 2D, as objects are stationary on the y-axis.
