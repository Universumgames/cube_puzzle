when first starting this project, you have to download the sdl depencies first. Luckily there is a script [download_dependecies](./dowload_dependencies.sh) which does exactly that.

when trying to package the app and upload to the appstore, you have to make some modifications to the `sdl*` dependencies. You have to check that all "framework" targets have the "skip install" setting under "Build Settings" set to "Yes"

once you are trying to build for the simulator you have to change the "header search paths" for every sdl library (except "SDL")in "build settings" and duplicate the settings containing "$(PLATFORM)" and changing it to "iOS"
