package com.example.domag.tasks

import com.example.domag.utils.time.Period
import java.time.ZonedDateTime

interface DeadlineCalculationStrategy : java.io.Serializable {
    fun calculateDeadline(currentDeadline: ZonedDateTime, period: Period): ZonedDateTime
    fun getType(): DeadlineCalculationStrategyType
}
