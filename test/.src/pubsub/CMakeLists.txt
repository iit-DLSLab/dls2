# create_test(multiple_pub_sub_with_single_participant_to_same_topic
# 	SOURCE
# 		multiple_pub_sub_with_single_participant_to_same_topic.cpp
# 	LINK
# 		dls_messages
# 		dls_com
# 		fastdds
# 		fastcdr
# 		dls_framework
# )

# create_test(high_frequency_test
# 	SOURCE
# 		high_frequency_test.cpp
# 	LINK
# 		dls_messages
# 		dls_com
# 		fastdds
# 		fastcdr
# 		dls_framework
# )

create_test(pubsub
	SOURCE
		pubsub.cpp
	LINK
		dls_messages
		dls_com
		dls_framework
		fastdds
		fastcdr
		catch2
)

create_test(pub_multisub
	SOURCE
		pub_multisub.cpp
	LINK
		dls_framework
		dls_messages
		fastdds
		fastcdr
		catch2
)

# create_test(pub_multisub_multiprocess
# 	SOURCE
# 		pub_multisub_multiprocess.cpp
# 	LINK
# 		dls_framework
# 		dls_messages
# 		fastdds
# 		fastcdr
# 		catch2
# )

add_executable(dummy_blind_state_publisher
	blind_state_test.cpp
)

target_link_libraries(dummy_blind_state_publisher
	dls_framework
	dls_com
	dls_messages
	fastdds
	fastcdr
)

add_executable(dummy_blind_state_subscriber
	blind_state_sub.cpp
)

target_link_libraries(dummy_blind_state_subscriber
	dls_framework
	dls_messages
	fastdds
	fastcdr
)

add_executable(bare_dummy_controller
	bare_dummy_controller.cpp
)
target_link_libraries(bare_dummy_controller
	dls_dummy_controller
	dls_controller_interface
	dls_messages
	dls_motion_generator
	dls_command
	dls_logging
	dls_framework
	fastdds
	fastcdr
)

# create_test(multi_pubsub_one_to_one
# 	SOURCE
# 		multi_pubsub_one_to_one.cpp
# 	LINK
# 		dls_framework
# 		dls_messages
# 		fastdds
# 		fastcdr
# )
