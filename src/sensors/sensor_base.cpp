// /*******************************************************************************
// *                                                       ,----,                 *
// *                                                     .'   .' \                *
// *                                                   ,----,'    |               *
// *               ________  ___       ________        |    :  .  ;               *
// *              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
// *              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
// *               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
// *                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
// *                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
// *                  \|_______|\|_______|\_________\./__;      :                 *
// *                                     \|_________||   :    .'                  *
// *                                                 ;   | .'                     *
// *                                                 `---'                        *
// *******************************************************************************/
// #include "dls2/sensors/sensor_base.hpp"

// using namespace dls;
// // =============================================================================
// // Constructors
// // =============================================================================
// SensorBase::SensorBase(Criticality c) :
// 	criticality(c),
// 	status(Status::UNINITIALISED),
// 	status_mutex()
// { }

// // =============================================================================
// // Member Functions
// // =============================================================================
// SensorBase::Criticality SensorBase::getCriticality() const
// {
// 	return this->criticality;
// }

// SensorBase::Status SensorBase::getStatus() const
// {
// 	std::lock_guard<std::mutex> lock(this->status_mutex);
// 	return this->status;
// }

// void SensorBase::setStatus(Status s)
// {
// 	std::lock_guard<std::mutex> lock(this->status_mutex);
// 	this->status = s;
// }
