package com.example.domag.utils.time

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test

class PeriodTest {
    private val number = 3

    @Test
    fun `year period should be type of Year`() {
        assertThat(Period.ofYears(number).type, equalTo(PeriodType.Year))
    }

    @Test
    fun `year period should store number of years`() {
        assertThat(Period.ofYears(number).count, equalTo(number))
    }

    @Test
    fun `month period should be type of Month`() {
        assertThat(Period.ofMonths(number).type, equalTo(PeriodType.Month))
    }

    @Test
    fun `month period should store number of months`() {
        assertThat(Period.ofMonths(number).count, equalTo(number))
    }

    @Test
    fun `week period should be type of Week`() {
        assertThat(Period.ofWeeks(number).type, equalTo(PeriodType.Week))
    }

    @Test
    fun `week period should store number of weeks`() {
        assertThat(Period.ofWeeks(number).count, equalTo(number))
    }

    @Test
    fun `day period should be type of Days`() {
        assertThat(Period.ofDays(number).type, equalTo(PeriodType.Day))
    }

    @Test
    fun `day period should store number of days`() {
        assertThat(Period.ofDays(number).count, equalTo(number))
    }
}
