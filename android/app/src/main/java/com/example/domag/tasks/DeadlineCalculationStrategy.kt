package com.example.domag.tasks

import com.example.domag.utils.time.Period
import java.time.ZonedDateTime

interface DeadlineCalculationStrategy {
    fun calculateDeadline(currentDeadline: ZonedDateTime, period: Period): ZonedDateTime
    fun getType() : DeadlineCalculationStrategyType
}
