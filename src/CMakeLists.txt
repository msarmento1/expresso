file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${expresso_SOURCE_DIR}/include/expresso/*.hpp")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/src)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/src)

conan_cmake_configure(REQUIRES
                      fmt/6.1.2
                      boost/1.80.0
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

find_package(fmt)
find_package(Boost)

add_library(expresso application.cpp connection.cpp ${HEADER_LIST})
target_include_directories(expresso PUBLIC ../include)
target_link_libraries(expresso fmt::fmt Boost::boost)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
