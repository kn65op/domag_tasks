package com.example.domag.utils.time

class Period(val type: PeriodType, val count: Int) {

    companion object {
        fun ofYears(number: Int) = Period(PeriodType.Year, number)
        fun ofMonths(number: Int) = Period(PeriodType.Month, number)
        fun ofWeeks(number: Int) = Period(PeriodType.Week, number)
    }
}