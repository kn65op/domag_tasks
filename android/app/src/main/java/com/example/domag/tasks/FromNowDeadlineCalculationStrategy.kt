package com.example.domag.tasks

import com.example.domag.utils.time.Period
import java.time.ZonedDateTime

class FromNowDeadlineCalculationStrategy : DeadlineCalculationStrategy {
    override fun calculateDeadline(currentDeadline: ZonedDateTime, period: Period): ZonedDateTime =
        ZonedDateTime.now().plus(period.toJavaPeriod())

    override fun getType(): DeadlineCalculationStrategyType =
        DeadlineCalculationStrategyType.FromNow
}
