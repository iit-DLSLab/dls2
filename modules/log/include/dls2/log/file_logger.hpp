#pragma once

#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>

namespace dls
{
	namespace logging
	{
		class FileLogger {
		public:
			explicit FileLogger(const std::string& path, bool append = false);

			bool is_open() const noexcept;

			void log(std::string_view line);

			void flush();
			void close();

		private:
			std::ofstream out_;
		};
	}
}
