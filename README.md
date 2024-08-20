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
Here you will find all of the necessary third party tools needed to build the application
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

### CMakePresets
The project is currently configured for the following configurations:
- Debug-UnixMake
- Release-UnixMake
- WindowsVS2022
- WIndowsVS2019

In order to generate a configuration and build, run the following commands
```
cmake --preset debug-unix
cmake --build --preset debug-unix
```
The following commands shows you all available configurations.
```
cmake --list-presets
cmake --build --list-presets
```

The executable will be built in ./bin/'BUILD_TYPE' where BUILD_TYPE='Debug' | 'Release'
*** IMPORTANT: You must run the executable from this directory for it to load textures correctly.

### Windows Build
Running one of the windows configurations will generate the solution files needed.

Ex.
```
cmake --preset windows-VS22
cmake --build --preset windows-VS22
```
The above commands generates a solution in build/'BUILD_TYPE'