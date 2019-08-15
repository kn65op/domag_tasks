package com.example.domag.utils.time

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test

class PeriodTest {
    val number = 3

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
}