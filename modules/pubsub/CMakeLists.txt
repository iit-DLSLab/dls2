# =================================== fastdds ==================================
# TODO this library was added late. Ideally it should wrap the libraries
# fastrtps and fastcdr in its public interface. Anyone who uses those should then
# be made to use this instead. This is left as an exercise to the reader
add_library(dls_pubsub
	STATIC
	messaging/participant.cpp
)
target_include_directories(dls_pubsub PUBLIC include)
dls_install(dls_pubsub)
set_property(TARGET dls_pubsub PROPERTY POSITION_INDEPENDENT_CODE TRUE)
