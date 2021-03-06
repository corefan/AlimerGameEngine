set "SOURCE=%~dp0"
set "SOURCE=%SOURCE:~0,-1%"
set "BUILD=build/android"
if "%TOOLCHAINS%" == "" set "TOOLCHAINS=%SOURCE%\CMake\Toolchains"
cmake -E make_directory "%BUILD%" && cmake -E chdir "%BUILD%" cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAINS%\android.toolchain.cmake" -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI=armeabi-v7a -DANDROID_NATIVE_API_LEVEL=23 "%SOURCE%" && cmake --build "%BUILD%"