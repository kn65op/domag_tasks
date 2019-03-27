package com.example.domag.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test
import java.time.ZoneId
import java.time.ZonedDateTime

class RecurringTaskTest {
    private val summary = "Summary"
    private val date1 = ZonedDateTime.of(
        2011,
        12,
        3,
        10,
        15,
        30,
        0,
        ZoneId.of("+01:00")
    )
    private val task = RecurringTask(summary, date1)

    @Test
    fun `should has proper type`() {
        assertThat(task.type, equalTo("RECURRING TASK"))
    }

    @Test
    fun `should always be not done`() {
        val notDone = false

        assertThat(task.done, equalTo(notDone))
    }

    @Test
    fun `nextDeadline should be date of first deadline`() {
        assertThat(task.nextDeadline, equalTo(date1))
    }
}