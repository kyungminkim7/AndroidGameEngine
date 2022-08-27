# AndroidGameEngine
This is a self-contained 3D game engine for Android written in native C++. It integrates natively with Google ARCore to allow making augmented reality games.

This game engine has been used to build games such as [AR Drone](https://play.google.com/store/apps/details?id=com.kstudios.ardrone&hl=en_US)
![AR Drone](images/ar_drone.png)

### Requirements/Dependencies
- CMake 3.14 or higher
- C++14 compiler
- OpenGL ES 3.2
- Google ARCore library

### Installation
1. Install the latest version of [git](https://git-scm.com/downloads).
2. Install the latest version of [Android Studio](https://developer.android.com/studio/install).
3. Download the [NDK and Tools](https://developer.android.com/ndk/guides).
4. Clone this repository in Android Studio using the following steps:
    - File -> New -> Project from Version Control -> Git
    - Enter **https://github.com/kyungminkim7/AndroidGameEngine.git** into URL
    - Press **Clone**
5. If you encounter build errors the 1st time you try building indicating that the arcore library is missing, just try running build again - the arcore library should be in the right place the 2nd time building.
	
### Project Game Examples
3 game examples/templates are included in this project:
1. GameActivity (default) - baseline game engine capabilities (screenshot shown below)
2. GameActivityAR - augmented reality app
3. MobileControlStation - uses networking
![GameActivity](images/testgame_screenshot.png)

### Switching Game Examples
To switch to a different game example, make the following changes to the project:
1. Modify `app/src/main/AndroidManifest.xml`:
   1. Comment out the activity block for the Base Game Template.
   2. Uncomment the activity block for the desired game.
2. Modify `app/src/main/cpp/src/CMakeLists.txt`:
   1. Comment out the line: `add_subdirectory(game_activity)`
   2. Uncomment the appropriate `add_subdirectory` line for the desired game example.

### Creating Your Own Game
To create and run your own game, follow the examples/templates and make the following modifications/additions to the project:
1. Create your own Java/Kotlin Activity in `app/src/main/java/com/example/androidgameengine`.
2. Modify `app/src/main/AndroidManifest.xml` to make your Activity the main Activity.
3. Make the corresponding C++ game/project in a subdirectory of `app/src/main/cpp/src`.
4. Modify `app/src/main/cpp/src/CMakeLists.txt` to only invoke `add_subdirectory` for your project.