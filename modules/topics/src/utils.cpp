#include "dls2/topics/utils.hpp"

// Dynamic types
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/xmlparser/XMLProfileManager.h>

namespace dls{
    eprosima::fastdds::dds::TypeSupport createDynamicTypeSupport(const std::string& xml_file_path, const std::string& type_name){
        if (eprosima::fastrtps::xmlparser::XMLP_ret::XML_OK !=
        eprosima::fastrtps::xmlparser::XMLProfileManager::loadXMLFile(xml_file_path))
        {
            std::cout <<
                "Cannot open XML file \""+xml_file_path+"\". Please, run the publisher from the folder "
                    << "that contatins this XML file." << std::endl;
            return eprosima::fastdds::dds::TypeSupport();
        }
        // -- create dynamic type
        eprosima::fastrtps::types::DynamicType_ptr dyn_type = eprosima::fastrtps::xmlparser::XMLProfileManager::getDynamicTypeByName(type_name)->build();
        return eprosima::fastdds::dds::TypeSupport(new eprosima::fastrtps::types::DynamicPubSubType(dyn_type));
    }

	eprosima::fastrtps::types::DynamicData_ptr createDynamicMessage(const eprosima::fastdds::dds::TypeSupport type_support){
        eprosima::fastrtps::types::DynamicType_ptr dyn_type = eprosima::fastrtps::xmlparser::XMLProfileManager::getDynamicTypeByName(type_support.get_type_name())->build();
        return eprosima::fastrtps::types::DynamicData_ptr(eprosima::fastrtps::types::DynamicDataFactory::get_instance()->create_data(dyn_type));
    }
}