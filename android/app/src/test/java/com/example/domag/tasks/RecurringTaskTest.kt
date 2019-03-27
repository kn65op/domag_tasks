package com.example.domag.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test

class RecurringTaskTest {
    private val summary = "Summary"

    private val task = RecurringTask(summary)
    @Test
    fun `should has proper type`() {
        assertThat(task.type, equalTo("RECURRING TASK"))
    }

    @Test
    fun `should always be not done` () {
        val notDone = false

        assertThat(task.done, equalTo(notDone))
    }
}