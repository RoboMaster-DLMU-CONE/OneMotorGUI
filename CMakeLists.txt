cmake_minimum_required(VERSION 3.20)
project(OneMotorGUI VERSION 0.1.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FetchContent)

find_package(OneMotor QUIET)

if (NOT OneMotor_FOUND)
    message(STATUS "本地未找到 OneMotor 包，尝试从 GitHub 获取...")
    FetchContent_Declare(
            OneMotor_fetched
            GIT_REPOSITORY "https://github.com/RoboMaster-DLMU-CONE/OneMotor"
            GIT_TAG "main"
    )
    FetchContent_MakeAvailable(OneMotor_fetched)
else ()
    message(STATUS "已找到 OneMotor 版本 ${OneMotor_VERSION}")
endif ()

include(FindGTKMM)
include(FindLibnl3)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include ${LIBNL3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${GTKMM_LIBRARIES} ${LIBNL3_LIBRARIES} OneMotor::OneMotor)
