# AndroidGameEngine
This is a self-contained 3D Game Engine for Android written in native C++.

### Requirements/Dependencies
- C++14 compiler
- OpenGL ES 2.0
- Currently only works with 3DS files for 3D models.

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
	
### Project Setup
- In order to create your game, provide a subclass of the Game class. An example template is shown by the TestGame.h and TestGame.cpp files.
- TestGame will not compile right away since I am not making the 3D model files available in the assets directory. Replace them with your own.