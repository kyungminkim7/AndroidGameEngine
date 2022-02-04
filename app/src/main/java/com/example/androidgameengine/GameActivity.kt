package com.example.androidgameengine

import android.content.Context
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.view.MotionEvent
import androidx.appcompat.app.AppCompatActivity
import com.example.androidgameengine.databinding.ActivityGameBinding
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GameActivity : AppCompatActivity(), GLSurfaceView.Renderer {
    companion object {
        private val TAG = GameActivity::class.java.simpleName

        init { System.loadLibrary("game") }
    }

    /// \name Game Functions
    ///
    /// Functions to be declared and implemented by the custom Game class.
    ///@{
    private external fun onLeftJoystickInputJNI(x: Float, y: Float)
    private external fun onRightJoystickInputJNI(x: Float, y: Float)
    private external fun onResetJNI()
    ///@}

    /// \name Game Engine Functions
    ///
    /// Functions that are implemented and handled by the game engine.
    ///@{
    private external fun onCreateJNI(context: Context, assetManager: AssetManager)
    private external fun onStartJNI()
    private external fun onResumeJNI()
    private external fun onPauseJNI()
    private external fun onStopJNI()
    private external fun onDestroyJNI()

    private external fun onSurfaceCreatedJNI(windowWidth: Int, windowHeight: Int,
                                             displayRotation: Int)
    private external fun onSurfaceChangedJNI(width: Int, height: Int,
                                             displayRotation: Int)

    private external fun updateJNI()

    private external fun onTouchDownEventJNI(x: Float, y: Float)
    private external fun onTouchMoveEventJNI(x: Float, y: Float)
    private external fun onTouchUpEventJNI(x: Float, y: Float)
    ///@}

    private lateinit var binding: ActivityGameBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        this.binding = ActivityGameBinding.inflate(this.layoutInflater)
        this.setContentView(this.binding.root)

        // Set up renderer
        this.binding.glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 24, 8)
        this.binding.glSurfaceView.setEGLContextClientVersion(3)
        this.binding.glSurfaceView.preserveEGLContextOnPause = true
        this.binding.glSurfaceView.setRenderer(this)
        this.binding.glSurfaceView.setOnTouchListener{_, event ->
            this.binding.glSurfaceView.queueEvent{
                when (event.action) {
                    MotionEvent.ACTION_DOWN -> this.onTouchDownEventJNI(event.x, event.y)
                    MotionEvent.ACTION_MOVE -> this.onTouchMoveEventJNI(event.x, event.y)
                    MotionEvent.ACTION_UP -> this.onTouchUpEventJNI(event.x, event.y)
                }
            }
            true
        }

        this.onCreateJNI(this.applicationContext, this.assets)
    }

    override fun onStart() {
        super.onStart()
        this.onStartJNI()
        this.binding.glSurfaceView.onResume()
    }

    override fun onResume() {
        super.onResume()
        this.onResumeJNI()
    }

    override fun onPause() {
        super.onPause()
        this.onPauseJNI()
    }

    override fun onStop() {
        super.onStop()
        this.binding.glSurfaceView.onPause()
        this.onStopJNI()
    }

    override fun onDestroy() {
        super.onDestroy()
        this.onDestroyJNI()
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        this.onSurfaceCreatedJNI(
            this.binding.glSurfaceView.width, this.binding.glSurfaceView.height,
            this.windowManager.defaultDisplay.rotation)

        this.binding.leftJoystick.setOnTouchListener{view, event ->
            val result = view.onTouchEvent(event)
            this.binding.glSurfaceView.queueEvent{
                when (event.action) {
                    MotionEvent.ACTION_DOWN, MotionEvent.ACTION_MOVE -> this.onLeftJoystickInputJNI(
                            this.binding.leftJoystick.xAxis, this.binding.leftJoystick.yAxis)

                    else -> this.onLeftJoystickInputJNI(0.0f, 0.0f)
                }
            }
            result
        }

        this.binding.rightJoystick.setOnTouchListener{view, event ->
            val result = view.onTouchEvent(event)
            this.binding.glSurfaceView.queueEvent{
                when (event.action) {
                    MotionEvent.ACTION_DOWN, MotionEvent.ACTION_MOVE ->
                        this.onRightJoystickInputJNI(
                            this.binding.rightJoystick.xAxis, this.binding.rightJoystick.yAxis)

                    else -> this.onRightJoystickInputJNI(0.0f, 0.0f)
                }
            }
            result
        }

        this.binding.resetButton.setOnClickListener{
            this.binding.glSurfaceView.queueEvent{this.onResetJNI()}
        }
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) =
        this.onSurfaceChangedJNI(width, height, this.windowManager.defaultDisplay.rotation)

    override fun onDrawFrame(gl: GL10) = this.updateJNI()
}