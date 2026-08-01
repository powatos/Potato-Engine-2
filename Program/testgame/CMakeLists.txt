# program/testgame

# headers added to silence clion warnings
add_executable(TestGame
	src/main.cpp

	src/CharacterController.cpp
	src/CharacterController.hpp

	src/MyPlayer.cpp
	src/MyPlayer.hpp

	src/TestGm.cpp
	src/TestGm.hpp
)

target_link_libraries(TestGame PRIVATE PotatoEngine2)

ConfigureAssets(TestGame)