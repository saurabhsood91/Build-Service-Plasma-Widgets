# A name for the project
project(plasma-obs)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)

set(obsdataengine_SOURCES obsdataengine.cpp)
set(obsdataengine_HEADERS obsdataengine.h)
set(QT_USE_QTNETWORK true)  
QT4_WRAP_CPP(obsdataengine_HEADERS_MOC ${obsdataengine_HEADERS})

include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(obsdataengine_engine_SRCS obsdataengine.cpp)
#set(datarequest_SRCS datarequest.cpp)
set(QT_USE_QTNETWORK true)  
# Now make sure all files get to the right place
#kde4_add_plugin(datarequest_engine ${datarequest_SRCS})
kde4_add_plugin(obsdataengine_engine ${obsdataengine_engine_SRCS})


#target_link_libraries(datarequest_engine
#                      ${KDE4_KDECORE_LIBS}
#                      ${KDE4_PLASMA_LIBS}
#		      ) 

target_link_libraries(obsdataengine_engine
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS}
		      ${QT_QTNETWORK_LIBRARY})


install(TARGETS obsdataengine_engine
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES obsdataengine.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
