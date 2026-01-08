# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("dobbyproject")

######################################################################################################
# 设置自定义输出文件名

option(DOBBY_GENERATE_SHARED "Build shared library" OFF)

option(DOBBY_DEBUG "Enable debug logging" ON)

option(NearBranch "Enable near branch trampoline" ON)

option(FullFloatingPointRegisterPack "Save and pack all floating-point registers" OFF)

option(Plugin.SymbolResolver "Enable symbol resolver" ON)

option(Plugin.ImportTableReplace "Enable import table replace " OFF)

option(Plugin.Android.BionicLinkerUtil "Enable android bionic linker util" OFF)

option(DOBBY_BUILD_EXAMPLE "Build example" OFF)

option(DOBBY_BUILD_TEST "Build test" OFF)

#set(DobbyHome Dobby)
#SET_OPTION(DOBBY_DEBUG ON)
#SET_OPTION(DOBBY_GENERATE_SHARED OFF)

#SET_OPTION(Plugin.LinkerLoadCallback OFF)

add_subdirectory(Dobby)


######################################################################################################


# 添加编译定义
add_definitions(-D_GNU_SOURCE)

# 检测目标架构
if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    add_definitions(-D__aarch64__)
    message("构建 ARM64 架构")
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    add_definitions(-D__arm__)
    message("构建 ARM 架构")
elseif(${ANDROID_ABI} STREQUAL "x86")
    add_definitions(-D__i386__)
    message("构建 x86 架构")
elseif(${ANDROID_ABI} STREQUAL "x86_64")
    add_definitions(-D__x86_64__)
    message("构建 x86_64 架构")
endif()

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)


find_library(
        dl-lib
        dl
)
######################################################################################################

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        start.cpp
        "Log/log.cpp"
        "Symbol/Symbol.cpp"
        "File/file.cpp"
        "UnityApi/unityapi.cpp"
        "li2cpp Dumper/li2cppDumper.cpp"
        "lol/lolm.cpp"
)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        dobby_static
        android
        log
        ${log-lib}
        ${dl-lib}
)