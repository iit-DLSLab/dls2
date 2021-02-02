set(DLS_TEST_SCOPE framework)

dls_register_test(layers
	SOURCE
		layers.cpp
	LINK
		dls_framework
)
