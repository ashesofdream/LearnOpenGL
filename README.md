# LearnOpenGL

A record for learning OpenGL follow https://learnopengl-cn.github.io/.  
## Build Tips:
1. I use vcpkg to  install all required packages and set cmake flag:
```asm
-DCMAKE_TOOLCHAIN_FILE="....../vcpkg/current/scripts/buildsystems/vcpkg.cmake"
```
2. add ` add_subdirectory()` to root cmakefile to build any subdirectories,because there are some dependencies written in root cmakelist.txt.
3. If build in windows , please use administrator to build it.Because I use `file(CREATE_LINK ...)` to link glsl and resource file to build directory.