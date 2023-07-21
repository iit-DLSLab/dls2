
#ifndef HARDWARE_DATA_HPP
#define HARDWARE_DATA_HPP

#include "dls2/application/app_data.hpp"

namespace dls
{
    class HardwareData : public AppData
    {
    public:
        HardwareData
        (
            const std::string& ID,
            const bool& is_real = false
        );

        const bool is_real;
    };
}// end namespace dls

#endif /* end of include guard: HARDWARE_DATA_HPP */