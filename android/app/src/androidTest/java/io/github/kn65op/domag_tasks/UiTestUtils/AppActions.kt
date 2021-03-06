package io.github.kn65op.domag_tasks.UiTestUtils

import androidx.test.core.app.launchActivity
import androidx.test.espresso.Espresso
import androidx.test.espresso.action.ViewActions
import androidx.test.espresso.matcher.ViewMatchers
import androidx.test.uiautomator.By
import androidx.test.uiautomator.UiDevice
import androidx.test.uiautomator.UiSelector
import androidx.test.uiautomator.Until
import io.github.kn65op.domag_tasks.gui.MainActivity

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

private const val commonNotificationText = "Tasks to do"

fun tasksNotification(device: UiDevice) =
    device.findObject(UiSelector().text(commonNotificationText))!!

fun goToNoDeadlineTasks() {
    Espresso.onView(ViewMatchers.withText("Tasks without deadline")).perform(ViewActions.click())
}
