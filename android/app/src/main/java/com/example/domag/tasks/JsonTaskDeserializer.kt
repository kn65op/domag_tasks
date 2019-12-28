package com.example.domag.tasks

import kotlinx.serialization.json.Json

class JsonTaskDeserializer : TasksDeserializer {
    override fun deserializeTask(rawData: String): Task {
        val data = rawData.trimStart()
        when {
            data.startsWith(SimpleTask.type) -> {
                return Json.parse(SimpleTask.serializer(), data.substring(SimpleTask.type.length))
            }
            data.startsWith(RecurringTask.type) -> {
                return Json.parse(RecurringTask.serializer(), data.substring(RecurringTask.type.length))
            }
            data.startsWith(NoDeadlineTask.type) -> {
                return Json.parse(
                    NoDeadlineTask.serializer(),
                    data.substring(NoDeadlineTask.type.length)
                )
            }
            else -> throw UnknownTask(data)
        }
    }

    class UnknownTask(message: String) : Exception(message)
}