// Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// This file is part of eProsima Fast DDS Visualizer Plugin.
//
// eProsima Fast DDS Visualizer Plugin is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// eProsima Fast DDS Visualizer Plugin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with eProsima Fast DDS Visualizer Plugin. If not, see <https://www.gnu.org/licenses/>.

/**
 * @file utils.cpp
 */

#include <filesystem>
#include <sstream>

#include "dls2/web_socket_translator/utils.hpp"

namespace dls {

std::string get_timestamp_string(
        const eprosima::fastrtps::rtps::Time_t& timestamp)
{
    std::stringstream ss;
    ss << timestamp;
    return ss.str();
}

double get_timestamp_seconds_numeric_value(
        const eprosima::fastrtps::rtps::Time_t& timestamp)
{
    return (timestamp.seconds() + (timestamp.nanosec() * 1e-9));
}

std::string to_string(
        const std::wstring& str)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(str);
}

std::string to_string(
        char c)
{
    return std::string(1, c);
}

std::string to_string(
        wchar_t c)
{
    return to_string(std::wstring(1, c));
}

std::vector<std::string> get_files_in_dir(
        const std::string& dir_path,
        const std::string& file_extension /* = ".xml" */)
{
    return get_files_in_dir_regex(
        dir_path,
        std::regex(".*\\." + file_extension + "$"));
}

std::vector<std::string> get_files_in_dir_regex(
        const std::string& dir_path,
        const std::regex& regex_rule)
{
    std::vector<std::string> result;

    // Get all files in dir
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        const auto& file_path = entry.path().string();

        // For each file found, check it follows the pattern
        bool match = std::regex_match(file_path, regex_rule);
        if (match)
        {
            // If it matches, add it to the list
            result.push_back(file_path);
        }
    }

    return result;
}

} /* namespace dls */
