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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef LOG_LAYER_HPP_IAHZ5BZG
#define LOG_LAYER_HPP_IAHZ5BZG

#include "dls2/application_framework/app_layer.hpp"
#include "dls2/util/messaging/subscriber_base.hpp"
#include "dls2/msg/stringmsgPubSubTypes.h"
#include "dls2/msg/hyqreal_rawPubSubTypes.h"

#include "dls2/topics/hyqreal_raw.hpp"
#include "dls2/topics/debug_log_stream.hpp"
#include "dls2/topics/error_log_stream.hpp"
#include "dls2/topics/fatal_log_stream.hpp"
#include "dls2/topics/info_log_stream.hpp"
#include "dls2/topics/warn_log_stream.hpp"
#include "dls2/util/debug/debug.hpp"

#include <mutex>
#include <condition_variable>

/// THIS class is in a very hackish copy-paste state at the moment. It will be
/// made better
namespace dls
{
class LogLayer : public AppLayer
{
public:
	LogLayer();

	Status run() override;
	Status shutdown() override;

private:
	static std::string get_current_time();
	class DebugListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		DebugListener() :
			SubscriberBase<StringMsgPubSubType>(topics::debug_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << LogLayer::get_current_time() << ": DEBUG: " <<  msg.msg() << std::flush;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} debug_listener;
	class InfoListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		InfoListener() :
			SubscriberBase<StringMsgPubSubType>(topics::info_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << LogLayer::get_current_time() << ": INFO: " <<  msg.msg() << std::flush;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} info_listener;
	class WarnListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		WarnListener() :
			SubscriberBase<StringMsgPubSubType>(topics::warn_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << LogLayer::get_current_time() << ": WARN: " <<  msg.msg() << std::flush;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} warn_listener;
	class ErrorListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		ErrorListener() :
			SubscriberBase<StringMsgPubSubType>(topics::error_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << LogLayer::get_current_time() << ": ERROR: " <<  msg.msg() << std::flush;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} error_listener;
	class FatalListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		FatalListener() :
			SubscriberBase<StringMsgPubSubType>(topics::fatal_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << LogLayer::get_current_time() << ": FATAL: " <<  msg.msg() << std::flush;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} fatal_listener;

	class RawListener : public SubscriberBase<HyQRealRawMsgPubSubType>
	{
	public:
		RawListener() :
			SubscriberBase<HyQRealRawMsgPubSubType>(topics::hyqreal_raw),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			HyQRealRawMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				// TODO this is just a temporary messy quick implementation.
				// This whole layer needs to be written better in any case
				// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << LogLayer::get_current_time() << ": RAW: " <<
						msg.lf().haa().actual_position() << " " <<
						msg.lf().haa().actual_force() << " " <<
						msg.lf().haa().calc_velocity() << " " <<
						msg.lf().haa().pressure_1() << " " <<
						msg.lf().haa().pressure_2() << " " <<
						msg.lf().haa().pressure_3() << " " <<
						msg.lf().haa().pressure_4() << " " <<
						msg.lf().haa().spool_position() << " " <<
						msg.lf().haa().force_command() << " " <<
						msg.lf().haa().spool_command() << " " <<
						msg.lf().haa().error_word() << " " <<
						msg.lf().haa().system_temp() << " " <<
						msg.lf().haa().spare_ptr1() << " " <<
						msg.lf().haa().spare_ptr2() << " " <<
						msg.lf().haa().status_word() << " " <<

						msg.lf().hfe().actual_position() << " " <<
						msg.lf().hfe().actual_force() << " " <<
						msg.lf().hfe().calc_velocity() << " " <<
						msg.lf().hfe().pressure_1() << " " <<
						msg.lf().hfe().pressure_2() << " " <<
						msg.lf().hfe().pressure_3() << " " <<
						msg.lf().hfe().pressure_4() << " " <<
						msg.lf().hfe().spool_position() << " " <<
						msg.lf().hfe().force_command() << " " <<
						msg.lf().hfe().spool_command() << " " <<
						msg.lf().hfe().error_word() << " " <<
						msg.lf().hfe().system_temp() << " " <<
						msg.lf().hfe().spare_ptr1() << " " <<
						msg.lf().hfe().spare_ptr2() << " " <<
						msg.lf().hfe().status_word() << " " <<

						msg.lf().kfe().actual_position() << " " <<
						msg.lf().kfe().actual_force() << " " <<
						msg.lf().kfe().calc_velocity() << " " <<
						msg.lf().kfe().pressure_1() << " " <<
						msg.lf().kfe().pressure_2() << " " <<
						msg.lf().kfe().pressure_3() << " " <<
						msg.lf().kfe().pressure_4() << " " <<
						msg.lf().kfe().spool_position() << " " <<
						msg.lf().kfe().force_command() << " " <<
						msg.lf().kfe().spool_command() << " " <<
						msg.lf().kfe().error_word() << " " <<
						msg.lf().kfe().system_temp() << " " <<
						msg.lf().kfe().spare_ptr1() << " " <<
						msg.lf().kfe().spare_ptr2() << " " <<
						msg.lf().kfe().status_word() << " " <<

						msg.lh().haa().actual_position() << " " <<
						msg.lh().haa().actual_force() << " " <<
						msg.lh().haa().calc_velocity() << " " <<
						msg.lh().haa().pressure_1() << " " <<
						msg.lh().haa().pressure_2() << " " <<
						msg.lh().haa().pressure_3() << " " <<
						msg.lh().haa().pressure_4() << " " <<
						msg.lh().haa().spool_position() << " " <<
						msg.lh().haa().force_command() << " " <<
						msg.lh().haa().spool_command() << " " <<
						msg.lh().haa().error_word() << " " <<
						msg.lh().haa().system_temp() << " " <<
						msg.lh().haa().spare_ptr1() << " " <<
						msg.lh().haa().spare_ptr2() << " " <<
						msg.lh().haa().status_word() << " " <<

						msg.lh().hfe().actual_position() << " " <<
						msg.lh().hfe().actual_force() << " " <<
						msg.lh().hfe().calc_velocity() << " " <<
						msg.lh().hfe().pressure_1() << " " <<
						msg.lh().hfe().pressure_2() << " " <<
						msg.lh().hfe().pressure_3() << " " <<
						msg.lh().hfe().pressure_4() << " " <<
						msg.lh().hfe().spool_position() << " " <<
						msg.lh().hfe().force_command() << " " <<
						msg.lh().hfe().spool_command() << " " <<
						msg.lh().hfe().error_word() << " " <<
						msg.lh().hfe().system_temp() << " " <<
						msg.lh().hfe().spare_ptr1() << " " <<
						msg.lh().hfe().spare_ptr2() << " " <<
						msg.lh().hfe().status_word() << " " <<

						msg.lh().kfe().actual_position() << " " <<
						msg.lh().kfe().actual_force() << " " <<
						msg.lh().kfe().calc_velocity() << " " <<
						msg.lh().kfe().pressure_1() << " " <<
						msg.lh().kfe().pressure_2() << " " <<
						msg.lh().kfe().pressure_3() << " " <<
						msg.lh().kfe().pressure_4() << " " <<
						msg.lh().kfe().spool_position() << " " <<
						msg.lh().kfe().force_command() << " " <<
						msg.lh().kfe().spool_command() << " " <<
						msg.lh().kfe().error_word() << " " <<
						msg.lh().kfe().system_temp() << " " <<
						msg.lh().kfe().spare_ptr1() << " " <<
						msg.lh().kfe().spare_ptr2() << " " <<
						msg.lh().kfe().status_word() << " " << std::endl;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} raw_listener;

	// BEGIN critical section
		bool                     should_quit;
		std::condition_variable  should_quit_cv;
		std::mutex               should_quit_mutex;
	// END critical section
};
} // end namespace dls

#endif /* end of include guard: LOG_LAYER_HPP_IAHZ5BZG */
