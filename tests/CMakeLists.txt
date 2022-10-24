file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${expresso_SOURCE_DIR}/include/expresso/*.hpp")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/tests)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/tests)

conan_cmake_configure(REQUIRES
                      catch2/3.1.0
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

find_package(Catch2)

add_executable(expresso_test application_test.cpp ${HEADER_LIST})
target_include_directories(expresso_test PUBLIC ../include)
target_link_libraries(expresso_test Catch2::Catch2 expresso)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
