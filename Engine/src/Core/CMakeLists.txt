
target_sources(PotatoEngine2 PRIVATE
    Control/GameInstance.cpp
    Control/Gamemode.cpp
	Control/IScreenController.cpp

	Data/DataManager.cpp
	Data/PersistentLevel.cpp

	Event/EventController.cpp

	Input/

	Textures/Texture.cpp
	Textures/TextureManager.cpp

	Tick/Tickable.cpp
	Tick/TickController.cpp
)