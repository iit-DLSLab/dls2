#include "dls2/supervisor/pegasus_orchestrator.hpp"

int main()
{
    dls::PegasusOrchestrator plugin("dls_supervisor"); // same name of the library the CustomPlugin is compiled into
    plugin.execute();
	return 0;
}