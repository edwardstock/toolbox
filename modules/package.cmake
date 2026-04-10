include(CMakePackageConfigHelpers)

# ── pkg-config ────────────────────────────────────────────────────────────────
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cfg/${PROJECT_NAME}.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/${PROJECT_NAME}.pc"
    @ONLY
)

# ── CMake package config ──────────────────────────────────────────────────────
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cfg/${PROJECT_NAME}-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    VERSION       ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# ── Install ───────────────────────────────────────────────────────────────────
install(
    TARGETS  ${PROJECT_NAME}
    EXPORT   ${PROJECT_NAME}-targets
    RUNTIME  DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Generated header lives in the build tree
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/include/toolbox/toolbox_config.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/toolbox"
)

install(
    EXPORT    ${PROJECT_NAME}-targets
    NAMESPACE ${PROJECT_NAME}::
    FILE      "${PROJECT_NAME}-targets.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

# ── CPack ─────────────────────────────────────────────────────────────────────
set(CPACK_PACKAGE_NAME              ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION           ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR            "Eduard Maximovich")
set(CPACK_PACKAGE_CONTACT           "edward.vstock@gmail.com")
set(CPACK_PACKAGE_HOMEPAGE_URL      "https://github.com/edwardstock/toolbox")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_RESOURCE_FILE_LICENSE     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DESCRIPTION_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_IGNORE_FILES       [[\.git/]] [[_build/]] [[cmake-build-.*/]] [[build/]])

set(CPACK_GENERATOR     "TGZ;ZIP")
set(CPACK_PACKAGE_FILE_NAME
    "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)
