cmake_minimum_required(VERSION 3.19)
project(common_lib LANGUAGES CXX)
if (WIN32)
    set(CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}\\installed\\x64-windows;C:/Qt/6.11.0/msvc2022_64")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake")
elseif(UNIX)
    set(CMAKE_PREFIX_PATH "/opt/vcpkg/installed/x64-linux;/home/ksp/Qt/6.9.0/gcc_64")
    set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()
set(CMAKE_CXX_STANDARD 20)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)
find_package(cpptrace REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
qt_standard_project_setup()
include_directories("./src")

add_library(common_lib STATIC src/server_info/server_info.cpp
        src/server_info/server_info.h
        src/exceptions/abstract_exception.h
        src/exceptions/bad_url_format_exception.h
        src/exceptions/copy_config_exception.h
        src/exceptions/create_dir_exception.h
        src/exceptions/delete_dir_exception.h
        src/exceptions/json_format_exception.h
        src/exceptions/process_failed_exception.h
        src/exceptions/unsupported_protocol_exception.h
        src/exceptions/wrong_args_exception.h
        src/logger/logger.h
        src/logger/logger.cpp
        src/utils/process_runner.h
        src/utils/process_runner.cpp
        src/utils/enum_util.h
        src/utils/qthread/q_thread_exception.h
        src/utils/qthread/base_thread.h
        src/utils/qthread/base_thread.cpp
        src/utils/http_client/http_client.h
        src/utils/http_client/http_client.cpp
        src/vpn/abstract_vpn.h)

target_link_libraries(common_lib
    PRIVATE
        Qt::Core
        Qt::Network
        cpptrace::cpptrace
        magic_enum::magic_enum
)

target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

