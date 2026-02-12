#include "dls2/supervisor/fake_control_station.hpp"

int main()
{
    dls::FakeControlStation plugin_cs("fake_cs");
    plugin_cs.execute();
	return 0;
}

