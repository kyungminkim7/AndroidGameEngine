package com.example.androidgameengine;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.google.android.material.button.MaterialButton;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameActivity extends AppCompatActivity implements GLSurfaceView.Renderer {
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
    private native void onSurfaceCreatedJNI(Context applicationContext,
                                            int windowWidth, int windowHeight, int displayRotation,
                                            AssetManager assetManager);

    private native void onLeftJoystickInputJNI(float x, float y);
    private native void onRightJoystickInputJNI(float x, float y);

    private native void onResetJNI();
    ///@}

    /// \name Game Engine Functions
    ///
    /// Functions that are implemented and handled by the game engine.
    ///@{
    private native void onStartJNI();
    private native void onResumeJNI();
    private native void onPauseJNI();
    private native void onStopJNI();
    private native void onDestroyJNI();

    private native void onSurfaceChangedJNI(int width, int height, int displayRotation);

    private native void updateJNI();
    private native void renderJNI();

    private native void onTouchDownEventJNI(float x, float y);
    private native void onTouchMoveEventJNI(float x, float y);
    private native void onTouchUpEventJNI(float x, float y);
    ///@}

    private static final String TAG = GameActivity.class.getSimpleName();

    private GLSurfaceView glSurfaceView;
    private Joystick leftJoystick;
    private Joystick rightJoystick;
    private MaterialButton resetButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setContentView(R.layout.activity_game);

        // Set up renderer
        this.glSurfaceView = findViewById(R.id.glSurfaceView);
        this.glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 24, 8);
        this.glSurfaceView.setEGLContextClientVersion(3);
        this.glSurfaceView.setPreserveEGLContextOnPause(true);
        this.glSurfaceView.setRenderer(this);

        this.glSurfaceView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                // Save event results before passing it to glSurfaceView thread to prevent race condition
                float x = event.getX();
                float y = event.getY();

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        glSurfaceView.queueEvent(()->onTouchDownEventJNI(x, y));
                        break;

                    case MotionEvent.ACTION_MOVE:
                        glSurfaceView.queueEvent(()->onTouchMoveEventJNI(x, y));
                        break;

                    case MotionEvent.ACTION_UP:
                        glSurfaceView.queueEvent(()->onTouchUpEventJNI(x, y));
                        break;
                }
                return true;
            }
        });

        this.leftJoystick = findViewById(R.id.leftJoystick);
        this.rightJoystick = findViewById(R.id.rightJoystick);
        this.resetButton = findViewById(R.id.resetButton);
    }

    @Override
    protected void onStart() {
        super.onStart();
        this.onStartJNI();
    }

    @Override
    protected void onResume() {
        super.onResume();
        this.onResumeJNI();
        this.glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        this.glSurfaceView.onPause();
        this.onPauseJNI();
    }

    @Override
    protected void onStop() {
        super.onStop();
        this.onStopJNI();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // Synchronized to avoid racing onDrawFrame.
        synchronized (this) {
            this.onDestroyJNI();
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        this.onSurfaceCreatedJNI(this.getApplicationContext(),
                                 this.glSurfaceView.getWidth(), this.glSurfaceView.getHeight(),
                                 this.getWindowManager().getDefaultDisplay().getRotation(),
                                 getAssets());

        this.leftJoystick.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                boolean result = v.onTouchEvent(event);

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                    case MotionEvent.ACTION_MOVE:
                        glSurfaceView.queueEvent(()-> onLeftJoystickInputJNI(leftJoystick.getMeasurementX(),
                                                                          leftJoystick.getMeasurementY()));
                        break;

                    case MotionEvent.ACTION_UP:
                        glSurfaceView.queueEvent(()-> onLeftJoystickInputJNI(0.0f, 0.0f));
                        break;
                }

                return result;
            }
        });

        this.rightJoystick.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                boolean result = v.onTouchEvent(event);

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                    case MotionEvent.ACTION_MOVE:
                        glSurfaceView.queueEvent(()-> onRightJoystickInputJNI(rightJoystick.getMeasurementX(),
                                                                        rightJoystick.getMeasurementY()));
                        break;

                    case MotionEvent.ACTION_UP:
                        glSurfaceView.queueEvent(()-> onRightJoystickInputJNI(0.0f, 0.0f));
                        break;
                }

                return result;
            }
        });

        this.resetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                glSurfaceView.queueEvent(()-> onResetJNI());
            }
        });
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        this.onSurfaceChangedJNI(width, height,
                                 this.getWindowManager().getDefaultDisplay().getRotation());
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        // Synchronized to avoid racing onDestroy.
        synchronized (this) {
            this.updateJNI();
            this.renderJNI();
        }
    }
}
