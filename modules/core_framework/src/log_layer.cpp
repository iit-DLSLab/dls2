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
#ifndef LOG_LAYER_CPP_DLJLOFSG
#define LOG_LAYER_CPP_DLJLOFSG

#include "dls2/core_framework/log_layer.hpp"
#include <sys/time.h>
#include <cstdio>

using namespace dls;

LogLayer::LogLayer(std::string ID)
	: AppLayer(ID)
	, ddslink(
		"log_layer",
		dls::domains::logging
	)
{
	//debug_log
	ddslink.addReader(
		"debug_log",
		dls::topics::debug_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": DEBUG: " <<  msg->msg() << std::flush;
			}
		}
	);

	//info_log
	ddslink.addReader(
		"info_log",
		dls::topics::info_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": INFO: " <<  msg->msg() << std::flush;
			}
		}
	);

	//warn_log
	ddslink.addReader(
		"warn_log",
		dls::topics::warn_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": WARN: " <<  msg->msg() << std::flush;
			}
		}
	);

	//error_log
	ddslink.addReader(
		"error_log",
		dls::topics::error_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": ERROR: " <<  msg->msg() << std::flush;
			}
		}
	);

	//fatal_log
	ddslink.addReader(
		"fatal_log",
		dls::topics::fatal_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": FATAL: " <<  msg->msg() << std::flush;
			}
		}
	);


	//hyq_raw_log
	ddslink.addReader(
		"hyq_raw_log",
		dls::topics::low_level_estimation::hyqreal_raw,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				HyQRealRawMsg *msg = (HyQRealRawMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": RAW: " <<
						msg->lf().haa().actual_position() << " " <<
						msg->lf().haa().actual_force() << " " <<
						msg->lf().haa().calc_velocity() << " " <<
						msg->lf().haa().pressure_1() << " " <<
						msg->lf().haa().pressure_2() << " " <<
						msg->lf().haa().pressure_3() << " " <<
						msg->lf().haa().pressure_4() << " " <<
						msg->lf().haa().spool_position() << " " <<
						msg->lf().haa().force_command() << " " <<
						msg->lf().haa().spool_command() << " " <<
						msg->lf().haa().error_word() << " " <<
						msg->lf().haa().system_temp() << " " <<
						msg->lf().haa().spare_ptr1() << " " <<
						msg->lf().haa().spare_ptr2() << " " <<
						msg->lf().haa().status_word() << " " <<

						msg->lf().hfe().actual_position() << " " <<
						msg->lf().hfe().actual_force() << " " <<
						msg->lf().hfe().calc_velocity() << " " <<
						msg->lf().hfe().pressure_1() << " " <<
						msg->lf().hfe().pressure_2() << " " <<
						msg->lf().hfe().pressure_3() << " " <<
						msg->lf().hfe().pressure_4() << " " <<
						msg->lf().hfe().spool_position() << " " <<
						msg->lf().hfe().force_command() << " " <<
						msg->lf().hfe().spool_command() << " " <<
						msg->lf().hfe().error_word() << " " <<
						msg->lf().hfe().system_temp() << " " <<
						msg->lf().hfe().spare_ptr1() << " " <<
						msg->lf().hfe().spare_ptr2() << " " <<
						msg->lf().hfe().status_word() << " " <<

						msg->lf().kfe().actual_position() << " " <<
						msg->lf().kfe().actual_force() << " " <<
						msg->lf().kfe().calc_velocity() << " " <<
						msg->lf().kfe().pressure_1() << " " <<
						msg->lf().kfe().pressure_2() << " " <<
						msg->lf().kfe().pressure_3() << " " <<
						msg->lf().kfe().pressure_4() << " " <<
						msg->lf().kfe().spool_position() << " " <<
						msg->lf().kfe().force_command() << " " <<
						msg->lf().kfe().spool_command() << " " <<
						msg->lf().kfe().error_word() << " " <<
						msg->lf().kfe().system_temp() << " " <<
						msg->lf().kfe().spare_ptr1() << " " <<
						msg->lf().kfe().spare_ptr2() << " " <<
						msg->lf().kfe().status_word() << " " <<

						msg->lh().haa().actual_position() << " " <<
						msg->lh().haa().actual_force() << " " <<
						msg->lh().haa().calc_velocity() << " " <<
						msg->lh().haa().pressure_1() << " " <<
						msg->lh().haa().pressure_2() << " " <<
						msg->lh().haa().pressure_3() << " " <<
						msg->lh().haa().pressure_4() << " " <<
						msg->lh().haa().spool_position() << " " <<
						msg->lh().haa().force_command() << " " <<
						msg->lh().haa().spool_command() << " " <<
						msg->lh().haa().error_word() << " " <<
						msg->lh().haa().system_temp() << " " <<
						msg->lh().haa().spare_ptr1() << " " <<
						msg->lh().haa().spare_ptr2() << " " <<
						msg->lh().haa().status_word() << " " <<

						msg->lh().hfe().actual_position() << " " <<
						msg->lh().hfe().actual_force() << " " <<
						msg->lh().hfe().calc_velocity() << " " <<
						msg->lh().hfe().pressure_1() << " " <<
						msg->lh().hfe().pressure_2() << " " <<
						msg->lh().hfe().pressure_3() << " " <<
						msg->lh().hfe().pressure_4() << " " <<
						msg->lh().hfe().spool_position() << " " <<
						msg->lh().hfe().force_command() << " " <<
						msg->lh().hfe().spool_command() << " " <<
						msg->lh().hfe().error_word() << " " <<
						msg->lh().hfe().system_temp() << " " <<
						msg->lh().hfe().spare_ptr1() << " " <<
						msg->lh().hfe().spare_ptr2() << " " <<
						msg->lh().hfe().status_word() << " " <<

						msg->lh().kfe().actual_position() << " " <<
						msg->lh().kfe().actual_force() << " " <<
						msg->lh().kfe().calc_velocity() << " " <<
						msg->lh().kfe().pressure_1() << " " <<
						msg->lh().kfe().pressure_2() << " " <<
						msg->lh().kfe().pressure_3() << " " <<
						msg->lh().kfe().pressure_4() << " " <<
						msg->lh().kfe().spool_position() << " " <<
						msg->lh().kfe().force_command() << " " <<
						msg->lh().kfe().spool_command() << " " <<
						msg->lh().kfe().error_word() << " " <<
						msg->lh().kfe().system_temp() << " " <<
						msg->lh().kfe().spare_ptr1() << " " <<
						msg->lh().kfe().spare_ptr2() << " " <<
						msg->lh().kfe().status_word() << " " << std::endl;
			}
		}
	);
}

LogLayer::Status LogLayer::run()
{
	while(!this->should_quit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return this->getStatus();
}

LogLayer::Status LogLayer::shutdown()
{
	this->should_quit = true;
	return getStatus();
}

std::string LogLayer::get_current_time()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[256];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}
#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
