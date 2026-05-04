cmake_minimum_required(VERSION 3.16)
project(kvn_force VERSION 0.3.2 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)
if (WIN32)
    set(CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}\\installed\\x64-windows;C:/Qt/6.11.0/msvc2022_64")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake")
elseif(UNIX)
    set(CMAKE_PREFIX_PATH "/opt/vcpkg/installed/x64-linux;/home/ksp/Qt/6.9.0/gcc_64")
    set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()
add_subdirectory(../common ${CMAKE_BINARY_DIR}/common)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick Network Test)
find_package(cpptrace REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

# 1. Ищем Git на системе
find_package(Git QUIET)

# 2. Устанавливаем значение по умолчанию, если Git не найден
set(GIT_HASH "unknown")

# 3. Если Git найден, выполняем команду для получения хеша
if(GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
endif()

message(STATUS "Current Git Hash is: ${GIT_HASH}")


qt_standard_project_setup(REQUIRES 6.8)
qt6_add_resources(RESOURCES res.qrc)

qt_add_executable(KVNForce
        src/main.cpp
        src/system_tray/system_tray.cpp
        src/system_tray/system_tray.h
        ${RESOURCES}
        src/qml_register_manager.h
        src/qml_register_manager.h
        src/settings/settings.cpp
        src/settings/settings.h
        src/servers/servers_manager.cpp
        src/servers/servers_manager.h
        src/vpn/vpn_factory.cpp
        src/vpn/vpn_factory.h
        src/vpn/vpn_manager.h
        src/vpn/message_type.h
        src/vpn/vpn_desktop_service.cpp
        src/vpn/vpn_desktop_service.h
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
    @ONLY
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
include_directories("./src")
include_directories("../common/src")

qt_add_qml_module(KVNForce
        URI kvn_force
        QML_FILES
        src/gui_forms/Main.qml
        src/gui_forms/BottomPanel.qml
        src/gui_forms/Connect.qml
        src/gui_forms/Add.qml
        src/gui_forms/SettingsForm.qml
        src/gui_forms/ServerSettings.qml
        src/gui_forms/RoutingSettings.qml
        src/gui_forms/RoutingSettingsRS.qml
        src/gui_forms/ATextField.qml
        src/gui_forms/ASelectField.qml
        src/gui_forms/Theme.qml
        src/gui_forms/ServerPicker.qml
        src/gui_forms/DeleteDialog.qml
        src/gui_forms/Toast.qml
        src/gui_forms/About.qml
        src/gui_forms/qmldir
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(KVNForce PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.KVNForce
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(KVNForce
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Quick
        Qt::Network
        Qt::Test
        cpptrace::cpptrace
        magic_enum::magic_enum
        common_lib
)



install(TARGETS KVNForce
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Тесты (включаем поддержку CTest)
enable_testing()
add_subdirectory(test)
