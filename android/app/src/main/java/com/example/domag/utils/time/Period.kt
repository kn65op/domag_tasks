package com.example.domag.utils.time

class Period(val type: PeriodType, val count: Int) {

    fun toJavaPeriod(): java.time.Period = when (type) {
        PeriodType.Year -> java.time.Period.ofYears(count)
        PeriodType.Month -> java.time.Period.ofMonths(count)
        PeriodType.Week -> java.time.Period.ofWeeks(count)
        PeriodType.Day -> java.time.Period.ofDays(count)
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Period

        if (type != other.type) return false
        if (count != other.count) return false

        return true
    }

    override fun hashCode(): Int {
        var result = type.hashCode()
        result = 31 * result + count
        return result
    }

    companion object {
        fun ofYears(number: Int) = Period(PeriodType.Year, number)
        fun ofMonths(number: Int) = Period(PeriodType.Month, number)
        fun ofWeeks(number: Int) = Period(PeriodType.Week, number)
        fun ofDays(number: Int) = Period(PeriodType.Day, number)
        fun ofJavaPeriod(period: java.time.Period) = Period(PeriodType.Year, period.years)
    }
}
