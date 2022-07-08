message(STATUS "## Reading dls_debian_settings.cmake")

set(CPACK_PACKAGE_VERSION_MAJOR                 ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR                 ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH                 ${PROJECT_VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_VERSION                ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR                        "dlslab")
set(CPACK_PACKAGE_CONTACT                       "dlslab")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER             "dlslab")
set(CPACK_VERBATIM_VARIABLES                    ON)
set(CPACK_GENERATOR                             "DEB")
set(CPACK_DEB_COMPONENT_INSTALL                 ON)

set(CPACK_DEBIAN_DLS_DEV_PACKAGE_NAME           "dls2-dev" CACHE INTERNAL "")
set(CPACK_DEBIAN_DLS_DEV_FILE_NAME              "dls2-dev-${PROJECT_VERSION}.deb" CACHE INTERNAL "")
set(CPACK_DEBIAN_DLS_DEV_PACKAGE_DEPENDS        "dls2-runtime, dls-messages-dev, robotlib-dev, dls-param-server-dev" CACHE INTERNAL "")

set(CPACK_DEBIAN_DLS_DOC_PACKAGE_NAME          "dls2-doc" CACHE INTERNAL "")
set(CPACK_DEBIAN_DLS_DOC_FILE_NAME             "dls2-doc-${PROJECT_VERSION}.deb" CACHE INTERNAL "") 
set(CPACK_DEBIAN_DLS_DOC_PACKAGE_DEPENDS       "" CACHE INTERNAL "")

set(CPACK_DEBIAN_DLS_RUNTIME_PACKAGE_NAME       "dls2-runtime" CACHE INTERNAL "")
set(CPACK_DEBIAN_DLS_RUNTIME_FILE_NAME          "dls2-runtime-${PROJECT_VERSION}.deb" CACHE INTERNAL "")
set(CPACK_DEBIAN_DLS_RUNTIME_PACKAGE_DEPENDS    "dls-messages, robotlib, dls-param-server" CACHE INTERNAL "")

get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
list(REMOVE_ITEM CPACK_COMPONENTS_ALL "Unspecified")

include(CPack)
