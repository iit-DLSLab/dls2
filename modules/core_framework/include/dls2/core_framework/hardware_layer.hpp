/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef HARDWARE_LAYER_HPP_64INVL3T
#define HARDWARE_LAYER_HPP_64INVL3T

#include "dls2/core/app_layer.hpp"
#include "dls2/hardware/hardware.hpp"
#include "dls2/hardware/hardware_data.hpp"
#include "dls2/util/messaging/dds_writer.hpp"

#include <pthread.h>

namespace dls
{
    class HardwareLayer : public AppLayer
    {
    public:
        /// Default Constructor
		///
        HardwareLayer(std::string);

        /// Default Destructor
		///
        ~HardwareLayer();

        /// Run the layer
		///
        Status run() override;

        /// Stop the layer
		///
	    Status stop() override;

        /// Activates a hardware
        ///
        /// @ret true if the hardware exists, false otherwise.
        bool loadHardware(const std::string&);

        /// Deactivates a hardware
        ///
        /// @ret true if the hardware was running, false otherwise
        bool unloadHardware(const std::string&);

		/// Print the state of this layer
		///
		std::string where() override{return "Base layer for hardwares";}
    
    private:
        // BEGIN critical section
		    std::map<std::string, std::shared_ptr<AppData>> hardwares;
		    std::mutex hardwares_mutex;
	    // END critical section

       	bool unloadHardware(std::shared_ptr<AppData> pData);
    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_LAYER_HPP_64INVL3T */
