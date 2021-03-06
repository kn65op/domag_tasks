package io.github.kn65op.domag_tasks.tasks

import io.github.kn65op.domag_tasks.utils.time.Period
import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test
import java.time.ZonedDateTime

class FromNowDeadlineCalculationStrategyTest {
    private val period = Period.ofDays(3)
    private val dateAfterPeriod = ZonedDateTime.now().plusDays(3)
    private val anyDate = ZonedDateTime.now().minusYears(3)

    private fun assertDateMatch(lhs: ZonedDateTime, rhs: ZonedDateTime) {
        assertThat(lhs.toLocalDate(), equalTo(rhs.toLocalDate()))
    }

    @Test
    fun `when passed date should return time advanced from now`() {
        assertDateMatch(
            FromNowDeadlineCalculationStrategy().calculateDeadline(anyDate, period),
            dateAfterPeriod
        )
    }

    @Test
    fun `should return proper type`() {
        assertThat(
            FromNowDeadlineCalculationStrategy().getType(),
            equalTo(DeadlineCalculationStrategyType.FromNow)
        )
    }
}
