package com.example.domag.UiTestUtils

import androidx.test.core.app.launchActivity
import androidx.test.uiautomator.By
import androidx.test.uiautomator.UiDevice
import androidx.test.uiautomator.UiSelector
import androidx.test.uiautomator.Until
import com.example.domag.MainActivity

fun launchApp() {
    launchActivity<MainActivity>()
}

fun openNotificationBarFromHome(device: UiDevice) {
    val timeout = 1000L
    device.pressHome()
    device.openNotification()
    device.wait(Until.hasObject(By.text("Manage notifications")), timeout)
    device.wait(Until.hasObject(By.text("No notifications")), timeout)
}

private val commonNotificationText = "Tasks to do"

fun tasksNotification(device: UiDevice) = device.findObject(UiSelector().text(commonNotificationText))
