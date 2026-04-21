file(GLOB SRC
     "../src/*/*.cpp"
     "../src/*/*.h"
     "../src/*/*/*.cpp"
     "../src/*/*/*.h"
     "../src/*/*/*/*.cpp"
     "../src/*/*/*/*.h"
     "../src/*/*/*/*/*.cpp"
     "../src/*/*/*/*/*.h"
     "../src/*.cpp"
     "../src/*.h"
)

FOREACH(item ${SRC})
  IF(${item} MATCHES "src/main.cpp")
    LIST(REMOVE_ITEM SRC ${item})
  ENDIF(${item} MATCHES "src/main.cpp")
ENDFOREACH(item)
add_executable(q_string_test q_string_test.cpp ${SRC})
target_link_libraries(q_string_test
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Quick
        Qt::Network
        Qt::Test
        cpptrace::cpptrace
        common_lib
)

add_executable(server_test server_test.cpp ${SRC})
target_link_libraries(server_test
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Quick
        Qt::Network
        Qt::Test
        cpptrace::cpptrace
        common_lib
)


add_test(NAME MyClassTest COMMAND q_string_test)
add_test(NAME ServerTest COMMAND server_test)