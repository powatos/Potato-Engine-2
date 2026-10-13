# program/Platformer

# headers added to silence clion warnings
add_executable(Platformer
	src/main.cpp

	src/CharacterController.cpp
	src/CharacterController.hpp

	src/PlatformPlayer.cpp
	src/PlatformPlayer.hpp

	src/PlatformerGamemode.cpp
	src/PlatformerGamemode.hpp
)

target_link_libraries(Platformer PRIVATE
	"$<LINK_LIBRARY:WHOLE_ARCHIVE,PotatoEngine2>"
)

ConfigureAssets(Platformer)