# Purpose
The purpose of this project is to learn fundamental game design techniques while learning SFML and creating a 2D game along the way.

# Documentation
The docs directory contains class diagrams as well as notes that I've been writing to help flesh out some fundamental concepts.

# Platforms
I am going to be using CMake and Github actions for this SFML project. For a more in depth look at Platform versions, look at the Actions page. My main goal is to create a singleplayer 2D game that behaves almost identically on all Platforms.

* MacOS 14.6.1
* Ubuntu 22.04LTS
* Windows VS2022, VS2019

# Requirements
Here you will find all of the necessary third part tools needed to build the application
- Git
- CMake

### Linux Specific Dependencies
In order to build the application, the CMake script will first pull SFML from github and build into static libraries. If you are using linux, you will need specific dependencies in order to build SFML. They are listed below.

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

# Building and Running
Use the following commands to build on any OS
```
cmake -B build
cmake --build build --config Debug
```

The exectuable will be output to ./bin

### Windows Build
If you are building for windows ``` cmake -B build ``` will generate a .sln to use with VS2022 or VS2019 in which you can open and build from there.

If using the .sln its important that you click on the main application and select 'Set as Startup Project' Then you should be able to build

