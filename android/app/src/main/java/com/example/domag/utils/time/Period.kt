package com.example.domag.utils.time

import kotlinx.serialization.Serializable
import kotlin.math.abs
import kotlin.math.sign

@Serializable
class Period private constructor(val type: PeriodType, val count: Int) : java.io.Serializable {
    class InvalidJavaPeriod(message: String) : Exception(message)

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

    override fun toString(): String {
        return "Period(type=$type, count=$count)"
    }

    companion object {
        fun ofYears(number: Int) = Period(PeriodType.Year, number)
        fun ofMonths(number: Int) = Period(PeriodType.Month, number)
        fun ofWeeks(number: Int) = Period(PeriodType.Week, number)
        fun ofDays(number: Int) = Period(PeriodType.Day, number)
        fun ofJavaPeriod(period: java.time.Period): Period {
            if (moreThenOneUnit(period)) throw InvalidJavaPeriod("More then one unit in period")
            return when {
                period.years > noAmount -> ofYears(period.years)
                period.months > noAmount -> ofMonths(period.months)
                period.days % daysInWeek == noAmount -> ofWeeks(period.days / daysInWeek)
                else -> ofDays(period.days)
            }
        }

        private fun moreThenOneUnit(period: java.time.Period): Boolean {
            val oneField = 1
            return listOf(
                period.days,
                period.months,
                period.years
            ).fold(0) { a, b -> (a + abs(sign(b.toDouble()))).toInt() } > oneField
        }

        private const val noAmount = 0
        private const val daysInWeek = 7
    }
}
