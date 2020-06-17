cmake_minimum_required(VERSION 3.15)
project(SpriteGame)
set(CMAKE_CXX_STANDARD 11)
add_subdirectory(acllib)
add_executable(SpriteGame
        GameController.cpp
        sprite/SpriteBase.h GameFrame.h sprite/HeroSprite.h sprite/BulletSprite.h sprite/EnemySprite.h GameFrame.cc Rectangle.h)
target_link_libraries(SpriteGame acllib)
target_link_libraries(SpriteGame gdi32)
target_link_libraries(SpriteGame ole32)
target_link_libraries(SpriteGame oleaut32)
target_link_libraries(SpriteGame winmm)
target_link_libraries(SpriteGame gdi32)
target_link_libraries(SpriteGame msimg32)