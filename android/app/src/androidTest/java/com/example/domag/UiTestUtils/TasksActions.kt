package com.example.domag.UiTestUtils

import android.widget.DatePicker
import androidx.test.core.app.ApplicationProvider
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.Espresso.openActionBarOverflowOrOptionsMenu
import androidx.test.espresso.action.ViewActions.*
import androidx.test.espresso.contrib.PickerActions
import androidx.test.espresso.matcher.ViewMatchers.*
import com.example.domag.R
import com.example.domag.utils.time.PeriodType
import org.hamcrest.CoreMatchers.allOf
import org.hamcrest.Matchers
import java.lang.Thread.sleep
import java.time.ZonedDateTime
import java.time.format.TextStyle
import java.util.*

fun prepareOneSimpleTask() {
    prepareEmptyTasks()
    createSimpleTask(firstTask)
}

fun createSimpleTask(taskSummary: String, date: Date = date1) {
    clickAddNewTaskButton()
    setTaskSummary(taskSummary)
    setTaskDate(date)
    clickConfirmTaskButton()
}

fun createRecurringTask(
    taskSummary: String,
    date: Date,
    periodAmount: Int,
    periodType: PeriodType
) {
    clickAddNewTaskButton()
    changeToRecurringTask()
    setTaskSummary(taskSummary)
    setTaskDate(date)
    setTaskPeriodAmount(periodAmount)
    setTaskPeriodType(periodType)
    clickConfirmTaskButton()
}

fun createNoDeadlineTask(taskSummary: String) {
    clickAddNewTaskButton()
    changeToNoDeadlineTask()
    setTaskSummary(taskSummary)
    clickConfirmTaskButton()
}

fun setTaskPeriodType(type: PeriodType) {
    onView(withId(R.id.task_period_type_spinner)).perform(click())
    val textToClick = when (type) {
        PeriodType.Year -> "years"
        PeriodType.Month -> "months"
        PeriodType.Week -> "weeks"
        PeriodType.Day -> "days"
    }
    onView(withText(textToClick)).perform(click())
}

fun setTaskPeriodAmount(periodAmount: Int) {
    onView(withId(R.id.task_period_value)).perform(
        clearText(),
        typeText(periodAmount.toString()),
        closeSoftKeyboard()
    )
}

fun changeToRecurringTask() {
    onView(withId(R.id.task_type_selection_spinner)).perform(click())
    onView(withText("Recurring task")).perform(click())
}

fun changeToNoDeadlineTask() {
    onView(withId(R.id.task_type_selection_spinner)).perform(click())
    onView(withText("No deadline task")).perform(click())
}

fun clickAddNewTaskButton() {
    onView(withId(R.id.addNewTaskButton)).perform(click())
}

fun clickConfirmTaskButton() {
    onView(withId(R.id.config_simple_task_button)).perform(click())
    sleep(1000) //Seems that another add task cannot be run so fast
    // as click for add new task does not work fine
}

fun setTaskSummary(taskSummary: String) {
    onView(withId(R.id.newTaskName)).perform(
        clearText(),
        typeText(taskSummary),
        closeSoftKeyboard()
    )
}

fun setTaskDate(date: Date) {
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

fun prepareEmptyTasks() {
    openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
    onView(withText("Remove ALL tasks")).perform(click())
}

fun prepareThreeSimpleTasks() {
    prepareEmptyTasks()
    createSimpleTask(thirdTask)
    createSimpleTask(firstTask)
    createSimpleTask(secondTask)
}

fun switchTaskDone(task: String) {
    onView(allOf(withId(R.id.task_view_done), hasSibling(withText(task)))).perform(click())
}

fun removeTask(task: String) {
    clickOnTask(task)
    openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
    onView(withText("Remove")).perform(click())
}

fun clickOnTask(task: String) {
    onView(withText(task)).perform(click())
}

fun changeTaskSummary(originalTask: String, newTask: String) {
    clickOnTask(originalTask)
    setTaskSummary(newTask)
    clickConfirmTaskButton()
}

fun changeTaskDate(originalTask: String, date: Date) {
    clickOnTask(originalTask)
    setTaskDate(date)
    clickConfirmTaskButton()
}

fun removeDoneTasks() {
    openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
    onView(withText("Remove completed tasks")).perform(click())
}

fun toOwnDate(time: ZonedDateTime) =
    Date(
        time.dayOfMonth,
        time.monthValue,
        time.year,
        time.month.getDisplayName(TextStyle.FULL, Locale.ENGLISH)
    )

