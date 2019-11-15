package com.example.domag.tasks

import com.example.domag.utils.time.Period
import java.time.ZonedDateTime

class FromPreviousDeadlineCalculationStrategy : DeadlineCalculationStrategy() {
    override fun calculateDeadline(someDate: ZonedDateTime, period: Period): ZonedDateTime {
        var nextDeadline = someDate.plus(period.toJavaPeriod())
        val now = ZonedDateTime.now()
        while (nextDeadline <=t  now) {
            nextDeadline = nextDeadline.plus(period.toJavaPeriod())
        }
        return nextDeadline
    }
}
