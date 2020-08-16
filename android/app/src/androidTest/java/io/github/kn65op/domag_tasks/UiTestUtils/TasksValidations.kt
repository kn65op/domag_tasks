package io.github.kn65op.domag_tasks.UiTestUtils

import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.uiautomator.UiDevice
import androidx.test.uiautomator.UiSelector
import io.github.kn65op.domag_tasks.R
import io.github.kn65op.domag_tasks.tasks.DeadlineCalculationStrategyType
import org.hamcrest.CoreMatchers.equalTo

fun checkTaskSummaryOnPosition(position: Int, summary: String) {
    onView(withId(R.id.MainTasksList)).check(
        matches(
            atPosition(
                position,
                hasDescendant(withText(summary))
            )
        )
    )
}

fun checkTaskOnPosition(position: Int, summary: String, date: Date = date1) {
    checkTaskSummaryOnPosition(position, summary)
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

fun checkDeadlineStrategyTypeOnEdit(strategy: DeadlineCalculationStrategyType) {
    onView(withId(R.id.next_deadline_strategy_type_spinner)).check(
        matches(
            hasDescendant(withText(toDeadlineStrategyText(strategy)))
        )
    )
}
