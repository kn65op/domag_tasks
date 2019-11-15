package com.example.domag.tasks

import com.example.domag.utils.time.Period
import java.time.ZonedDateTime

abstract class DeadlineCalculationStrategy {
    abstract fun calculateDeadline(currentDeadline: ZonedDateTime, period: Period): ZonedDateTime
}
