
// =============================================================================
// Includes
// =============================================================================
#include <string>

// =============================================================================
// class Interface
// =============================================================================
namespace dls
{
/// static class representing the version of the framework
///
/// This class makes it possilbe to change the version number without having to
/// recompile everything
class VersionInfo
{
public:
	static std::string getVersionString();
	static int getVersionMajor();
	static int getVersionMinor();
	static int getVersionPatch();
};
} // end namespace dls
