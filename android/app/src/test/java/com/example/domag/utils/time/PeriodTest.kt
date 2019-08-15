package com.example.domag.utils.time

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test

class PeriodTest {
    @Test
    fun `empty period should be type of None`()
    {
        assertThat(Period().type, equalTo(PeriodType.None))
    }
}