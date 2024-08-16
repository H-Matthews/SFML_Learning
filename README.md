# Purpose
The purpose of this project is to learn fundamental game design techniques while learning SFML and creating a 2D game along the way. I will be documenting new concepts related to game design in docs/Game_Design_Techniques.md

# Platforms
I am going to be using CMake and Github actions for this SFML project. For a more in depth look at Platform versions, look at the Actions page. My main goal is to create a singleplayer 2D game that behaves almost identically on all Platforms.

* MacOS 14.6.1
* Ubuntu 22.04LTS
* Windows VS2022

# Building the Application
In order to build the application, the CMake script will first pull SFML from github and build into static libraries. If you are using linux, you will need specific dependcies in order to build SFML. They are listed below.

### Dependencies
CMake - 3.30.2 (Most Recent version will do)

### Linux Dependencies for SFML
On Ubuntu and other Debian-based distros, you can use the following command
```
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
```

Use the following commands to build on any OS
```
cmake -B build
cmake --build build --config Debug
```

If you are building for windows ``` cmake -B build ``` will build a .sln to use with VS2022