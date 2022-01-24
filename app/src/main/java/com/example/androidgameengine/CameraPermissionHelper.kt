package com.example.androidgameengine

import android.Manifest
import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.provider.Settings
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

private const val CAMERA_PERMISSION = Manifest.permission.CAMERA
private const val CAMERA_PERMISSION_CODE = 0

class CameraPermissionHelper {
    companion object {
        fun hasCameraPermission(activity: Activity) =
            ContextCompat.checkSelfPermission(activity, CAMERA_PERMISSION) ==
                    PackageManager.PERMISSION_GRANTED

        fun requestCameraPermission(activity: Activity) =
            ActivityCompat.requestPermissions(activity, arrayOf(CAMERA_PERMISSION),
                CAMERA_PERMISSION_CODE)

        fun shouldShowRequestPermissionRationale(activity: Activity) =
            ActivityCompat.shouldShowRequestPermissionRationale(activity, CAMERA_PERMISSION)

        fun launchPermissionSettings(activity: Activity) {
            val intent = Intent()
            intent.action = Settings.ACTION_APPLICATION_DETAILS_SETTINGS
            intent.data = Uri.fromParts("package", activity.packageName, null)
            activity.startActivity(intent)
        }
    }
}