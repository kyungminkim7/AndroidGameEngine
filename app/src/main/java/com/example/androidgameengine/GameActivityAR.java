package com.example.androidgameengine;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;

import com.google.android.material.button.MaterialButton;
import com.google.android.material.snackbar.Snackbar;
import androidx.appcompat.app.AppCompatActivity;

import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameActivityAR extends AppCompatActivity implements GLSurfaceView.Renderer {
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

    private native void onJoystickInputJNI(float x, float y);

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

    private static final String TAG = GameActivityAR.class.getSimpleName();
    private static final int SNACKBAR_COLOR = 0xbf323232;

    private GLSurfaceView glSurfaceView;
    private Joystick joystick;
    private MaterialButton resetButton;

    private Handler arPlaneInitializedHandler;
    private Runnable arPlaneInitializedRunnable = ()->{
        Snackbar snackbar = Snackbar.make(GameActivityAR.this.findViewById(android.R.id.content),
                                    "Touch the floor to set the game.",
                                          Snackbar.LENGTH_INDEFINITE);
        snackbar.getView().setBackgroundColor(SNACKBAR_COLOR);
        snackbar.show();
    };

    private Handler gameInitializedHandler;
    private Runnable gameInitializedRunnable = ()->{
        Snackbar snackbar = Snackbar.make(GameActivityAR.this.findViewById(android.R.id.content),
                                    "Game initialized.",
                                          Snackbar.LENGTH_LONG);
        snackbar.getView().setBackgroundColor(SNACKBAR_COLOR);
        snackbar.show();
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setContentView(R.layout.ar_activity_game);

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

        this.joystick = findViewById(R.id.joystick);
        this.resetButton = findViewById(R.id.resetButton);

        this.arPlaneInitializedHandler = new Handler();
        this.gameInitializedHandler = new Handler();
    }

    @Override
    protected void onStart() {
        super.onStart();
        this.onStartJNI();
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (!CameraPermissionHelper.hasCameraPermission(this)) {
            CameraPermissionHelper.requestCameraPermission(this);
            return;
        }

        Snackbar loadingSnackbar = Snackbar.make(this.findViewById(android.R.id.content),
                                           "Searching for the floor. Move the camera around slowly to scan the environment...",
                                                 Snackbar.LENGTH_INDEFINITE);
        loadingSnackbar.getView().setBackgroundColor(SNACKBAR_COLOR);
        loadingSnackbar.show();

        this.onResumeJNI();
        this.glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        this.glSurfaceView.onPause();
        this.onPauseJNI();

        this.arPlaneInitializedHandler.removeCallbacks(this.arPlaneInitializedRunnable);
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
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] results) {
        if (!CameraPermissionHelper.hasCameraPermission(this)) {
            Toast.makeText(this, "Camera permission is needed to run this application", Toast.LENGTH_LONG)
                    .show();
            if (!CameraPermissionHelper.shouldShowRequestPermissionRationale(this)) {
                // Permission denied with checking "Do not ask again".
                CameraPermissionHelper.launchPermissionSettings(this);
            }
            this.finish();
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        this.onSurfaceCreatedJNI(this.getApplicationContext(),
                                 this.glSurfaceView.getWidth(), this.glSurfaceView.getHeight(),
                                 this.getWindowManager().getDefaultDisplay().getRotation(),
                                 getAssets());

        this.joystick.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                boolean result = v.onTouchEvent(event);
                float x = joystick.getMeasurementX();
                float y = joystick.getMeasurementY();

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                    case MotionEvent.ACTION_MOVE:
                        glSurfaceView.queueEvent(()-> onJoystickInputJNI(x, y));
                        break;

                    case MotionEvent.ACTION_UP:
                        glSurfaceView.queueEvent(()-> onJoystickInputJNI(0.0f, 0.0f));
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

    private void arPlaneInitialized() {this.arPlaneInitializedHandler.post(this.arPlaneInitializedRunnable);}
    private void gameInitialized() {this.gameInitializedHandler.post(this.gameInitializedRunnable);}
}
