package com.kn65op.domag_tasks.tasks

import com.kn65op.domag_tasks.utils.time.Period
import java.time.ZonedDateTime

interface DeadlineCalculationStrategy : java.io.Serializable {
    fun calculateDeadline(currentDeadline: ZonedDateTime, period: Period): ZonedDateTime
    fun getType(): DeadlineCalculationStrategyType
}
