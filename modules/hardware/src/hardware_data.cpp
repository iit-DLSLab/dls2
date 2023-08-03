
#ifndef HARDWARE_DATA_CPP
#define HARDWARE_DATA_CPP

#include "dls2/hardware/hardware_data.hpp"

using namespace dls;

HardwareData::HardwareData
(
	const std::string& ID,
	const bool& is_real_
) 
	: AppData(ID)
	, is_real(is_real_)
{ }

#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */