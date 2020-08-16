package com.kn65op.domag_tasks.tasks

import com.kn65op.domag_tasks.utils.platform.localization.Localization
import com.kn65op.domag_tasks.utils.time.Period
import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.contains
import com.natpryce.hamkrest.equalTo
import com.nhaarman.mockitokotlin2.any
import com.nhaarman.mockitokotlin2.eq
import com.nhaarman.mockitokotlin2.mock
import com.nhaarman.mockitokotlin2.whenever
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
    private val someDate = ZonedDateTime.now().minusWeeks(3)
    private val someDateTwo = ZonedDateTime.now().minusWeeks(2)
    private val period = Period.ofDays(3)
    private val strategy: DeadlineCalculationStrategy = mock()
    private val task = RecurringTask(
        summary = summary,
        nextDeadline = date1,
        period = period,
        deadlineCalculationStrategy = strategy
    )
    private val notDone = false

    init {
        whenever(strategy.getType()).thenReturn(DeadlineCalculationStrategyType.FromNow)
    }

    private fun assertDateMatch(task: Task, expectedDate: ZonedDateTime) {
        assertThat(task.nextDeadline?.toLocalDate(), equalTo(expectedDate.toLocalDate()))
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
    fun `nextDeadline with after marking done should be date returned from strategy`() {
        whenever(strategy.calculateDeadline(date1, period)).thenReturn(someDate)
        task.done = true

        assertDateMatch(task, someDate)
    }

    @Test
    fun `nextDeadline after marking not done should not be updated`() {
        task.done = false

        assertThat(task.nextDeadline, equalTo(date1))
    }

    @Test
    fun `nextDeadline after marking done twice should update date to day returned from strategy`() {
        whenever(strategy.calculateDeadline(date1, period)).thenReturn(someDate)
        whenever(strategy.calculateDeadline(someDate, period)).thenReturn(someDateTwo)
        task.done = true
        task.done = true

        assertDateMatch(task, someDateTwo)
    }

    @Test
    fun `serializeToString should serialize to Json`() {
        val expectedText =
            """{"summary":"$summary","nextDeadline":"2011-12-03T10:15:30+01:00","period":{"type":"Day","count":3},"id":0,"deadlineCalculationStrategy":0}"""
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

    @Test
    fun `should return proper time information`() {
        val localization: Localization = mock()
        val someText = "some"
        whenever(localization.getPluralWithNumberFor(any(), eq(3))).thenReturn(someText)
        assertThat(
            task.nextDeadlineText(localization),
            contains(Regex(""".+03-.+-2011 \($someText\)"""))
        )
    }
}