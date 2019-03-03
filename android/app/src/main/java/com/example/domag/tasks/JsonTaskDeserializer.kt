package com.example.domag.tasks

import kotlinx.serialization.json.Json

class JsonTaskDeserializer : TasksDeserializer {
    override fun deserializeTask(rawData: String): Task {
        val data = rawData.trimStart()
        if (data.startsWith(SimpleTask.type))
        {
                return Json.parse(SimpleTask.serializer(), data.substring(SimpleTask.type.length))
        }
        throw UnknownTask(data)
    }

    class UnknownTask(message: String) :Exception(message)
}