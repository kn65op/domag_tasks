package com.example.domag.storage

import com.example.domag.tasks.Task

class UnableToStoreTask(message: String) : Exception(message)
class UnableToLoadTasks(message: String) : Exception(message)

interface DataStorage {
    fun store(task: Task)
    fun loadTasks(): List<Task>
    fun clearTasks()
}