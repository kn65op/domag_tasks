package com.example.domag.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import org.junit.Test
import java.time.ZoneId
import java.time.ZonedDateTime
import kotlin.test.assertFailsWith

class JsonTaskDeserializerTest {
    private val deserializer = JsonTaskDeserializer()

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
            |name: $fineTask
            |"nextDeadline":"2011-12-03T10:15:30+01:00"
            |}
            |
        """.trimMargin()
        val expectedTask = SimpleTask(
            fineTask,
            ZonedDateTime.of(
                2012,
                12,
                3,
                10,
                15,
                40,
                0,
                ZoneId.of("Europe/Paris")
            ),
            id
        )
        assertThat(deserializer.deserializeTask(taskData), equalTo<Task>(expectedTask))
    }
}
