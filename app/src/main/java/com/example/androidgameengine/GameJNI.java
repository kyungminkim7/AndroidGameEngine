package com.example.androidgameengine;

import android.content.res.AssetManager;

public class GameJNI {
    static {
        System.loadLibrary("game");
    }

    public static native void init(int windowWidth, int windowHeight, AssetManager assetManager);
    public static native void shutdown();

    public static native void setWindowSize(int width, int height);

    public static native void update();
    public static native void render();

    public static native void onTouchDownEvent(float x, float y);
    public static native void onTouchMoveEvent(float x, float y);
    public static native void onTouchUpEvent(float x, float y);
}
