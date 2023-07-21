
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

		std::atomic_int sample_count;
		std::function<void(void *)> callback;
        void *msg;

		void on_subscription_matched (
			eprosima::fastdds::dds::DataReader*,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus &info
		) override;

		void on_data_available ( eprosima::fastdds::dds::DataReader* ) override;

	};


	class DDSPartListener
	{
	public:

		// virtual void on_double_data_read(
		// 		const std::vector<types::NumericDatum>& numeric_data,
		// 		double timestamp)
		// {
		// 	DEBUG("Calling on_double_data_read");
		// 	static_cast<void>(numeric_data);
		// 	static_cast<void>(timestamp);
		// }

		// virtual void on_string_data_read(
		// 		const std::vector<types::TextDatum>& text_data,
		// 		double timestamp)
		// {
		// 	DEBUG("Calling on_string_data_read");
		// 	static_cast<void>(text_data);
		// 	static_cast<void>(timestamp);
		// }

		virtual void on_topic_discovery(
				const std::string& topic_name,
				const std::string& type_name)
		{
			static_cast<void>(topic_name);
			static_cast<void>(type_name);
		}

	};
	

} /// \endcond namespace dls
#endif /* end of include guard: DDSLISTENERS_HPP*/
