#include <dls2/controller/dummy_controller.hpp>

int main()
{
	std::shared_ptr<iit::dog::Dog> pDog;
	dls::DummyController dummy_controller(pDog);
	while(true)
	{
		dummy_controller.run(std::chrono::system_clock::now());
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}
