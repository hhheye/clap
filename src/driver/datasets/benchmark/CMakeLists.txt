cmake_minimum_required(VERSION 2.8.3)
project(zzz_driver_datasets_benchmark)

find_package(catkin REQUIRED)
catkin_package()

catkin_python_setup()

catkin_install_python(PROGRAMS
  nodes/tracking
  nodes/decision
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
