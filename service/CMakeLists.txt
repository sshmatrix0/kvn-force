cmake_minimum_required(VERSION 3.19)
project(kvn_force_service LANGUAGES CXX)
if (WIN32)
    set(CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}\\installed\\x64-windows;C:/Qt/6.11.0/msvc2022_64")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake")
elseif(UNIX)
    set(CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/x64-linux;/home/ksp/Qt/6.9.0/gcc_64")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()
set(CMAKE_CXX_STANDARD 20)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)
find_package(cpptrace REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
qt_standard_project_setup()
qt6_add_resources(RESOURCES res.qrc)
qt_add_executable(KVNForceService
        src/main.cpp
        src/message_server/message_server.cpp
        src/message_server/message_server.h
        src/logger/logger.cpp
        src/logger/logger.h
        ${RESOURCES}
        src/utils/qthread/base_thread.cpp
        src/exceptions/abstract_exception.h
        src/exceptions/copy_config_exception.h
        src/exceptions/create_dir_exception.h
        src/exceptions/delete_dir_exception.h
        src/exceptions/json_format_exception.h
        src/exceptions/process_failed_exception.h
        src/exceptions/unsupported_protocol_exception.h
        src/exceptions/wrong_args_exception.h
        src/vpn/abstract_vpn.h
        src/servers/server_info.cpp
        src/servers/server_info.h
        src/utils/process_runner.cpp
        src/utils/process_runner.h
        src/utils/enum_util.h
        src/vpn/vpn_service.cpp
        src/vpn/vpn_service.h
        src/utils/http_client/http_client.cpp
        src/utils/http_client/http_client.h
        src/vpn/vpn_service_manager.cpp
        src/vpn/vpn_service_manager.h
        src/message_server/message_type.h
)
if (WIN32)
   set_target_properties(KVNForceService PROPERTIES
       LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
    )
endif()
# Define the source and destination directories
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
# Use CMAKE_CURRENT_BINARY_DIR to place the copy in the corresponding build directory
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")

# Add a custom target to perform the copy operation
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SOURCE_DIR}"
        "${DESTINATION_DIR}"
        COMMENT "Copying assets to build directory"
)
add_dependencies(KVNForceService copy_assets)
include_directories("./src")

target_link_libraries(KVNForceService
    PRIVATE
        Qt::Core
        Qt::Network
        cpptrace::cpptrace
        magic_enum::magic_enum
)

install(TARGETS KVNForceService
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

