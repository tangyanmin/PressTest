#include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(helloword press_test.cpp client.cpp)
#add_executable(helloword press_test.cpp server.cpp)
target_link_libraries(helloword BoostParts pthread rt ssl crypto)
