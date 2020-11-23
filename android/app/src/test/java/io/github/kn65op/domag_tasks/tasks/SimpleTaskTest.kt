package io.github.kn65op.domag_tasks.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test
import java.time.ZoneId
import java.time.ZonedDateTime

class SimpleTaskTest {
    val summary = "Some task"
    private val date = ZonedDateTime.of(
        2011,
        12,
        3,
        10,
        15,
        30,
        0,
        ZoneId.of("+01:00")
    )
    val task = SimpleTask(summary, date)

    @Test
    fun `serializeToString should serialize to Json`() {
        val expectedText =
            """{"summary":"$summary","nextDeadline":"2011-12-03T10:15:30+01:00","id":0}"""
        assertThat(task.serializeToString(), equalTo(expectedText))
    }

    @Test
    fun `serializeToString should serialize done task to Json`() {
        val expectedText =
            """{"summary":"$summary","nextDeadline":"2011-12-03T10:15:30+01:00","id":2,"done":true}"""
        val taskDone = SimpleTask(summary, date, 2, true)
        assertThat(taskDone.serializeToString(), equalTo(expectedText))
    }
}

