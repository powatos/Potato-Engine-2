
target_sources(PotatoEngine2 PRIVATE
    Control/GameInstance.cpp
    Control/Gamemode.cpp
	Control/IWindowController.cpp

	Data/Asset.cpp
	Data/AssetManager.cpp
	Data/PersistentLevel.cpp

	Fonts/Font.cpp
	Fonts/FontManager.cpp

	Event/EventManager.cpp

	Input/

	Textures/Texture.cpp
	Textures/TextureManager.cpp

	Tick/Tickable.cpp
	Tick/TickManager.cpp
)