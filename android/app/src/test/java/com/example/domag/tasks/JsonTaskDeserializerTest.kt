package com.example.domag.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test
import java.time.ZoneId
import java.time.ZonedDateTime
import kotlin.test.assertFailsWith

class JsonTaskDeserializerTest {
    private val deserializer = JsonTaskDeserializer()
    private val expectedNextDeadline = ZonedDateTime.of(
        2012,
        12,
        3,
        10,
        15,
        40,
        0,
        ZoneId.of("Europe/Paris")
    )

    @Test
    fun `Given unknown task should throw`() {
        assertFailsWith<JsonTaskDeserializer.UnknownTask> { deserializer.deserializeTask("EMPTY TASK") }
    }

    @Test
    fun `Given Simple task should deserialize`() {
        val id = 8
        val fineTask = "fineTask"
        val taskData = """|SIMPLE TASK
            |{id: $id,
            |done: false
            |summary: $fineTask
            |"nextDeadline":"2011-12-03T10:15:30+01:00"
            |}
            |
        """.trimMargin()

        val expectedTask = SimpleTask(
            fineTask,
            expectedNextDeadline,
            id
        )
        assertThat(deserializer.deserializeTask(taskData), equalTo<Task>(expectedTask))
    }

    @Test
    fun `Given Simple task with other data should deserialize`() {
        val id = 7
        val fineTask = "another"
        val taskData = """|SIMPLE TASK
            |{id: $id,
            |done: true
            |summary: $fineTask
            |"nextDeadline":"2011-12-03T10:15:30+01:00"
            |}
            |
        """.trimMargin()

        val expectedTask = SimpleTask(
            summary = fineTask,
            nextDeadline = expectedNextDeadline,
            id = id,
            done = true
        )
        assertThat(deserializer.deserializeTask(taskData), equalTo<Task>(expectedTask))
    }
}
