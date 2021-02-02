#include <catch2/catch.hpp>

#include "dls2/application_framework/app_layer.hpp"
#include "dls2/application_framework/console_layer.hpp"
#include "dls2/application_framework/control_layer.hpp"
#include "dls2/application_framework/estimation_layer.hpp"
#include "dls2/application_framework/gait_generator_layer.hpp"
#include "dls2/application_framework/hardware_layer.hpp"
#include "dls2/application_framework/hyq_app.hpp"
#include "dls2/application_framework/init.hpp"
#include "dls2/application_framework/log_layer.hpp"
#include "dls2/application_framework/options.hpp"
#include "dls2/application_framework/version_info.hpp"

using namespace dls;

// These tests aren't testing for any specific functionality
// sometimes, when changing the version of fastdds, some things will break
// randomly. These tests just build various objects in the code base that may
// use dds functionality. If these tests segfault, it can give a quick
// indication of where things are going wrong in new versions
//
// These tests are therefore very coarse grained, aimed at only giving an
// initial direction for investigation.
TEST_CASE("Creating a layer works", "[layer]")
{
	dls::impl::initFramework();
	ConsoleLayer    layer1;
	ControlLayer    layer2;
	EstimationLayer layer3;
	// GaitLayer       layer4;
	HardwareLayer   layer5;
	LogLayer        layer6;
}

