#ifndef CLASS_LOADER_HPP_S3OYPT1Z
#define CLASS_LOADER_HPP_S3OYPT1Z

/// Utility class that simplifies loading shared objects into classes
class ClassLoader
{
public:
	template <class T>
	static std::shared_ptr<T> loadClass(const std::string &name);
};

#include "util/class_loader.tpp"

#endif /* end of include guard: CLASS_LOADER_HPP_S3OYPT1Z */
