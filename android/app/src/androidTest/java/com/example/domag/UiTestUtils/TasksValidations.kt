package com.example.domag.UiTestUtils

import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.matcher.ViewMatchers.*
import com.example.domag.R

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

