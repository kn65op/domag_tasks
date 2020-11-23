package io.github.kn65op.domag_tasks.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test

class NoDeadlineTaskTest {
    val summary = "Some task"
    val task = NoDeadlineTask(summary)

    @Test
    fun `serializeToString should serialize to Json`() {
        val expectedText =
            """{"summary":"$summary","id":0}"""
        assertThat(task.serializeToString(), equalTo(expectedText))
    }

    @Test
    fun `serializeToString should serialize done task to Json`() {
        val expectedText =
            """{"summary":"$summary","id":0,"done":true}"""
        val taskDone = NoDeadlineTask(summary, done = true)
        assertThat(taskDone.serializeToString(), equalTo(expectedText))
    }
}

