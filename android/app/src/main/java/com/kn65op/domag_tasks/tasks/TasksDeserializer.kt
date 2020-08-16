package com.kn65op.domag_tasks.tasks

interface TasksDeserializer {
    fun deserializeTask(rawData: String): Task
}