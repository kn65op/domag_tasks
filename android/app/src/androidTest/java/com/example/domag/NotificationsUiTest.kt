package com.example.domag

import androidx.test.InstrumentationRegistry
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.uiautomator.By
import androidx.test.uiautomator.UiDevice
import androidx.test.uiautomator.UiSelector
import androidx.test.uiautomator.Until
import com.example.domag.UiTestUtils.createTask
import com.example.domag.UiTestUtils.firstTask
import com.example.domag.UiTestUtils.launchApp
import com.example.domag.UiTestUtils.prepareEmptyTasks
import org.hamcrest.CoreMatchers.equalTo
import org.hamcrest.MatcherAssert.assertThat
import org.junit.After
import org.junit.Before
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class NotificationsUiTest {
    @Before
    fun setup() {
        launchApp()
    }

    private val device = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation());

    @After
    fun tearDown() {
        device.pressHome()
    }

    @Test
    fun whenNoTask_shouldNotBeNotification() {
        prepareEmptyTasks()

        assertNoNotification()
    }

    @Test
    fun whenAddedPastTask_shouldShowNotification() {
        prepareEmptyTasks()

        createTask(firstTask)

        assertNotificationShown()
    }

    private val timeout = 1000L
    private val commonNotificationText = "Tasks to do"

    private fun assertNoNotification() {
        device.pressHome()
        device.openNotification()
        device.wait(Until.hasObject(By.text("Manage notifications")), timeout)
        device.wait(Until.hasObject(By.text("No notifications")), timeout)
        val notification = device.findObject(UiSelector().text(commonNotificationText))
        assertThat(notification.exists(), equalTo(false))
    }

    private fun assertNotificationShown() {
        device.pressHome()
        device.openNotification()
        device.wait(Until.hasObject(By.text("Manage notifications")), timeout)
        device.wait(Until.hasObject(By.text("No notifications")), timeout)
        val notification = device.findObject(UiSelector().text(commonNotificationText))
        assertThat(notification.exists(), equalTo(true))
    }

}
