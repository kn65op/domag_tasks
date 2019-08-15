package com.example.domag.utils.time

class Period(val type: PeriodType) {

    companion object {
        fun ofYears() = Period(PeriodType.Year)
    }
}