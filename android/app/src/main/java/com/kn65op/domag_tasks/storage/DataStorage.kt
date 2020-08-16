package com.kn65op.domag_tasks.storage

import com.kn65op.domag_tasks.tasks.Task
import com.kn65op.domag_tasks.tasks.Tasks

class UnableToStoreTask(message: String) : Exception(message)
class UnableToLoadTasks(message: String) : Exception(message)

interface DataStorage {
    fun store(task: Task)
    fun loadTasks(): Tasks
    fun clearTasks()
    fun remove(task: Task)
    fun removeDoneTasks()
}