package com.example.domag.utils.time

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import com.natpryce.hamkrest.isA
import com.natpryce.hamkrest.throws
import org.hamcrest.core.IsInstanceOf.instanceOf
import org.junit.Test
import kotlin.test.assertFailsWith

class PeriodTest {
    private val number = 3
    private fun yearsPeriod() = Period.ofYears(number)
    private fun weeksPeriod() = Period.ofWeeks(number)
    private fun daysPeriod() = Period.ofDays(number)
    private fun monthsPeriod() = Period.ofMonths(number)
    private fun javaYearPeriod() = java.time.Period.ofYears(number)
    private fun javaMonthPeriod() = java.time.Period.ofMonths(number)
    private fun javaWeekPeriod() = java.time.Period.ofWeeks(number)
    private fun javaDayPeriod() = java.time.Period.ofDays(number)
    private fun javaOneDayOneMonthPeriod() = java.time.Period.of(0, 1, 1)
    private fun javaOnePositiveOneNegativePeriod() = java.time.Period.of(-1, 1, 0)

    @Test
    fun `year period should be type of Year`() {
        assertThat(yearsPeriod().type, equalTo(PeriodType.Year))
    }

    @Test
    fun `year period should store number of years`() {
        assertThat(yearsPeriod().count, equalTo(number))
    }

    @Test
    fun `month period should be type of Month`() {
        assertThat(monthsPeriod().type, equalTo(PeriodType.Month))
    }

    @Test
    fun `month period should store number of months`() {
        assertThat(monthsPeriod().count, equalTo(number))
    }

    @Test
    fun `week period should be type of Week`() {
        assertThat(weeksPeriod().type, equalTo(PeriodType.Week))
    }

    @Test
    fun `week period should store number of weeks`() {
        assertThat(weeksPeriod().count, equalTo(number))
    }

    @Test
    fun `day period should be type of Days`() {
        assertThat(daysPeriod().type, equalTo(PeriodType.Day))
    }

    @Test
    fun `day period should store number of days`() {
        assertThat(daysPeriod().count, equalTo(number))
    }

    @Test
    fun `year period should be converted to Java Period`() {
        assertThat(yearsPeriod().toJavaPeriod(), equalTo(javaYearPeriod()))
    }

    @Test
    fun `month period should be converted to Java Period`() {
        assertThat(monthsPeriod().toJavaPeriod(), equalTo(javaMonthPeriod()))
    }

    @Test
    fun `weeks period should be converted to Java Period`() {
        assertThat(weeksPeriod().toJavaPeriod(), equalTo(javaWeekPeriod()))
    }

    @Test
    fun `days period should be converted to Java Period`() {
        assertThat(daysPeriod().toJavaPeriod(), equalTo(javaDayPeriod()))
    }

    @Test
    fun `year period should be converted from Java Period`() {
        assertThat(Period.ofJavaPeriod(javaYearPeriod()), equalTo(yearsPeriod()))
    }

    @Test
    fun `month period should be converted from Java Period`() {
        assertThat(Period.ofJavaPeriod(javaMonthPeriod()), equalTo(monthsPeriod()))
    }

    @Test
    fun `week period should be converted from Java Period`() {
        assertThat(Period.ofJavaPeriod(javaWeekPeriod()), equalTo(weeksPeriod()))
    }

    @Test
    fun `day period should be converted from Java Period`() {
        assertThat(Period.ofJavaPeriod(javaDayPeriod()), equalTo(daysPeriod()))
    }

    @Test
    fun `given more then one type of unit in Java Period should throw`() {
        assertFailsWith<Period.InvalidJavaPeriod> { Period.ofJavaPeriod(javaOneDayOneMonthPeriod()) }
    }

    @Test
    fun `given negative and positive values in Java Period should throw`() {
        assertFailsWith<Period.InvalidJavaPeriod> { Period.ofJavaPeriod(javaOnePositiveOneNegativePeriod()) }
    }
}
