package com.example.androidgameengine

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.PointF
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import androidx.core.content.withStyledAttributes
import java.lang.Integer.min

class Joystick(context: Context, attrs: AttributeSet): View(context, attrs) {
    // Returns a range from [-1, 1] from left to right
    val xAxis
        get() = (this.handleCenter.x - this.baseCenter.x) / (this.baseRadius - this.handleRadius)

    // Returns a range from [-1, 1] from bottom to top
    val yAxis
        get() = (this.baseCenter.y - this.handleCenter.y) / (this.baseRadius - this.handleRadius)

    private val basePaint = Paint(Paint.ANTI_ALIAS_FLAG).apply { style = Paint.Style.FILL }
    private var baseCenter = PointF()
    private var baseRadius = 0.0f

    private val handlePaint = Paint(Paint.ANTI_ALIAS_FLAG).apply { style = Paint.Style.FILL }
    private var handleCenter = PointF()
    private var handleRadius = 0.0f
    private var handleSizeRatio = 0.0f

    private var leftBound = 0.0f
    private var rightBound = 0.0f
    private var topBound = 0.0f
    private var bottomBound = 0.0f

    init {
        this.context.withStyledAttributes(attrs, R.styleable.Joystick) {
            basePaint.color = getColor(R.styleable.Joystick_baseColor, 0xff0099b2.toInt())
            handlePaint.color = getColor(R.styleable.Joystick_baseColor, 0xff00b5e7.toInt())
            handleSizeRatio = getFloat(R.styleable.Joystick_handleSizeRatio, 0.4f)
        }
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val minWidth = this.paddingLeft + this.paddingRight + this.suggestedMinimumWidth
        val width = resolveSizeAndState(minWidth, widthMeasureSpec, 1)

        val minHeight = this.paddingTop + this.paddingBottom + this.suggestedMinimumHeight
        val height = resolveSizeAndState(minHeight, heightMeasureSpec, 1)

        val dimension = min(width, height)
        this.setMeasuredDimension(dimension, dimension)
    }

    override fun onSizeChanged(width: Int, height: Int, oldWidth: Int, oldHeight: Int) {
        this.baseCenter = PointF(
            (width / 2).toFloat(),
            (height / 2).toFloat())
        this.baseRadius = (min(width, height) / 2).toFloat()

        this.handleCenter = this.baseCenter
        this.handleRadius = this.baseRadius * this.handleSizeRatio

        this.leftBound = this.baseCenter.x - this.baseRadius + this.handleRadius
        this.rightBound = this.baseCenter.x + this.baseRadius - this.handleRadius
        this.topBound = this.baseCenter.y - this.baseRadius + this.handleRadius
        this.bottomBound = this.baseCenter.y + this.baseRadius - this.handleRadius
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        super.onTouchEvent(event)

        this.handleCenter = when (event.action) {
            MotionEvent.ACTION_DOWN -> PointF(
                event.x.coerceIn(this.leftBound, this.rightBound),
                event.y.coerceIn(this.topBound, this.bottomBound))

            MotionEvent.ACTION_MOVE -> PointF(
                event.x.coerceIn(this.leftBound, this.rightBound),
                event.y.coerceIn(this.topBound, this.bottomBound))

            MotionEvent.ACTION_UP -> this.baseCenter

            else -> return false
        }
        invalidate()

        return true
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        canvas.drawCircle(this.baseCenter.x, this.baseCenter.y, this.baseRadius, this.basePaint)
        canvas.drawCircle(this.handleCenter.x, this.handleCenter.y, this.handleRadius,
            this.handlePaint)
    }
}