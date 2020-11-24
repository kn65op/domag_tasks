package io.github.kn65op.domag_tasks.tasks

import kotlinx.serialization.json.Json

class JsonTaskDeserializer : TasksDeserializer {
    override fun deserializeTask(rawData: String): Task {
        val data = rawData.trimStart()
        return when {
            data.startsWith(SimpleTask.type) -> {
                Json.decodeFromString(
                    SimpleTask.serializer(),
                    data.substring(SimpleTask.type.length)
                )
            }
            data.startsWith(RecurringTask.type) -> {
                Json.decodeFromString(
                    RecurringTask.serializer(),
                    data.substring(RecurringTask.type.length)
                )
            }
            data.startsWith(NoDeadlineTask.type) -> {
                Json.decodeFromString(
                    NoDeadlineTask.serializer(),
                    data.substring(NoDeadlineTask.type.length)
                )
            }
            else -> throw UnknownTask(data)
        }
    }

    class UnknownTask(message: String) : Exception(message)
}