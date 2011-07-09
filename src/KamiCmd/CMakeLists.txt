set(SOURCES 
    main.cpp
    KamiCmd.cpp
)

set(HEADERS
    KamiCmd.h
)

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})

SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR})
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

if (WIN32)
set(PLATFORM_SPECIFIC
    ${PLATFORM_SPECIFIC}
    KamiCmd.rc
)
endif()

if(APPLE)
SET(PLATFORM_SPECIFIC "KamiCmd.icns")
    SET_SOURCE_FILES_PROPERTIES(
      "KamiCmd.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
    )

    SET(MACOSX_BUNDLE_ICON_FILE MacIcon.icns)
endif()

if(UNIX)
    find_program(XDG-DESKTOP-MENU_EXECUTABLE xdg-desktop-menu)
    configure_file(KamiCmd.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/KamiCmd.desktop)    
    install(CODE "execute_process(COMMAND ${XDG-DESKTOP-MENU_EXECUTABLE} install --novendor ${CMAKE_CURRENT_BINARY_DIR}/KamiCmd.desktop)")
    install(FILES KamiCmd.png KamiCmd.ico DESTINATION ${INSTALL_SHARE_DIR})
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(directories.h.in ${CMAKE_CURRENT_BINARY_DIR}/directories.h)

set (GENERATED
    ${CMAKE_CURRENT_BINARY_DIR}/directories.h
)

add_executable(KamiCmd ${SOURCES} ${HEADERS} ${MOC_SOURCES} ${PLATFORM_SPECIFIC} ${GENERATED})
target_link_libraries(KamiCmd ${QT_LIBRARIES})

install(TARGETS KamiCmd DESTINATION ${INSTALL_BIN_DIR})