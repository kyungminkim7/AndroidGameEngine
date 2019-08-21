package com.example.androidgameengine;

import android.content.res.AssetManager;

public class GameJNI {
    static {
        System.loadLibrary("game");
    }

    /// \name Game Functions
    ///
    /// Functions to be declared and implemented by the custom Game class.
    ///@{

    ///
    /// Starting point for game and engine initialization.
    ///
    public static native void onSurfaceCreated(int windowWidth, int windowHeight, AssetManager assetManager);

    public static native void onRollThrustInput(float roll, float thrust);
    public static native void onYawPitchInput(float yaw, float pitch);
    ///@}

    /// \name Game Engine Functions
    ///
    /// Functions that are implemented and handled by the game engine.
    ///@{
    public static native void onStart();
    public static native void onResume();
    public static native void onPause();
    public static native void onStop();
    public static native void onDestroy();

    public static native void onSurfaceChanged(int width, int height);

    public static native void update();
    public static native void render();

    public static native void onTouchDownEvent(float x, float y);
    public static native void onTouchMoveEvent(float x, float y);
    public static native void onTouchUpEvent(float x, float y);
    ///@}
}
