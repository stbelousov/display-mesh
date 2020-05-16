# Description
A desktop GUI application for 3D objects visualization.  
Each 3D object is described in the input file (e.g., sample.input) as a mesh of triangular facets.  
Each facet may also contain temperature, which is visualized as well.

The implementation is based on the Qt widget toolkit.

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
