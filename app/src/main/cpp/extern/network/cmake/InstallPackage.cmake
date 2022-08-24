#
# Function call definition:
# install_package(
#   PACKAGE <package>
#   TARGETS <targets>
#   INCLUDE_DIRS [include_dirs...]
# )
#
# Input file requirements:
#   1. ${CMAKE_CURRENT_SOURCE_DIR}/cmake/<package>Config.cmake.in
#
# ---------------------------------------------------------------
#                           DESCRIPTION
# ---------------------------------------------------------------
# This helper function creates rules for exporting and installing 
# targets to a package so that an external project can import 
# these targets as a dependecy by using the simple command after 
# installing this project to the system: 
# find_package(<package> [version])
#
# The project can then link to a target simply by calling:
# target_link_libraries(<external_proj_target> 
#    [PRIVATE|PUBLIC|INTERFACE] <package>::<targets>
# )
#        
# ---------------------------------------------------------------
#                       INPUT REQUIREMENTS
# ---------------------------------------------------------------
# 1. ${CMAKE_CURRENT_SOURCE_DIR}/cmake/<package>Config.cmake.in
# This file is used to import dependencies of this target (if any).
# This file must at a minimum require the following line:
# include("${CMAKE_CURRENT_LIST_DIR}/@install_package_PACKAGE@Targets.cmake")
#
# 2. The PROJECT_VERSION variable must be set.
#
# 3. If [include_dirs...] is given as a relative path, they are
# interpreted relative to the ${CMAKE_CURRENT_SOURCE_DIR}. 
#
include(CMakePackageConfigHelpers)

function(install_package)
    set(oneValueArgs PACKAGE) 
    set(multipleValueArgs TARGETS INCLUDE_DIRS)
    cmake_parse_arguments(install_package 
        "" "${oneValueArgs}" "${multipleValueArgs}" "${ARGV}"
    )

    # Package version
    write_basic_package_version_file(
        ${install_package_PACKAGE}ConfigVersion.cmake
        COMPATIBILITY AnyNewerVersion
    )

    # Include directories
    install(DIRECTORY ${install_package_INCLUDE_DIRS} DESTINATION include)

    # Target export
    install(TARGETS ${install_package_TARGETS}
        EXPORT ${install_package_PACKAGE}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(EXPORT ${install_package_PACKAGE}Targets
        NAMESPACE ${install_package_PACKAGE}::
        DESTINATION "lib/cmake/${install_package_PACKAGE}"
    )

    # Config files
    configure_file(
        "cmake/InstallPackageConfig.cmake.in" 
        ${install_package_PACKAGE}Config.cmake @ONLY
    )

    install(
        FILES 
            "${CMAKE_CURRENT_BINARY_DIR}/${install_package_PACKAGE}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${install_package_PACKAGE}ConfigVersion.cmake"
        DESTINATION "lib/cmake/${install_package_PACKAGE}"
    )

    # Generate uninstall target
    configure_file(
        "cmake/UninstallPackage.cmake.in"
        "Uninstall${install_package_PACKAGE}.cmake" @ONLY
    )


    add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/Uninstall${install_package_PACKAGE}.cmake"
    )
endfunction()
