// =============================================================================
// Includes
// =============================================================================
#include <memory>
#include <signal.h>
#include <iostream> // TODO temp

#include "application_framework/hyq_app.hpp"
#include "application_framework/hardware_layer.hpp"

// =============================================================================
// Forward Declarations
// =============================================================================

/// Pointer to application
std::shared_ptr<HyQApp> pApp;

void handle_args(int argc, char **argv);

// ============================== Signal Handling ==============================
struct sigaction sig_action;

/// Handler for segfaults during initialisiation
///
/// This handler is called before the application has been fully initialised
void initialisation_segfault_handler(int signum, siginfo_t *info, void *);

/// Handler for segfaults during application runtime
///
/// This handler is called if the application is initialised. It will call all
/// emergency stops and exit the program safely
void runtime_segfault_handler(int signum, siginfo_t *info, void *);

// =============================================================================
// Main Logic
// =============================================================================
int main(int argc, char **argv)
{
	// Runtime Configuration
	handle_args(argc, argv);

	// Setup Signal Handling
	sig_action.sa_sigaction = initialisation_segfault_handler;
	sig_action.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sig_action, nullptr);

	// Create application
	pApp = std::make_shared<HyQApp>();
	std::shared_ptr<HardwareLayer> pHardwareLayer = std::make_shared<HardwareLayer>();
	pApp->addLayer(pHardwareLayer);

	// Change Signal Handler to safely stop robot
	sig_action.sa_sigaction = runtime_segfault_handler;
	sig_action.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sig_action, nullptr);

	// Run the app
	pApp->run();

	// Force segfault TODO remove
	int *p = nullptr;
	*p = 1;
}

// =============================================================================
// Helper Functions
// =============================================================================
void handle_args(int argc, char **argv)
{
	// TODO something useful here
	argc += 1;
	char *a = argv[0];
	a = argv[1];
	a += 1;
}

// -----------------------------------------------------------------------------
// Signal Handler Functions
// -----------------------------------------------------------------------------
void initialisation_segfault_handler(int signum, siginfo_t *, void *)
{
	// TODO inspect info
	if(signum != SIGSEGV) return;
	std::cout << "Segfault during initialisation" << std::endl;

	exit(-1);
}

void runtime_segfault_handler(int signum, siginfo_t *, void *)
{
	// TODO inspect info
	if(signum != SIGSEGV) return;
	std::cout << "Segfault during runtime" << std::endl;
	pApp->panic();

	exit(-1);
}
