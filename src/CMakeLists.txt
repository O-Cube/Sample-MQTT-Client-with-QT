cmake_minimum_required(VERSION 3.14)

project(MQTT_Network LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Widgets Mqtt Gui Widgets Sql)

add_executable(MQTT_Network
  main.cpp
  mqttdinterface.h mqttdinterface.cpp
  uiclient.ui
  mymqttclient.h mymqttclient.cpp
  databasemanagement.h databasemanagement.cpp
  mythread.h mythread.cpp

)
target_link_libraries(MQTT_Network Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(MQTT_Network Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(MQTT_Network Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MQTT_Network Qt${QT_VERSION_MAJOR}::Mqtt)
target_link_libraries(MQTT_Network Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(MQTT_Network Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MQTT_Network Qt${QT_VERSION_MAJOR}::Sql)

include(GNUInstallDirs)
install(TARGETS MQTT_Network
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
