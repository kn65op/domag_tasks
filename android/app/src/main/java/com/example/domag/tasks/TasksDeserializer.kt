package com.example.domag.tasks

interface TasksDeserializer {
    fun deserializeTask(rawData: String): Task
}