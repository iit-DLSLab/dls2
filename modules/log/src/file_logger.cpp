#include "dls2/log/file_logger.hpp"

using namespace dls::logging;

FileLogger::FileLogger(const std::string& path, bool append) {
	namespace fs = std::filesystem;

	const fs::path p{path};

	// Create parent directories if needed
	if (p.has_parent_path()) {
		fs::create_directories(p.parent_path());
	}

	std::ios::openmode mode = std::ios::out;
	mode |= append ? std::ios::app : std::ios::trunc;

	out_.open(p, mode);
}

bool FileLogger::is_open() const noexcept {
	return out_.is_open();
}

void FileLogger::log(std::string_view line) {
	if (!out_.is_open()){
		return;
	}

	out_ << line << '\n';
}

void FileLogger::flush() {
	if (out_.is_open()) {
		out_.flush();
	}
}

void FileLogger::close() {
	if (out_.is_open()) {
		out_.close();
	}
}
