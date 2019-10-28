extern "C" GaitGenerator *create()
{
	auto p = new DummyGaitGenerator;
	// return new DummyController(std::make_shared<Dog>());
	std::cout << "I built a dummy gait generator" << std::endl;
	return p;
}

extern "C" void destroy(GaitGenerator *p)
{
	delete p;
	std::cout << "I deleted a dummy gait generator" << std::endl;
}
