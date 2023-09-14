set(DLS_TEST_SCOPE pubsub)

dls_register_test(pubsub
	LINK
		dls_com
		dls_messages
		dls_framework
	SOURCE
		pubsub.cpp
)

dls_register_test(pub_multisub
	LINK
		dls_com
		dls_messages
		dls_framework
	SOURCE
		pub_multisub.cpp
)

dls_register_test(multipub_multisub
	LINK
		dls_com
		dls_messages
		dls_framework
	SOURCE
		multipub_multisub.cpp
)

dls_register_test(high_frequency
	LINK
		dls_com
		dls_messages
		pthread
		dls_framework
	SOURCE
		high_frequency.cpp
)

add_executable(DLS2_Client domain_participant.cpp)
target_link_libraries(DLS2_Client
    PUBLIC
		dls_framework
)