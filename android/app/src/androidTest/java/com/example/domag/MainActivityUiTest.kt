package com.example.domag

import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.platform.app.InstrumentationRegistry
import androidx.test.uiautomator.UiDevice
import com.example.domag.UiTestUtils.*
import org.junit.Before
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class MainActivityUiTest {
    @Before
    fun setup() {
        launchApp()
    }

    @Test
    fun whenNoTasks_shouldBeEmpty() {
        prepareEmptyTasks()

        val noTasks = 0
        checkTasksSize(noTasks)
    }

    @Test
    fun whenThereIsTask_shouldBeOnList() {
        prepareOneSimpleTask()
        checkTaskOnPosition(0, firstTask)
    }

    @Test
    fun whenThereAreFewTasksWithSameDate_shouldBeSortedByName() {
        prepareThreeSimpleTasks()

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, thirdTask)
    }

    @Test
    fun whenThereAreFewTasksWithDifferentDates_shouldBeSortedByDate() {
        prepareEmptyTasks()
        createSimpleTask(thirdTask, date1)
        createSimpleTask(firstTask, date3)
        createSimpleTask(secondTask, date2)

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, thirdTask, date1)
        checkTaskOnPosition(1, secondTask, date2)
        checkTaskOnPosition(2, firstTask, date3)
    }

    @Test
    fun whenTaskIsMarkedAsDone_shouldBeOnBottom() {
        prepareThreeSimpleTasks()

        switchTaskDone(firstTask)

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, secondTask)
        checkTaskOnPosition(1, thirdTask)
        checkTaskOnPosition(2, firstTask)
    }

    @Test
    fun whenTaskIsUnmarkedAsDone_shouldBeInOwnPlaceBack() {
        prepareThreeSimpleTasks()

        switchTaskDone(firstTask)
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, thirdTask)
    }

    @Test
    fun afterRemovalOfTask_shouldHaveOnlyTwoTasks() {
        prepareThreeSimpleTasks()

        removeTask(secondTask)

        val remainingTasks = 2
        checkTasksSize(remainingTasks)
        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, thirdTask)
    }

    @Test
    fun taskSummaryEdit() {
        prepareOneSimpleTask()

        changeTaskSummary(firstTask, secondTask)

        checkTaskOnPosition(0, secondTask)
    }

    @Test
    fun taskDateEdit() {
        prepareOneSimpleTask()

        changeTaskDate(firstTask, date2)

        checkTaskOnPosition(0, firstTask, date2)
    }

    @Test
    fun tasksShouldBeSortedAfterChanges() {
        prepareThreeSimpleTasks()

        changeTaskDate(firstTask, date3)
        changeTaskSummary(thirdTask, zeroTask)

        checkTaskOnPosition(0, zeroTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, firstTask, date3)
    }

    @Test
    fun shouldRemoveOnlyCompletedTasks() {
        prepareThreeSimpleTasks()
        switchTaskDone(firstTask)

        removeDoneTasks()

        val remainingTasks = 2
        checkTasksSize(remainingTasks)
        checkTaskOnPosition(0, secondTask)
        checkTaskOnPosition(1, thirdTask)
        val device = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation())
        checkTaskIsNotInView(firstTask, device)
    }

}
