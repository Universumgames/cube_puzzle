# Cube puzzle
This is a simple 2D SDL2 puzzle game.

## View on [itch.io](https://universegame.itch.io/qubepuzzle)

## Dependencies
Here is a list of all dependencies to compile and run the application on a Debian system (you may have to adjust these names for your package manager):
- libsdl2-dev 
- libsdl2-image-dev 
- libsdl2-mixer-dev 
- libsdl2-ttf-dev 
- cmake 
- build-essential 
- make
- gcc

Here a easy to copy terminal command to install all dependencies with apt
```sh
apt install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev cmake build-essential make gcc
```


## How to install and run
The [`CMakeLists.txt`](CMakeLists.txt) file ist created with unix systems in mind and a personally configured to run on specifically setup windows machine. For unix users, assuming you have installed all dependencies, the easiest way to install and run the application via the [`run.sh`](run.sh) script or by typing `make` in the project directory. For Apple users wanting to compile this application in the XCode IDE the script [`genXcode.sh`](genXcode.sh) is provided to generate all necessary files to open and compile the project in XCode.

## Create new levels

## Create new tutorial levels