package com.example.domag.tasks

import com.example.domag.utils.time.Period
import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test
import java.time.ZonedDateTime

class FromPreviousDeadlineCalculationStrategyTest {
    private val period = Period.ofDays(3)
    private val anyDate = ZonedDateTime.now().plusDays(2)
    private val dateAfterPeriod = ZonedDateTime.now().plusDays(5)

    private fun assertDateMatch(lhs: ZonedDateTime, rhs: ZonedDateTime) {
        assertThat(lhs.toLocalDate(), equalTo(rhs.toLocalDate()))
    }

    @Test
    fun `when passed date should return time advenced from now`() {
        assertDateMatch(
            FromPreviousDeadlineCalculationStrategy().calculateDeadline(anyDate, period),
            dateAfterPeriod
        )
    }
}
