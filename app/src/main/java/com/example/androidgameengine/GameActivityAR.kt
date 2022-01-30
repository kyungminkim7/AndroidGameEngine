package com.example.androidgameengine

import android.content.Context
import android.content.res.AssetManager
import android.graphics.Color
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.MotionEvent
import android.view.View.OnTouchListener
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.example.androidgameengine.databinding.ArActivityGameBinding
import com.google.android.material.snackbar.Snackbar
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10


class GameActivityAR: AppCompatActivity(), ActivityCompat.OnRequestPermissionsResultCallback,
    GLSurfaceView.Renderer {
    companion object {
        private val TAG = GameActivityAR::class.java.simpleName
        private const val SNACKBAR_COLOR = -0x40cdcdce
        private const val SNACKBAR_TEXT_COLOR = Color.WHITE

        init { System.loadLibrary("game") }
    }

    /// \name Game Functions
    ///
    /// Functions to be declared and implemented by the custom Game class.
    ///@{

    /// \name Game Functions
    ///
    /// Functions to be declared and implemented by the custom Game class.
    ///@{
    ///
    /// Starting point for game and engine initialization.
    ///
    private external fun onSurfaceCreatedJNI(applicationContext: Context,
                                             windowWidth: Int, windowHeight: Int,
                                             displayRotation: Int, assetManager: AssetManager)

    private external fun onJoystickInputJNI(x: Float, y: Float)

    private external fun onResetJNI()
    ///@}

    ///@}
    /// \name Game Engine Functions
    ///
    /// Functions that are implemented and handled by the game engine.
    ///@{
    private external fun onStartJNI()
    private external fun onResumeJNI()
    private external fun onPauseJNI()
    private external fun onStopJNI()
    private external fun onDestroyJNI()

    private external fun onSurfaceChangedJNI(width: Int, height: Int, displayRotation: Int)

    private external fun updateJNI()
    private external fun renderJNI()

    private external fun onTouchDownEventJNI(x: Float, y: Float)
    private external fun onTouchMoveEventJNI(x: Float, y: Float)
    private external fun onTouchUpEventJNI(x: Float, y: Float)
    ///@}

    ///@}

    private lateinit var binding: ArActivityGameBinding

    private lateinit var arPlaneInitializedHandler: Handler
    private val arPlaneInitializedRunnable = Runnable {
        val snackbar = Snackbar.make(findViewById(android.R.id.content),
            getString(R.string.ar_plane_initialized), Snackbar.LENGTH_INDEFINITE)
        snackbar.view.setBackgroundColor(SNACKBAR_COLOR)
        snackbar.setTextColor(SNACKBAR_TEXT_COLOR)
        snackbar.show()
    }

    private lateinit var gameInitializedHandler: Handler
    private val gameInitializedRunnable = Runnable {
        val snackbar = Snackbar.make(findViewById(android.R.id.content),
            getString(R.string.game_initialized), Snackbar.LENGTH_LONG)
        snackbar.view.setBackgroundColor(SNACKBAR_COLOR)
        snackbar.setTextColor(SNACKBAR_TEXT_COLOR)
        snackbar.show()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        this.binding = ArActivityGameBinding.inflate(this.layoutInflater)
        this.setContentView(this.binding.root)

        // Set up renderer
        this.binding.glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 24, 8)
        this.binding.glSurfaceView.setEGLContextClientVersion(3)
        this.binding.glSurfaceView.preserveEGLContextOnPause = true
        this.binding.glSurfaceView.setRenderer(this)
        this.binding.glSurfaceView.setOnTouchListener(OnTouchListener { _, event -> // Save event results before passing it to glSurfaceView thread to prevent race condition
            this.binding.glSurfaceView.queueEvent {
                when (event.action) {
                    MotionEvent.ACTION_DOWN -> onTouchDownEventJNI( event.x, event.y )
                    MotionEvent.ACTION_MOVE -> onTouchMoveEventJNI( event.x, event.y )
                    MotionEvent.ACTION_UP -> onTouchUpEventJNI( event.x, event.y )
                }
            }
            true
        })

        this.arPlaneInitializedHandler = Handler(Looper.getMainLooper())
        this.gameInitializedHandler = Handler(Looper.getMainLooper())
    }

    override fun onStart() {
        super.onStart()
        this.binding.glSurfaceView.onResume()
        this.binding.glSurfaceView.queueEvent { this.onStartJNI() }
    }

    override fun onResume() {
        super.onResume()

        // Check for camera permissions
        if (!CameraPermissionHelper.hasCameraPermission(this)) {
            CameraPermissionHelper.requestCameraPermission(this)
            return
        }

        // Notify user of env initialization
        val snackbar = Snackbar.make(findViewById(android.R.id.content),
            getString(R.string.ar_plane_search), Snackbar.LENGTH_INDEFINITE)
        snackbar.view.setBackgroundColor(SNACKBAR_COLOR)
        snackbar.setTextColor(SNACKBAR_TEXT_COLOR)
        snackbar.show()

        // Resume game
        this.binding.glSurfaceView.queueEvent { this.onResumeJNI() }
    }

    override fun onPause() {
        super.onPause()

        this.binding.glSurfaceView.queueEvent { this.onPauseJNI() }

        this.arPlaneInitializedHandler.removeCallbacks(arPlaneInitializedRunnable)
        this.gameInitializedHandler.removeCallbacks(gameInitializedRunnable)
    }

    override fun onStop() {
        super.onStop()
        this.binding.glSurfaceView.queueEvent { this.onStopJNI() }
        this.binding.glSurfaceView.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        onDestroyJNI()
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>,
                                            results: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, results)

        if (!CameraPermissionHelper.hasCameraPermission(this)) {
            Toast.makeText(this, "Camera permission is needed to run this application",
                Toast.LENGTH_LONG).show()

            if (!CameraPermissionHelper.shouldShowRequestPermissionRationale(this)) {
                // Permission denied with checking "Do not ask again".
                CameraPermissionHelper.launchPermissionSettings(this)
            }

            finish()
        }
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        this.onSurfaceCreatedJNI(this.applicationContext,
            this.binding.glSurfaceView.width, this.binding.glSurfaceView.height,
            this.windowManager.defaultDisplay.rotation, this.assets)

        this.binding.joystick.setOnTouchListener { view, event ->
            val result = view.onTouchEvent(event)
            this.binding.glSurfaceView.queueEvent {
                when (event.action) {
                    MotionEvent.ACTION_DOWN, MotionEvent.ACTION_MOVE -> this.onJoystickInputJNI(
                        this.binding.joystick.xAxis, this.binding.joystick.yAxis)

                    MotionEvent.ACTION_UP -> this.onJoystickInputJNI(0.0f, 0.0f)
                }
            }
            result
        }

        this.binding.resetButton.setOnClickListener {
            this.binding.glSurfaceView.queueEvent { this.onResetJNI() }
        }
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) =
        this.onSurfaceChangedJNI(width, height, this.windowManager.defaultDisplay.rotation)

    override fun onDrawFrame(gl: GL10) {
        updateJNI()
        renderJNI()
    }

    private fun arPlaneInitialized(): Unit {
        this.arPlaneInitializedHandler.post(arPlaneInitializedRunnable)
    }

    private fun gameInitialized(): Unit {
        this.gameInitializedHandler.post(gameInitializedRunnable)
    }
}