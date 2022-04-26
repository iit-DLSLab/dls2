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
#ifndef DDSLISTENERS_HPP
#define DDSLISTENERS_HPP

#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <string>

/// \cond doxygen_namespace_dls
namespace dls
{
	// =====================================================================
	// Publisher Helper Listener Class
	// =====================================================================
	class DDSPubListener : public eprosima::fastdds::dds::DataWriterListener
	{

	public:

		DDSPubListener();

		void on_publication_matched
		(
			eprosima::fastdds::dds::DataWriter 				 	   *writer,
			const eprosima::fastdds::dds::PublicationMatchedStatus &info
		) override;

		std::atomic_int matched_count;
	};

	// =====================================================================
	// Subscriber Helper Listener Class
	// =====================================================================
	class DDSSubListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		DDSSubListener(std::function<void(void *)> callback_);

		~DDSSubListener();

		void *msg;
		
		std::atomic_int sample_count;
		std::function<void(void *)> callback;

		void on_subscription_matched (
			eprosima::fastdds::dds::DataReader*,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus &info
		) override;

		void on_data_available ( eprosima::fastdds::dds::DataReader* ) override;

	};
	

} /// \endcond namespace dls
#endif /* end of include guard: DDSLISTENERS_HPP*/
