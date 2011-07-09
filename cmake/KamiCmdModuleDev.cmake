
macro(KamiCmdModuleDev name)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB UI ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
FILE(GLOB RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)



qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI})
qt4_add_resources(QRC_SOURCES ${RESOURCES})

add_library(${name} SHARED ${SOURCES} ${HEADERS} ${UI_HEADERS} ${MOC_SOURCES} ${QRC_SOURCES})
target_link_libraries(${name} ${QT_LIBRARIES})
install(TARGETS ${name} DESTINATION ${INSTALL_MODULES_DIR})

endmacro(KamiCmdModuleDev)
