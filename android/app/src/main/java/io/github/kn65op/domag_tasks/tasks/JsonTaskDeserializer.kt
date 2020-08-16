package io.github.kn65op.domag_tasks.tasks

import kotlinx.serialization.json.Json
import kotlinx.serialization.json.JsonConfiguration

class JsonTaskDeserializer : TasksDeserializer {
    override fun deserializeTask(rawData: String): Task {
        val data = rawData.trimStart()
        return when {
            data.startsWith(SimpleTask.type) -> {
                json.parse(SimpleTask.serializer(), data.substring(SimpleTask.type.length))
            }
            data.startsWith(RecurringTask.type) -> {
                json.parse(
                    RecurringTask.serializer(),
                    data.substring(RecurringTask.type.length)
                )
            }
            data.startsWith(NoDeadlineTask.type) -> {
                json.parse(
                    NoDeadlineTask.serializer(),
                    data.substring(NoDeadlineTask.type.length)
                )
            }
            else -> throw UnknownTask(data)
        }
    }

    companion object {
        val json = Json(JsonConfiguration.Stable)
    }

    class UnknownTask(message: String) : Exception(message)
}