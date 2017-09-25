add_test(NAME hash_structors COMMAND ${PROJECT_TEST_NAME} -t hash_structors)
add_test(NAME hash_makeHash COMMAND ${PROJECT_TEST_NAME} -t hash_makeHash)
add_test(NAME hash_compare COMMAND ${PROJECT_TEST_NAME} -t hash_compare)

add_test(NAME hash_tree_constructors COMMAND ${PROJECT_TEST_NAME} -t hash_tree_constructors)
add_test(NAME hash_tree_empty COMMAND ${PROJECT_TEST_NAME} -t hash_tree_empty)
add_test(NAME hash_tree_size_compare COMMAND ${PROJECT_TEST_NAME} -t hash_tree_size_compare)
# add_test(NAME hash_tree_size_compare_random COMMAND ${PROJECT_TEST_NAME} -t hash_tree_size_compare_random)
add_test(NAME hash_tree_change COMMAND ${PROJECT_TEST_NAME} -t hash_tree_change)
add_test(NAME hash_tree_changed_hashes COMMAND ${PROJECT_TEST_NAME} -t hash_tree_changed_hashes)
add_test(NAME hash_tree_sort COMMAND ${PROJECT_TEST_NAME} -t hash_tree_sort)
add_test(NAME hash_tree_top_hash COMMAND ${PROJECT_TEST_NAME} -t hash_tree_top_hash)
add_test(NAME hash_tree_elements_per_level COMMAND ${PROJECT_TEST_NAME} -t hash_tree_elements_per_level)

add_test(NAME directory_constructors COMMAND ${PROJECT_TEST_NAME} -t directory_constructors)
add_test(NAME directory_gethashtree COMMAND ${PROJECT_TEST_NAME} -t directory_gethashtree)
add_test(NAME directory_compare COMMAND ${PROJECT_TEST_NAME} -t directory_compare)

# add_test(NAME box_test COMMAND ${PROJECT_TEST_NAME} -t box_test)
#add_test(NAME box_compare COMMAND ${PROJECT_TEST_NAME} -t box_compare)


include_directories(../include)
add_executable(${PROJECT_TEST_NAME} testMain.cpp
                           #../src/constants.cpp
                           ../src/hash.cpp
                           ../src/hash_tree.cpp
                           ../src/directory.cpp
                           #../src/transmitter.cpp
                           #../src/box.cpp
                           #../src/boxconfig.cpp
                           test_hash.cpp
                           test_hash_tree.cpp
                           test_directory.cpp
                           #test_box.cpp
                           )
target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT}
                                  ${Boost_LIBRARIES}
                                  ${LIBSODIUM_LIBRARY}
                                  ${ZEROMQ_LIBRARY}
                                  ${ZEROMQ_CPP_LIBRARY}
                                  ${JSONCPP_LIBRARY})