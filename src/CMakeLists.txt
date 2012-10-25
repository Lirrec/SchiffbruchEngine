
#set(sbe_HDRS ../include/Animation.h          ../include/util/ConcurrentQueue.h
#              ../include/Engine.h            ../include/GameBase.h
#              ../include/Geom.h              ../include/Graphic.h
#              ../include/util/QuadTree.h     ../include/util/HashManager.h
#              ../include/util/HashedString.h ../include/ImageSet.h
#              ../include/Logger.h            ../include/Module.h
#              ../include/util/NamedList.h    ../include/event/Event.h
#              ../include/ResourceManager.h   ../include/Sound.h
#              ../include/event/EventHelper.h ../include/event/EventUser.h
#			  ../include/event/SFMLEvent.h

#              util/TickControl.h		modules/Core.h
#              event/EventQueue.h		event/EventCore.h)
          #set(sbe_SRCS  Animation.cpp         Engine.cpp
          #    GameBase.cpp         Graphic.cpp
          #    ImageSet.cpp         Module.cpp
          #    Logger.cpp           ResourceManager.cpp
          #    Sound.cpp
          #    util/HashManager.cpp      util/HashedString.cpp
          #    util/TickControl.cpp
          #    event/EventCore.cpp  event/Event.cpp
          #    event/EventQueue.cpp event/EventHelper.cpp
          #    event/EventUser.cpp  event/SFMLEvent.cpp
          #    logic/Action.cpp     modules/Core.cpp )

file( GLOB_RECURSE sbe_SRCS RELATIVE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/*.cpp" )
file( GLOB_RECURSE sbe_SRCHDRS RELATIVE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/*.hpp" )

file( GLOB_RECURSE sbe_HDRS RELATIVE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/include/*.hpp" )

message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
message( "-- Found the following HDRS:\n ${sbe_HDRS}" )
message( "-- Found the following SRCHDRS:\n ${sbe_SRCHDRS}" )

add_definitions(-fomit-frame-pointer -fexpensive-optimizations -O3 -DBOOST_THREAD_USE_LIB)
include_directories(${PROJECT_SOURCE_DIR}/include)

if(SHARED)
    add_library(sbe SHARED ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})
else()
    add_library(sbe STATIC ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})
endif()

target_link_libraries(sbe ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${GENERIC_LIBS})
