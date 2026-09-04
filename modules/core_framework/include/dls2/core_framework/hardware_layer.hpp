
#ifndef HARDWARE_LAYER_HPP_64INVL3T
#define HARDWARE_LAYER_HPP_64INVL3T

#include "dls2/application/layer.hpp"
#include "dls2/hardware/hardware.hpp"
#include "dls2/hardware/hardware_data.hpp"
#include "dls2/util/messaging/dds_writer.hpp"

#include <pthread.h>

namespace dls
{
    class HardwareLayer : public Layer
    {
    public:
        /// Default Constructor
		///
        HardwareLayer(std::string);

        /// Default Destructor
		///
        ~HardwareLayer();

        /// Stop the layer
		///
	    void close() override;

        /// Activates a hardware
        ///
        /// @ret true if the hardware exists, false otherwise.
        bool loadHardware(const std::string&, const std::string& child_process_name = "${DLS_INSTALL_RUNTIME_DIR}/child_process_launcher");

        /// Deactivates a hardware
        ///
        /// @ret true if the hardware was running, false otherwise
        bool unloadHardware(const std::string&);

		/// Print the state of this layer
		///
		std::string where() override{return "Base layer for hardwares";}

		void monitor() override;
    
    private:
        // BEGIN critical section
		    std::map<std::string, std::shared_ptr<AppData>> hardwares;
		    std::mutex hardwares_mutex;
	    // END critical section

       	bool unloadHardware(std::shared_ptr<AppData> pData);
    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_LAYER_HPP_64INVL3T */
