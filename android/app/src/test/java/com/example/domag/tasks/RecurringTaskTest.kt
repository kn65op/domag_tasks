package com.example.domag.tasks

import com.example.domag.utils.time.Period
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
    private val dateAfterPeriod = ZonedDateTime.now().plusDays(3)
    private val period = Period.ofDays(3)
    private val task = RecurringTask(summary, date1, period)
    private val notDone = false

    private fun assertDateMatch(task: Task) {
        assertThat(task.nextDeadline.toLocalDate(), equalTo(dateAfterPeriod.toLocalDate()))
    }

    @Test
    fun `should has proper type`() {
        assertThat(task.type, equalTo("RECURRING TASK"))
    }

    @Test
    fun `should always be not done`() {
        assertThat(task.done, equalTo(notDone))
    }

    @Test
    fun `nextDeadline should be date of first deadline`() {
        assertThat(task.nextDeadline, equalTo(date1))
    }

    @Test
    fun `nextDeadline after marking done should be today plus period`() {
        task.done = true

        assertDateMatch(task)
    }

    @Test
    fun `nextDeadline after marking not done should not be updated`() {
        task.done = false

        assertThat(task.nextDeadline, equalTo(date1))
    }

    @Test
    fun `nextDeadline after marking done twice should update date to today plus period`() {
        task.done = true
        task.done = true

        assertDateMatch(task)
    }

    @Test
    fun `serializeToString should serialize to Json`() {
        val expectedText = """{"summary":"$summary","nextDeadline":"2011-12-03T10:15:30+01:00","period":{"type":"Day","count":3},"id":0}"""
        assertThat(task.serializeToString(), equalTo(expectedText))
    }

    @Test
    fun `should return period`() {
        val expectedPeriod = period

        assertThat(task.period, equalTo(expectedPeriod))
    }

    @Test
    fun `should update period`() {
        val somePeriod = Period.ofMonths(8)
        task.period = somePeriod

        assertThat(task.period, equalTo(somePeriod))
    }
}