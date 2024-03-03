![example1](https://github.com/stbelousov/mesh-visualizer/assets/3650015/bf5d15f3-89fc-46e0-8c3a-f80b3d4967ac)

![example2](https://github.com/stbelousov/mesh-visualizer/assets/3650015/12873aa2-8dd6-43a0-b376-7dea957ccf73)

# Description
A desktop GUI application for 3D objects visualization.  
Each 3D object is described in the input file (e.g., sample.input) as a mesh of triangular facets.  
Each facet may also contain temperature, which is visualized as well.

The implementation is based on the Qt widget toolkit.  
A few screenshots are provided in the "examples" folder.

Press and hold the left mouse button to rotate the scene.

# Input format

number_of_objects

"scene_name" "object_name" number_of_facets

...

facet_first_vertex_coordinates

facet_second_vertex_coordinates

facet_third_vertex_coordinates

facet_normal_coordinates

facet_temperature

...
