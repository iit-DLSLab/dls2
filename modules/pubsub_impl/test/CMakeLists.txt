dls_register_test(pubsub
	LINK
		dls_pubsub_impl
		dls_dls2_messaging
	SOURCE
		pubsub.cpp
)

dls_register_test(pub_multisub
	LINK
		dls_pubsub_impl
		dls_dls2_messaging
	SOURCE
		pub_multisub.cpp
)
