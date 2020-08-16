package io.github.kn65op.domag_tasks.tasks

import io.github.kn65op.domag_tasks.utils.time.Period
import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test
import java.time.ZonedDateTime

class FromPreviousDeadlineCalculationStrategyTest {
    private val period = Period.ofDays(3)
    private val today = ZonedDateTime.now()
    private val anyDate = today.minusDays(2)
    private val dateAfterPeriod = today.plusDays(1)

    private fun assertDateMatch(lhs: ZonedDateTime, rhs: ZonedDateTime) {
        assertThat(lhs.toLocalDate(), equalTo(rhs.toLocalDate()))
    }

    @Test
    fun `when passed date should return time advanced from last deadline`() {
        assertDateMatch(
            FromPreviousDeadlineCalculationStrategy().calculateDeadline(anyDate, period),
            dateAfterPeriod
        )
    }

    @Test
    fun `given last deadline more in past then now minus period when passed date should return time advanced to next deadline not now or past`() {
        val previousDeadline = today.minusDays(4)
        val expectedNextDeadline = today.plusDays(2)
        assertDateMatch(
            FromPreviousDeadlineCalculationStrategy().calculateDeadline(previousDeadline, period),
            expectedNextDeadline
        )
    }

    @Test
    fun `given last deadline in past equal to now minus period when passed date should return time advanced to next deadline not now or past`() {
        val previousDeadline = today.minusDays(3)
        val expectedNextDeadline = today.plusDays(3)
        assertDateMatch(
            FromPreviousDeadlineCalculationStrategy().calculateDeadline(previousDeadline, period),
            expectedNextDeadline
        )
    }

    @Test
    fun `should return proper type`() {
        assertThat(
            FromPreviousDeadlineCalculationStrategy().getType(),
            equalTo(DeadlineCalculationStrategyType.FromLastDeadline)
        )
    }
}
