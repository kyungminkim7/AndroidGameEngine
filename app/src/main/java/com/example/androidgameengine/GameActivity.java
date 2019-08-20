package com.example.androidgameengine;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameActivity extends AppCompatActivity implements GLSurfaceView.Renderer {
    private static final String TAG = GameActivity.class.getSimpleName();

    private GLSurfaceView glSurfaceView;
    private Joystick rollThrustJoystick;
    private Joystick yawPitchJoystick;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        // Set up renderer
        this.glSurfaceView = findViewById(R.id.glSurfaceView);
        this.glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 24, 8);
        this.glSurfaceView.setEGLContextClientVersion(3);
        this.glSurfaceView.setPreserveEGLContextOnPause(true);
        this.glSurfaceView.setRenderer(this);

        this.glSurfaceView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        GameJNI.onTouchDownEvent(event.getX(), event.getY());
                        break;

                    case MotionEvent.ACTION_MOVE:
                        GameJNI.onTouchMoveEvent(event.getX(), event.getY());
                        break;

                    case MotionEvent.ACTION_UP:
                        GameJNI.onTouchUpEvent(event.getX(), event.getY());
                        break;
                }
                return true;
            }
        });

        this.rollThrustJoystick = findViewById(R.id.rollThrustJoystick);
        this.yawPitchJoystick = findViewById(R.id.yawPitchJoystick);
    }

    @Override
    protected void onDestroy() {
        GameJNI.shutdown();
        super.onDestroy();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GameJNI.init(this.glSurfaceView.getWidth(), this.glSurfaceView.getHeight(), getAssets());

        this.rollThrustJoystick.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                boolean result = v.onTouchEvent(event);

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                    case MotionEvent.ACTION_MOVE:
                        GameJNI.onRollThrustInput(rollThrustJoystick.getMeasurementX(),
                                                  rollThrustJoystick.getMeasurementY());
                        break;

                    case MotionEvent.ACTION_UP:
                        GameJNI.onRollThrustInput(0.0f, 0.0f);
                        break;
                }

                return result;
            }
        });

        this.yawPitchJoystick.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                boolean result = v.onTouchEvent(event);

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                    case MotionEvent.ACTION_MOVE:
                        GameJNI.onYawPitchInput(yawPitchJoystick.getMeasurementX(),
                                                yawPitchJoystick.getMeasurementY());
                        break;

                    case MotionEvent.ACTION_UP:
                        GameJNI.onYawPitchInput(0.0f, 0.0f);
                        break;
                }

                return result;
            }
        });
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GameJNI.setWindowSize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        synchronized (this) {
            GameJNI.update();
            GameJNI.render();
        }
    }
}
