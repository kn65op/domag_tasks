package com.example.domag

import androidx.test.InstrumentationRegistry
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.uiautomator.By
import androidx.test.uiautomator.UiDevice
import androidx.test.uiautomator.UiSelector
import androidx.test.uiautomator.Until
import com.example.domag.UiTestUtils.launchApp
import com.example.domag.UiTestUtils.prepareEmptyTasks
import com.example.domag.UiTestUtils.prepareOneTask
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

    val device = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation());

    @After
    fun tearDown() {
        device.pressHome()
    }


    @Test
    fun whenAddedPastTask_shouldShowNotification() {
        prepareEmptyTasks()

        prepareOneTask()

        assertNotificationShown()
    }

    private val TIMEOUT = 1000L

    private fun assertNotificationShown() {
        device.openNotification()
        device.wait(Until.hasObject(By.text("You have 1 task")), TIMEOUT)
        val notification = device.findObject(UiSelector().text("You have 1 task"))
        assertThat(notification.exists(), equalTo(true))
    }

}
