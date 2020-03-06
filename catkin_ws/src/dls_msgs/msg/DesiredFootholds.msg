# This message defines a set of four desired footholds to obtain a map from
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
