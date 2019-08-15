package com.example.domag.utils.time

class Period(val type: PeriodType, val count: Int) {

    companion object {
        fun ofYears(number: Int) = Period(PeriodType.Year, number)
        fun ofMonths() = Period(PeriodType.Month, 0)
    }
}