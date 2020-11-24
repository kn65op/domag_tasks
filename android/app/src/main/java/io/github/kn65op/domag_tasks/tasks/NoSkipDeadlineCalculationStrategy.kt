package io.github.kn65op.domag_tasks.tasks

import io.github.kn65op.domag_tasks.utils.time.Period
import java.time.ZonedDateTime

class NoSkipDeadlineCalculationStrategy : DeadlineCalculationStrategy {
    override fun calculateDeadline(currentDeadline: ZonedDateTime, period: Period): ZonedDateTime =
        currentDeadline.plus(period.toJavaPeriod())

    override fun getType(): DeadlineCalculationStrategyType =
        DeadlineCalculationStrategyType.NoSkip
}
