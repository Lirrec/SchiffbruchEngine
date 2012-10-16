
set(sbe_HDRS ../include/Animation.h          ../include/ConcurrentQueue.h
              ../include/Engine.h            ../include/GameBase.h
              ../include/Geom.h              ../include/Graphic.h
              ../include/QuadTree.h          ../include/HashManager.h
              ../include/HashedString.h      ../include/ImageSet.h
              ../include/Logger.h            ../include/Module.h
              ../include/NamedList.h         ../include/event/Event.h
              ../include/event/EventCore.h   ../include/ResourceManager.h
              ../include/event/EventHelper.h ../include/Sound.h
              ../include/event/EventQueue.h  ../include/TickControl.h
              ../include/event/EventUser.h
              ../include/event/SFMLEvent.h   modules/Core.h )
set(sbe_SRCS Animation.cpp         Engine.cpp
              GameBase.cpp         Graphic.cpp
              HashManager.cpp      HashedString.cpp
              ImageSet.cpp         Module.cpp
              Logger.cpp           ResourceManager.cpp
              Sound.cpp            event/Event.cpp
              TickControl.cpp      event/EventCore.cpp
              event/EventQueue.cpp event/EventHelper.cpp
              event/EventUser.cpp  event/SFMLEvent.cpp
              logic/Action.cpp     modules/Core.cpp )

add_definitions(-fomit-frame-pointer -fexpensive-optimizations -O3 -DBOOST_THREAD_USE_LIB)
include_directories(${PROJECT_SOURCE_DIR}/include)

if(SHARED)
	add_library(sbe SHARED ${sbe_HDRS} ${sbe_SRCS})
else()
	add_library(sbe STATIC ${sbe_HDRS} ${sbe_SRCS})
endif()

target_link_libraries(sbe ${Boost_LIBRARIES} ${SFML_LIBRARIES})
