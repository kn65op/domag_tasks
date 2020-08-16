package io.github.kn65op.domag_tasks.tasks

interface TasksDeserializer {
    fun deserializeTask(rawData: String): Task
}