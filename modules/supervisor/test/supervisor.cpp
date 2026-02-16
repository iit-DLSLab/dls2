#include "dls2/supervisor/pegasus_orchestrator.hpp"

int main()
{
    dls::PegasusOrchestrator plugin_sup("dls_supervisor", 1, 1, 6);
    plugin_sup.execute();
	return 0;
}