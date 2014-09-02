cmake_minimum_required( VERSION 2.6 )

set(PROJECT lxqt-panel )

set(lxqt-panel_PRIV_H_FILES
    lxqtpanel.h
    lxqtpanellayout.h
    lxqtpanelpluginconfigdialog.h
    config/configpaneldialog.h
    plugin.h
    lxqtpanellimits.h
    popupmenu.h
    pluginmoveprocessor.h
)


# using LxQt namespace in the public headers.
set(lxqt-panel_PUB_H_FILES
    lxqtpanelglobals.h
    ilxqtpanelplugin.h
    ilxqtpanel.h
)

set(lxqt-panel_CPP_FILES
    
    lxqtpanel.cpp
    lxqtpanellayout.cpp
    lxqtpanelpluginconfigdialog.cpp
    config/configpaneldialog.cpp
    plugin.cpp
    popupmenu.cpp
    pluginmoveprocessor.cpp
)

set(MOCS
    lxqtpanel.h
    lxqtpanellayout.h
    lxqtpanelpluginconfigdialog.h
    config/configpaneldialog.h
    plugin.h
    pluginmoveprocessor.h
)

if(MOONLIGHT_MOD)
    set(lxqt-panel_CPP_FILES
        ${lxqt-panel_CPP_FILES}
        Activator.cpp
        moonlightpanelmodule.cpp
    )

    set(MOCS 
        ${MOCS}
        moonlightpanelmodule.h
    )

else()
    set(lxqt-panel_CPP_FILES
        ${lxqt-panel_CPP_FILES}
        main.cpp
        lxqtpanelapplication.cpp
    )

    set(MOCS 
        ${MOCS}
        lxqtpanelapplication.h
    )
endif()

set(LIBRARIES
    ${LXQT_LIBRARIES}
    ${QTXDG_LIBRARIES}
)

# QRC files .................
set(RESOURCES
)

set (lxqt-panel_UI_FILES
    config/configpaneldialog.ui
)

if(USE_QT5)
    qt5_wrap_ui(UI_HEADERS ${lxqt-panel_UI_FILES})
else()
    qt4_wrap_ui(UI_HEADERS ${lxqt-panel_UI_FILES})
endif()

add_definitions(-DDEFULT_PANEL_PLUGINS=\"MainMenu,Quicklaunch,TaskBar,Tray,Clock\")

include(LxQtTranslate)

file (GLOB CONFIG_FILES     resources/*.conf)

############################################

if (NOT CMAKE_BUILD_TYPE)
    set ( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (-Wall)
#    add_definitions (-Wall -pedantic)
endif()

add_definitions(-DCOMPILE_LXQT_PANEL)


set(PLUGIN_DESKTOPS_DIR
    ${CMAKE_INSTALL_PREFIX}/share/lxqt/${PROJECT}
)

add_definitions(-DPLUGIN_DESKTOPS_DIR=\"${PLUGIN_DESKTOPS_DIR}\")


#*******************************************
project(${PROJECT})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LXQT_INCLUDE_DIRS}
    ${LXQT_INCLUDE_DIRS}/LXQt # FIXME: liblxqt seems to be broken, workaround it.
)

if(USE_QT5)
    qt5_wrap_cpp(MOC_SOURCES ${MOCS})
    qt5_wrap_ui(UI_HEADERS ${FORMS})
    qt5_add_resources(QRC_SOURCES ${RESOURCES})
    set(QTX_LIBRARIES Qt5::Widgets Qt5::Xml Qt5::DBus)
else()
    qt4_wrap_cpp(MOC_SOURCES ${MOCS})
    qt4_wrap_ui(UI_HEADERS ${FORMS})
    qt4_add_resources(QRC_SOURCES ${RESOURCES})
    set(QTX_LIBRARIES ${QT_CORE_LIBRARY} ${QT_GUI_LIBRARY} ${QT_XML_LIBRARY} ${QT_DBUS_LIBRARY})
endif()

# Translations **********************************
include(LxQtTranslate)
lxqt_translate_ts(lxqt-runner_QM_FILES 
    SOURCES
        ${lxqt-panel_PUB_H_FILES}
        ${lxqt-panel_PRIV_H_FILES}
        ${lxqt-panel_CPP_FILES} 
        ${lxqt-panel_UI_FILES}
)
#************************************************


set (PLUGIN_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/lxqt-panel/)
add_definitions(-DPLUGIN_DIR=\"${PLUGIN_DIR}\")
message(STATUS "Panel plugins location: ${PLUGIN_DIR}")

if(MOONLIGHT_MOD)
    add_definitions(-DMOONLIGHT_MOD=Yes)

    set(MODULE_NAME moonlightDE-lxqt_panel)
    find_package(MOONLIGHT_CORE REQUIRED)
    include_directories(${MOONLIGHT_CORE_SERIVICES_DIR})

    usFunctionGenerateModuleInit( lxqt-panel_CPP_FILES
        NAME  ${MODULE_NAME}
        LIBRARY_NAME lxqt_panel
    )

    add_library(${MODULE_NAME} SHARED
        ${lxqt-panel_PUB_H_FILES}
        ${lxqt-panel_PRIV_H_FILES}
        ${lxqt-panel_CPP_FILES}
        ${MOC_SOURCES}
        ${lxqt-runner_QM_FILES}
        ${QRC_SOURCES}
        ${UI_HEADERS}
    )

    target_link_libraries(${MODULE_NAME}
        ${LIBRARIES} 
        ${QTX_LIBRARIES}
        # Link the CppMicroServices library
        ${CppMicroServices_LIBRARIES}
    )

    ## --------------------------------------------------------------
    ## Install
    ## --------------------------------------------------------------
    configure_file(descriptor.in  ${MODULE_NAME}.desktop )
    INSTALL(FILES ${CMAKE_BINARY_DIR}/panel/${MODULE_NAME}.desktop 
        DESTINATION ${MOONLIGHT_CORE_DESCRIPTORS_DIR})

    INSTALL(TARGETS ${MODULE_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${MOONLIGHT_CORE_MODULES_DIR}
        ARCHIVE DESTINATION lib/static)
else()
    add_executable(${PROJECT} ${lxqt-panel_PUB_H_FILES} ${lxqt-panel_PRIV_H_FILES} ${lxqt-panel_CPP_FILES} ${MOC_SOURCES} ${lxqt-runner_QM_FILES} ${QRC_SOURCES} ${UI_HEADERS})
    target_link_libraries(${PROJECT} ${LIBRARIES} ${QTX_LIBRARIES})

    install(TARGETS ${PROJECT} RUNTIME DESTINATION bin)

endif()

install(FILES   ${CONFIG_FILES}    DESTINATION ${LXQT_ETC_XDG_DIR}/lxqt)
install(FILES  ${lxqt-panel_PUB_H_FILES} DESTINATION include/lxqt)