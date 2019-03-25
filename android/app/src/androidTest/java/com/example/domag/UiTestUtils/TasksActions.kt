package com.example.domag.UiTestUtils

import android.widget.DatePicker
import androidx.test.core.app.ApplicationProvider
import androidx.test.core.app.launchActivity
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.Espresso.openActionBarOverflowOrOptionsMenu
import androidx.test.espresso.action.ViewActions.*
import androidx.test.espresso.contrib.PickerActions
import androidx.test.espresso.matcher.ViewMatchers.*
import com.example.domag.R
import org.hamcrest.CoreMatchers.allOf
import org.hamcrest.Matchers
import java.lang.Thread.sleep

fun prepareOneTask() {
    prepareEmptyTasks()
    createTask(firstTask)
}

fun createTask(taskSummary: String, date: Date = date1) {
    clickAddNewTaskButton()
    setTaskSummary(taskSummary)
    setTaskDate(date)
    clickConfirmTaskButton()
}

fun clickAddNewTaskButton() {
    onView(withId(R.id.addNewTaskButton)).perform(click())
}

fun clickConfirmTaskButton() {
    onView(withId(R.id.config_simple_task_button)).perform(click())
    sleep(1000) //TODO: Seems that another add task cannot be run so fast
    // as click for add new task does not work fine
}

fun setTaskSummary(taskSummary: String) {
    onView(withId(R.id.newTaskName)).perform(clearText(), typeText(taskSummary), closeSoftKeyboard())
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
    launchActivity<MainActivity>()
    openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
    onView(withText("Remove tasks")).perform(click())
}

fun prepareThreeTasks() {
    prepareEmptyTasks()
    createTask(thirdTask)
    createTask(firstTask)
    createTask(secondTask)
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
