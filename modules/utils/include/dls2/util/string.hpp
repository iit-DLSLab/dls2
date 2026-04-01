#pragma once

#include <string>
#include <cstring>
#include <cctype>
#include <iostream>

namespace dls
{
    namespace utils
    {
        inline std::string splitSafe(const char* participant_name)
        {
            // Fast DDS participant_name is a fixed-size char buffer
            constexpr std::size_t MAX_NAME_LEN = 256;

            // Safely construct string
            std::string name(
                participant_name,
                strnlen(participant_name, MAX_NAME_LEN)
            );

            // Cut at first "::" if present
            const auto pos = name.find("::");
            if (pos != std::string::npos)
            {
                name.resize(pos);
            }

            // Trim trailing whitespace / padding
            while (!name.empty() &&
                (name.back() == '\0' ||
                    std::isspace(static_cast<unsigned char>(name.back()))))
            {
                name.pop_back();
            }

            return name;
        }

    }// namespace utils

} // namespace dls