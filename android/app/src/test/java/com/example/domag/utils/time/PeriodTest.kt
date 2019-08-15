package com.example.domag.utils.time

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test

class PeriodTest {
    @Test
    fun `year period shuold be type of Year`() {
        assertThat(Period.ofYears().type, equalTo(PeriodType.Year))
    }
}