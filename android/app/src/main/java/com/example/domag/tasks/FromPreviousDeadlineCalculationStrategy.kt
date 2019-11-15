package com.example.domag.tasks

import com.example.domag.utils.time.Period
import java.time.ZonedDateTime

class FromPreviousDeadlineCalculationStrategy : DeadlineCalculationStrategy() {
    override fun calculateDeadline(someDate: ZonedDateTime, period: Period): ZonedDateTime =
        someDate
}
