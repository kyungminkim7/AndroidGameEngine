# AndroidGameEngine
This is a self-contained 3D Game Engine for Android written in native C++.

### Requirements/Dependencies
- C++14 compiler
- OpenGL ES 3.2
- Currently only works with 3DS files for 3D models.
- Min Android SDK version 24

### Installation
1. Install the latest version of [git](https://git-scm.com/downloads).
2. Install the latest version of [Android Studio](https://developer.android.com/studio/install).
3. Download the [NDK and Tools](https://developer.android.com/ndk/guides).
4. Clone this repository in Android Studio using the following steps:
	- File -> New -> Project from Version Control -> Git
	- Enter https://github.com/kyungminkim7/AndroidGameEngine.git into URL
	- Press "Clone"
5. Open a Git Bash terminal in the topmost AndroidGameEngine directory.
6. Clone project dependencies by entering the following command:
	- git submodule update --init --remote --recursive
7. In cpp/android_game_engine/bullet3/CMakeLists.txt, turn the following OPTIONs OFF:
    - USE_GRAPHICAL_BENCHMARK (Line 27)
    - BUILD_CPU_DEMOS (Line 206)
    - BUILD_BULLET2_DEMOS (Line 405)
    - BUILD_EXTRAS (Line 416)
    - BUILD_UNIT_TESTS (Line 461)
8. Also in cpp/android_game_engine/bullet3/CMakeLists.txt, disable finding an OpenGL pkg by commenting out Lines 288-296.
9. In cpp/android_game_engine/glm/CMakeLists.txt, turn the following OPTIONs OFF:
    - BUILD_SHARED_LIBS (Line 10)
    - GLM_TEST_ENABLE (Line 98)
	
### Project Setup
- In order to create your game, provide a subclass of the Game class in the cpp/ directory and make the necessary modifications to the CMakeLists.txt file in the same directory. An example template is shown by the TestGame.h and TestGame.cpp files.
- TestGame will not compile right away since I am not making the 3D model files available in the assets directory. Replace them with your own.
