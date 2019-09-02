package com.example.domag.UiTestUtils

import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.uiautomator.UiDevice
import androidx.test.uiautomator.UiSelector
import com.example.domag.R
import org.hamcrest.CoreMatchers.equalTo

fun checkTaskOnPosition(position: Int, summary: String, date: Date = date1) {
    onView(withId(R.id.MainTasksList)).check(
        matches(
            atPosition(
                position,
                hasDescendant(withText(summary))
            )
        )
    )
    onView(withId(R.id.MainTasksList)).check(
        matches(
            atPosition(
                position,
                hasDescendant(withSubstring("${date.day}-${date.monthText}-${date.year}"))
            )
        )
    )
}

fun checkTasksSize(n: Int) {
    onView(withId(R.id.MainTasksList)).check(matches(hasNElements(n)))
}

fun checkTaskIsNotInView(task: String, device: UiDevice) {
    val element = device.findObject(UiSelector().text(task))
    assertThat(element.exists(), equalTo(false))
}

fun checkPeriodTypeOnEdit(periodType: String) {
    onView(withId(R.id.task_period_type_spinner)).check(
        matches(
            hasDescendant(withText(periodType))
        )
    )
}
