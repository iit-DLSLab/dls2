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

#include "app_layer.hpp"
#include "dls2/hardware/hardware.hpp"
#include "dls2/components/app_data.hpp"
#include "dls2/util/messaging/dds_writer.hpp"

#include <pthread.h>

namespace dls
{
    class HardwareLayer : public AppLayer
    {
        typedef void * (*THREADFUNCPTR)(void *);

    public:
        /// Default Constructor
		///
        HardwareLayer(std::string);

        /// Default Destructor
		///
        ~HardwareLayer();

        Status run() override;

	    Status shutdown() override;

        /// Activates a hardware
        ///
        /// @ret true if the hardware exists, false otherwise.
        bool activateHardware(const std::string&);

        /// Deactivates a hardware
        ///
        /// @ret true if the hardware was running, false otherwise
        bool deactivateHardware(const std::string&);

		/// Print the state of this layer
		///
		std::string where() override{return "Base layer for hardwares";}
    
    private:
        // BEGIN critical section
		    std::map<std::string, std::shared_ptr<AppData>> hardwares;
		    std::mutex hardwares_mutex;
	    // END critical section

       	bool deactivateHardware(std::shared_ptr<AppData> pData);

        dls::DDSWriter *ddsMonitor;	
    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_LAYER_HPP_64INVL3T */
