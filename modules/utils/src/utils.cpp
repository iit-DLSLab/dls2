
#include "dls2/util/utils.hpp"

namespace dls{
	namespace utils{
		
		bool waitFunction(std::function<bool()> func, int timeout_ms, int period_ms, bool& stop_wait){
			// Wait for timeout seconds the input readyness
			auto start = std::chrono::high_resolution_clock::now();
			auto end = start;
			while(!stop_wait && std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < timeout_ms)
			{
				if(func()){
					return true;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
				end = std::chrono::high_resolution_clock::now();
			}
			return false;
		}
	}
}