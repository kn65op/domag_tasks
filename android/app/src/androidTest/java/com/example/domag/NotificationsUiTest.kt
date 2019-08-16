package com.example.domag

import androidx.test.InstrumentationRegistry
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.uiautomator.UiDevice
import com.example.domag.UiTestUtils.*
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

    private val device = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation())

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

        createSimpleTask(firstTask)

        assertNotificationShown()
    }

    private fun assertNoNotification() {
        openNotificationBarFromHome(device)
        val notification = tasksNotification(device)
        assertThat(notification.exists(), equalTo(false))
    }

    private fun assertNotificationShown() {
        openNotificationBarFromHome(device)
        val notification = tasksNotification(device)
        assertThat(notification.exists(), equalTo(true))
    }

}
