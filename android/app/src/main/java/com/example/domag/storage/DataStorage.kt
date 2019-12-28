package com.example.domag.storage

import com.example.domag.tasks.Task
import com.example.domag.tasks.Tasks

class UnableToStoreTask(message: String) : Exception(message)
class UnableToLoadTasks(message: String) : Exception(message)

interface DataStorage {
    fun store(task: Task)
    fun loadTasks(): Tasks
    fun clearTasks()
    fun remove(task: Task)
    fun removeDoneTasks()
}