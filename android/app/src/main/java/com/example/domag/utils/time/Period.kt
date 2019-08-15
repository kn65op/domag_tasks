package com.example.domag.utils.time

class Period(val type: PeriodType, val count: Int) {
    fun toJavaPeriod(): java.time.Period = when (type) {
        PeriodType.Year -> java.time.Period.ofYears(count)
        PeriodType.Week -> java.time.Period.ofWeeks(count)
        else -> java.time.Period.ofMonths(count)
    }

    companion object {
        fun ofYears(number: Int) = Period(PeriodType.Year, number)
        fun ofMonths(number: Int) = Period(PeriodType.Month, number)
        fun ofWeeks(number: Int) = Period(PeriodType.Week, number)
        fun ofDays(number: Int) = Period(PeriodType.Day, number)
    }
}
