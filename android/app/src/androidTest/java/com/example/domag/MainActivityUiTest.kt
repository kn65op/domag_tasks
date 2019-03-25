package com.example.domag

import androidx.test.ext.junit.runners.AndroidJUnit4
import com.example.domag.UiTestUtils.*
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class MainActivityUiTest {

    @Test
    fun whenNoTasks_shouldBeEmpty() {
        prepareEmptyTasks()

        val noTasks = 0
        checkTasksSize(noTasks)
    }

    @Test
    fun whenThereIsTask_shouldBeOnList() {
        prepareOneTask()
        checkTaskOnPosition(0, firstTask)
    }

    @Test
    fun whenThereAreFewTasksWithSameDate_shouldBeSortedByName() {
        prepareThreeTasks()

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, thirdTask)
    }

    @Test
    fun whenThereAreFewTasksWithDifferentDates_shouldBeSortedByDate() {
        prepareEmptyTasks()
        createTask(thirdTask, date1)
        createTask(firstTask, date3)
        createTask(secondTask, date2)

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, thirdTask, date1)
        checkTaskOnPosition(1, secondTask, date2)
        checkTaskOnPosition(2, firstTask, date3)
    }

    @Test
    fun whenTaskIsMarkedAsDone_shouldBeOnBottom() {
        prepareThreeTasks()

        switchTaskDone(firstTask)

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, secondTask)
        checkTaskOnPosition(1, thirdTask)
        checkTaskOnPosition(2, firstTask)
    }

    @Test
    fun whenTaskIsUnmarkedAsDone_shouldBeInOwnPlaceBack() {
        prepareThreeTasks()

        switchTaskDone(firstTask)
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, thirdTask)
    }

    @Test
    fun afterRemovalOfTask_shouldHaveOnlyTwoTasks() {
        prepareThreeTasks()

        removeTask(secondTask)

        val remainingTasks = 2
        checkTasksSize(remainingTasks)
        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, thirdTask)
    }

    @Test
    fun taskSummaryEdit() {
        prepareOneTask()

        changeTaskSummary(firstTask, secondTask)

        checkTaskOnPosition(0, secondTask)
    }

    @Test
    fun taskDateEdit() {
        prepareOneTask()

        changeTaskDate(firstTask, date2)

        checkTaskOnPosition(0, firstTask, date2)
    }

    @Test
    fun tasksShouldBeSortedAfterChanges() {
        prepareThreeTasks()

        changeTaskDate(firstTask, date3)
        changeTaskSummary(thirdTask, zeroTask)

        checkTaskOnPosition(0, zeroTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, firstTask, date3)
    }

    @Test
    fun shouldRemoveOnlyCompletedTasks() {
        prepareThreeTasks()
        switchTaskDone(firstTask)

        removeDoneTasks()

        val remainingTasks = 2
        checkTasksSize(remainingTasks)
        checkTaskOnPosition(0, secondTask)
        checkTaskOnPosition(1, thirdTask)
    }

}
