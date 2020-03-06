# This message defines a local square heightmap around a set of four points
Header header

# number of cells per line (rows or cols)
uint32 size

# resolution of each cell, in meters
float32 resolution

# Requested footholds, in the Horizontal frame
geometry_msgs/Point des_LF
geometry_msgs/Point des_RF
geometry_msgs/Point des_LH
geometry_msgs/Point des_RH

# Corresponding HeightMaps, in the Horizontal frame
float32[] LF   
float32[] RF
float32[] LH
float32[] RH
