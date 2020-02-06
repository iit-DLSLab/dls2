# ==============================================================================
# Debian Packaging Settings
# ==============================================================================
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "dlslab")
set(CPACK_PACKAGE_CONTACT "dlslab")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dlslab")
set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PATCH})
include(CPack)
