cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(pyconfig_test2
	LANGUAGES CXX)

find_package(Python 3 REQUIRED COMPONENTS Development)
find_package(fmt REQUIRED)
find_package(Boost REQUIRED COMPONENTS python)

add_executable(pyconfig_test2
	main2.cpp
)
target_link_libraries(
  pyconfig_test2
  PRIVATE
  -Wl,--no-as-needed
  Python::Python
  Boost::python
  fmt::fmt
)

add_executable(pyconfig_test3
  main3.cpp
)
target_link_libraries(
  pyconfig_test3
  PRIVATE
  -Wl,--no-as-needed
  Python::Python
  Boost::python
  #fmt::fmt
)

add_executable(pyconfig_test4
  main4.cpp
)
target_link_libraries(
  pyconfig_test4
  PRIVATE
  -Wl,--no-as-needed
  Python::Python
  Boost::python
  fmt::fmt
)

add_executable(pyconfig_test5
  main5.cpp
)
target_link_libraries(
  pyconfig_test5
  PRIVATE
  -Wl,--no-as-needed
  Python::Python
  Boost::python
  fmt::fmt
)
set_target_properties(
  pyconfig_test5
  PROPERTIES
  CXX_STANDARD 26
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON
)

add_executable(pyconfig_test6
  main6.cpp
)
target_link_libraries(
  pyconfig_test6
  PRIVATE
  -Wl,--no-as-needed
  Python::Python
  Boost::python
  fmt::fmt
)
set_target_properties(
  pyconfig_test6
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
