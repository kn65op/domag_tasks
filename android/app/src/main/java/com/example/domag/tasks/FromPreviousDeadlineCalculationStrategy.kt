package com.example.domag.tasks

import com.example.domag.utils.time.Period
import java.time.ZonedDateTime

class FromPreviousDeadlineCalculationStrategy : DeadlineCalculationStrategy {
    override fun calculateDeadline(currentDeadline: ZonedDateTime, period: Period): ZonedDateTime {
        var nextDeadline = currentDeadline.plus(period.toJavaPeriod())
        val now = ZonedDateTime.now()
        while (nextDeadline <= now) {
            nextDeadline = nextDeadline.plus(period.toJavaPeriod())
        }
        return nextDeadline
    }

    override fun getType(): DeadlineCalculationStrategyType =
        DeadlineCalculationStrategyType.FromLastDeadline
}
