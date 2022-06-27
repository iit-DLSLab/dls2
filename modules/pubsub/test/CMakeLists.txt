set(DLS_TEST_SCOPE pubsub)

dls_register_test(pubsub
	LINK
		dls_pubsub
		dls-messages
		dls_framework
	SOURCE
		pubsub.cpp
)

dls_register_test(pub_multisub
	LINK
		dls_pubsub
		dls-messages
	SOURCE
		pub_multisub.cpp
)

dls_register_test(multipub_multisub
	LINK
		dls_pubsub
		dls-messages
	SOURCE
		multipub_multisub.cpp
)

dls_register_test(high_frequency
	LINK
		dls_pubsub
		dls-messages
		pthread
	SOURCE
		high_frequency.cpp
)
