add_executable(test_time time.cpp)

target_link_libraries(test_time vlelib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(devsttime test_time)
