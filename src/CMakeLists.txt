
set(sbe_HDRS Animation.h
              ConcurrentQueue.h UI/ActorDrawable.h
              Engine.h          UI/Items/Button.h
              GameBase.h        UI/Items/Console.h
              Geom.h            UI/Items/DebugWindow.h
              Graphic.h         QuadTree.h
              HashManager.h     UI/Items/Label.h
              HashedString.h    UI/Screen.h
              ImageSet.h        UI/Items/TextBox.h
              Logger.h          UI/UI.h
              Module.h          UI/Window.h
              NamedList.h       event/Event.h
              UI/Items/GameView.h     event/EventCore.h
              ResourceManager.h event/EventHelper.h
              Sound.h           event/EventQueue.h
              TickControl.h     event/EventUser.h
              logic/Action.h    event/SFMLEvent.h
              modules/Core.h )
set(sbe_SRCS Animation.cpp
              Engine.cpp           UI/ActorDrawable.cpp
              GameBase.cpp         UI/Items/Button.cpp
              Graphic.cpp          UI/Items/Console.cpp
              HashManager.cpp      UI/Items/DebugWindow.cpp
              HashedString.cpp     UI/Items/TextBox.cpp
              ImageSet.cpp         UI/Items/Label.cpp
              Module.cpp           UI/Screen.cpp
              UI/Items/GameView.cpp      Logger.cpp
              ResourceManager.cpp  UI/UI.cpp
              Sound.cpp            event/Event.cpp
              TickControl.cpp      event/EventCore.cpp
              event/EventQueue.cpp event/EventHelper.cpp
              event/EventUser.cpp  event/SFMLEvent.cpp
              logic/Action.cpp     modules/Core.cpp )

add_definitions(-fomit-frame-pointer -fexpensive-optimizations -O3 -DBOOST_THREAD_USE_LIB)

if(SHARED)
	add_library(sbe SHARED ${sbe_HDRS} ${sbe_SRCS})
else()
	add_library(sbe STATIC ${sbe_HDRS} ${sbe_SRCS})
endif()

target_link_libraries(sbe ${Boost_LIBRARIES} ${SFML_LIBRARIES})
