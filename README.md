# Purpose
The purpose of this project is to learn fundamental game design techniques while learning SFML and creating a 2D game along the way.

# Documentation
The docs directory contains class diagrams as well as notes that I've been writing to help flesh out some fundamental concepts. The diagrams were drawn with the DrawIO extension in VSCode.

# Platforms
The project can run on the following platforms

* MacOS 14.6.1
* Ubuntu 22.04LTS
* Windows VS2022, VS2019

# Requirements
Here you will find all of the necessary third party tools needed to build the application regardlesss of platform
- Git
- CMake

### Windows Dependencies
- Visual Studio 2022 \ 2019

### Linux Dependencies
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
The recommended way to build this project is with CMakePresets. If you would like to build it with default commands you can it just needs a specific folder structure for the build files.

### Building with CMake

#### Default Commands
Using default commands: 
```
cmake -B build/{PLATFORM}
cmake --build build/{PLATFORM}
```
Ex. If on windows, PLATFORM=windows

#### CMakePresets
The project is currently configured for the following configurations:
- Debug-UnixMake
- Release-UnixMake
- WindowsVS2022
- WindowsVS2019

The following commands shows you all available configurations
```
cmake --list-presets
cmake --build --list-presets
```

In order to generate a configuration and build, run the following commands
```
cmake --preset 'PRESET_COFNIG_NAME'
cmake --build --preset 'PRESET_NAME'
```

### Windows Specific Build
Using the 'windows-VS22' preset will generate the solution files for Visual Studio
```
cmake --preset windows-VS22
```

## Running
The binary will always reside in bin/'CONFIG_TYPE'. In order for the filepaths to be resolved correctly, it NEEDS be run from this directory OR the build/'PLATFORM' directory

Visual Studio will run it from build/'PLATFORM'