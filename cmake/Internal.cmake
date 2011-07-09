# A hack, actually. A real install target is needed.
set(EXECUTABLE_OUTPUT_PATH ${${TARGET_NAME}_BINARY_DIR}/)

# Default install path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/dist CACHE PATH 
        "Install path prefix, prepended onto install directories" FORCE)
endif()

if (WIN32)
set(_DEFAULT_BIN_DIR "bin")
set(_DEFAULT_LIB_DIR "bin")
set(_DEFAULT_CONFIG_DIR "config")
set(_DEFAULT_DATA_DIR "data")
set(_DEFAULT_TOOLS_DIR "bin")
set(_DEFAULT_SBIN_DIR "bin")
set(_DEFAULT_SHARE_DIR "share")
else()
set(_DEFAULT_BIN_DIR "bin")
set(_DEFAULT_LIB_DIR "lib/KamiCmd")
set(_DEFAULT_CONFIG_DIR "config")
set(_DEFAULT_DATA_DIR "data")
set(_DEFAULT_TOOLS_DIR "lib/KamiCmd/bin")
set(_DEFAULT_SBIN_DIR "sbin")
set(_DEFAULT_SHARE_DIR "share")
endif()

set(INSTALL_BIN_DIR ${_DEFAULT_BIN_DIR} CACHE PATH "Install dir for command-line tools")
set(INSTALL_LIB_DIR ${_DEFAULT_LIB_DIR} CACHE PATH "Install dir for shared libraries tools")
set(INSTALL_CONFIG_DIR ${_DEFAULT_CONFIG_DIR} CACHE PATH "Install dir for config files")
set(INSTALL_DATA_DIR ${_DEFAULT_DATA_DIR} CACHE PATH "Install dir for data files")
set(INSTALL_TOOLS_DIR ${_DEFAULT_TOOLS_DIR} CACHE PATH "Install dir for internal tools")
set(INSTALL_SBIN_DIR ${_DEFAULT_SBIN_DIR} CACHE PATH "Install dir for daemons")
set(INSTALL_SHARE_DIR ${_DEFAULT_SHARE_DIR} CACHE PATH "Install dir for miscelanious files")
set(INSTALL_MODULES_DIR ${_DEFAULT_LIB_DIR}/Modules CACHE PATH "Install dir for modules")

function(get_absolute_path OUT DIR)
    get_filename_component(_ABS_PATH ${DIR} ABSOLUTE)
    if (${_ABS_PATH} STREQUAL ${DIR})
        # Absolute path
        set(${OUT} ${DIR} PARENT_SCOPE)
    else()
        # Relative (to install prefix) path
        set(${OUT} ${CMAKE_INSTALL_PREFIX}/${DIR} PARENT_SCOPE)
    endif()
endfunction()

get_absolute_path(INSTALL_CONFIG_PATH ${INSTALL_CONFIG_DIR})
get_absolute_path(INSTALL_DATA_PATH ${INSTALL_DATA_DIR})

if (WIN32)
# Require Windows XP, at least
add_definitions("-D_WIN32_WINNT=0x0501")
endif()