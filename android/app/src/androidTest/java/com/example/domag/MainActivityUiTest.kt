package com.example.domag

import android.widget.DatePicker
import androidx.test.core.app.ApplicationProvider
import androidx.test.core.app.launchActivity
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.Espresso.openActionBarOverflowOrOptionsMenu
import androidx.test.espresso.action.ViewActions.*
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.contrib.PickerActions
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.ext.junit.runners.AndroidJUnit4
import com.example.domag.UiTestUtils.atPosition
import com.example.domag.UiTestUtils.hasNElements
import org.hamcrest.CoreMatchers.allOf
import org.hamcrest.Matchers
import org.junit.Test
import org.junit.runner.RunWith
import java.lang.Thread.sleep

@RunWith(AndroidJUnit4::class)
class MainActivityUiTest {

    private val zeroTask = "0 task Zero"
    private val firstTask = "A first task"
    private val secondTask = "B second task"
    private val thirdTask = "C task"
    private val threeTasks = 3
    private val year1 = 2019
    private val month1 = 2
    private val monthName1 = "February"
    private val month2 = 7
    private val monthName2 = "July"
    private val day1 = 3
    private val day2 = 8
    private val day3 = 28

    data class Date(val day: Int, val month: Int, val year: Int, val monthText: String)

    private val date1 = Date(day1, month1, year1, monthName1)
    private val date2 = Date(day2, month2, year1, monthName2)
    private val date3 = Date(day3, month2, year1, monthName2)

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

    private fun prepareOneTask() {
        prepareEmptyTasks()
        createTask(firstTask)
    }

    private fun createTask(taskSummary: String, date: Date = date1) {
        clickAddNewTaskButton()
        setTaskSummary(taskSummary)
        setTaskDate(date)
        clickConfirmTaskButton()
    }

    private fun clickAddNewTaskButton() {
        onView(withId(R.id.addNewTaskButton)).perform(click())
    }

    private fun clickConfirmTaskButton() {
        onView(withId(R.id.config_simple_task_button)).perform(click())
        sleep(1000) //TODO: Seems that another add task cannot be run so fast
        // as click for add new task does not work fine
    }

    private fun setTaskSummary(taskSummary: String) {
        onView(withId(R.id.newTaskName)).perform(clearText(), typeText(taskSummary), closeSoftKeyboard())
    }

    private fun setTaskDate(date: Date) {
        onView(withId(R.id.add_task_date)).perform(click())
        onView(withClassName(Matchers.equalTo(DatePicker::class.java.name))).perform(
            PickerActions.setDate(
                date.year,
                date.month,
                date.day
            )
        )
        onView(withId(android.R.id.button1)).perform(click())
    }

    private fun prepareEmptyTasks() {
        launchActivity<MainActivity>()
        openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
        onView(withText("Remove tasks")).perform(click())
    }

    private fun prepareThreeTasks() {
        prepareEmptyTasks()
        createTask(thirdTask)
        createTask(firstTask)
        createTask(secondTask)
    }

    private fun switchTaskDone(task: String) {
        onView(allOf(withId(R.id.task_view_done), hasSibling(withText(task)))).perform(click())
    }

    private fun checkTaskOnPosition(position: Int, summary: String, date: Date = date1) {
        onView(withId(R.id.MainTasksList)).check(matches(
            atPosition(
                position,
                hasDescendant(withText(summary))
            )
        ))
        onView(withId(R.id.MainTasksList)).check(
            matches(
                atPosition(
                    position,
                    hasDescendant(withSubstring("${date.day}-${date.monthText}-${date.year}"))
                )
            )
        )
    }

    private fun checkTasksSize(n: Int) {
        onView(withId(R.id.MainTasksList)).check(matches(hasNElements(n)))
    }

    private fun removeTask(task: String) {
        clickOnTask(task)
        openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
        onView(withText("Remove")).perform(click())
    }

    private fun clickOnTask(task: String) {
        onView(withText(task)).perform(click())
    }

    private fun changeTaskSummary(originalTask: String, newTask: String) {
        clickOnTask(originalTask)
        setTaskSummary(newTask)
        clickConfirmTaskButton()
    }

    private fun changeTaskDate(originalTask: String, date: Date) {
        clickOnTask(originalTask)
        setTaskDate(date)
        clickConfirmTaskButton()
    }

    private fun removeDoneTasks() {
        openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
        onView(withText("Remove completed tasks")).perform(click())
    }
}
