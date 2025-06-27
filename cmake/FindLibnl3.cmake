find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBNL3 REQUIRED libnl-3.0)

if (NOT LIBNL3_FOUND)
    message(FATAL_ERROR "libnl-3.0 not found!")
endif ()

message(STATUS "Found libnl-3.0 include dirs: ${LIBNL3_INCLUDE_DIRS}")
message(STATUS "Found libnl-3.0 libraries: ${LIBNL3_LIBRARIES}")
