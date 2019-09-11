package com.example.domag.tasks

import kotlinx.serialization.json.Json

class JsonTaskDeserializer : TasksDeserializer {
    override fun deserializeTask(rawData: String): Task {
        val data = rawData.trimStart()
        if (data.startsWith(SimpleTask.type)) {
            return Json.parse(SimpleTask.serializer(), data.substring(SimpleTask.type.length))
        } else if (data.startsWith(RecurringTask.type)) {
            return Json.parse(RecurringTask.serializer(), data.substring(RecurringTask.type.length))
        } else if (data.startsWith(NoDeadlineTask.type)) {
            return Json.parse(

                NoDeadlineTask.serializer(),
                data.substring(NoDeadlineTask.type.length)
            )
        }
        throw UnknownTask(data)
    }

    class UnknownTask(message: String) : Exception(message)
}