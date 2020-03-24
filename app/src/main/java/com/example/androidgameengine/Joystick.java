package com.example.androidgameengine;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class Joystick extends View {
    private Paint basePaint;
    private float baseCenterX;
    private float baseCenterY;
    private float baseRadius;

    private Paint handlePaint;
    private float handleCenterX;
    private float handleCenterY;
    private float handleRadius;
    private float handleSizeRatio;

    private float leftBound;
    private float rightBound;
    private float topBound;
    private float bottomBound;

    private static float clip(float n, float lowerBound, float upperBound) {
        return Math.max(lowerBound, Math.min(n, upperBound));
    }

    public Joystick(Context context, AttributeSet attribs) {
        super(context, attribs);

        this.basePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        this.basePaint.setStyle(Paint.Style.FILL);

        this.handlePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        this.handlePaint.setStyle(Paint.Style.FILL);

        TypedArray a = context.getTheme().obtainStyledAttributes(attribs, R.styleable.Joystick, 0, 0);
        try {
            this.basePaint.setColor(a.getInteger(R.styleable.Joystick_baseColor, 0xff0099b2));
            this.handlePaint.setColor(a.getInteger(R.styleable.Joystick_handleColor, 0xff00b5e7));
            this.handleSizeRatio = a.getFloat(R.styleable.Joystick_handleSizeRatio, 0.4f);
        } finally {
            a.recycle();
        }
    }

    // Returns a range from [-1, 1] from left to right
    public float getMeasurementX() {
        return (this.handleCenterX - this.baseCenterX) / (this.baseRadius - this.handleRadius);
    }

    // Returns a range from [-1, 1] from bottom to top
    public float getMeasurementY() {
        return (this.baseCenterY - this.handleCenterY) / (this.baseRadius - this.handleRadius);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int minWidth = this.getPaddingLeft() + this.getPaddingRight() + this.getSuggestedMinimumWidth();
        int width = View.resolveSizeAndState(minWidth, widthMeasureSpec, 1);

        int minHeight = this.getPaddingTop() + this.getPaddingBottom() + this.getSuggestedMinimumHeight();
        int height = View.resolveSizeAndState(minHeight, heightMeasureSpec, 1);

        int dimension = Math.min(width, height);
        setMeasuredDimension(dimension, dimension);
    }

    @Override
    protected void onSizeChanged(int width, int height, int oldWidth, int oldHeight) {
        this.baseCenterX = width / 2;
        this.baseCenterY = height / 2;
        this.baseRadius = Math.min(width, height) / 2;

        this.handleCenterX = this.baseCenterX;
        this.handleCenterY = this.baseCenterY;
        this.handleRadius = this.baseRadius * this.handleSizeRatio;

        this.leftBound = this.baseCenterX - this.baseRadius + this.handleRadius;
        this.rightBound = this.baseCenterX + this.baseRadius - this.handleRadius;
        this.topBound = this.baseCenterY - this.baseRadius + this.handleRadius;
        this.bottomBound = this.baseCenterY + this.baseRadius - this.handleRadius;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        super.onTouchEvent(event);

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                this.handleCenterX = clip(event.getX(), this.leftBound, this.rightBound);
                this.handleCenterY = clip(event.getY(), this.topBound, this.bottomBound);
                break;

            case MotionEvent.ACTION_MOVE:
                this.handleCenterX = clip(event.getX(), this.leftBound, this.rightBound);
                this.handleCenterY = clip(event.getY(), this.topBound, this.bottomBound);
                break;

            case MotionEvent.ACTION_UP:
                this.handleCenterX = this.baseCenterX;
                this.handleCenterY = this.baseCenterY;
                break;
        }

        invalidate();
        return true;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        canvas.drawCircle(this.baseCenterX, this.baseCenterY, this.baseRadius, this.basePaint);
        canvas.drawCircle(this.handleCenterX, this.handleCenterY, this.handleRadius, this.handlePaint);
    }
}
