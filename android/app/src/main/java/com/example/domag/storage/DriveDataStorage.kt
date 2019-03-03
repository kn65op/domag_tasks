package com.example.domag.storage

import android.content.Context
import android.util.Log
import com.example.domag.R
import com.example.domag.tasks.Id
import com.example.domag.tasks.Task
import com.example.domag.tasks.TasksDeserializer
import com.example.domag.utils.filesystem.FileIo
import com.example.domag.utils.filesystem.FileIoError
import com.example.domag.utils.filesystem.FileIoImpl
import com.example.domag.utils.platform.PlatformWrapper
import java.io.File

class DriveDataStorage(
    private val androidContext: Context,
    private val platform : PlatformWrapper,
    private val fileIo: FileIo = FileIoImpl(),
    private val taskDeserializer: TasksDeserializer
) : DataStorage {
    private val version = """
        |version: 1
        |
        |
    """.trimMargin()
    private val taskSeparatorText = "=========="

    private val tasksFile: File
        get() = File(androidContext.filesDir, "Tasks.txt")

    override fun store(task: Task) {
        val currentTasks = loadTasks()
        updateIdIfNeeded(task)
        val tasks = currentTasks.dropWhile { it.id == task.id } + task
        val serializedTasks =
            tasks.fold(String()) { acc, currentTask -> acc + taskSeparator(task.type) + currentTask.serializeToString() }
        val dataToStore = version + serializedTasks
        storeData(dataToStore)
    }

    private fun updateIdIfNeeded(task: Task) {
        if (task.id == 0) {
            task.id = getNextTaskId()
        }
    }

    private var maxTaskId = 0

    private fun getNextTaskId(): Id {
        return ++maxTaskId
    }

    private fun taskSeparator(taskType: String) = """
        |
        |
        |$taskSeparatorText $taskType
        |
        |
    """.trimMargin()

    private fun storeData(dataToStore: String) = try {
        fileIo.writeToFile(tasksFile, dataToStore)
    } catch (e: FileIoError) {
        throw UnableToStoreTask("Unable to store tasks: ${e.message}")
    }

    override fun loadTasks(): List<Task> {
        val dataFromFile = readFromFile()
        val fileData = dataFromFile.split(taskSeparatorText)
        val versionSize = 1
        val tasksData = fileData.drop(versionSize)
        try {
            val tasks = tasksData.flatMap { listOf(taskDeserializer.deserializeTask(it.trim())) }
            tasks.forEach { if (it.id > maxTaskId) maxTaskId = it.id }
            return tasks
        } catch (e: Exception) {
            Log.i(TAG, platform.getString(R.string.task_file_broken))
            platform.showToast(platform.getString(R.string.task_file_broken))
            clearTasks();
            return emptyList()
        }
    }

    private fun readFromFile() =
        if (fileIo.exists(tasksFile)) {
            try {
                fileIo.readFromFile(tasksFile)
            } catch (e: FileIoError) {
                throw UnableToLoadTasks("Unable to load tasks: ${e.message}")
            }
        } else {
            String()
        }

    override fun clearTasks() {
        storeData(version)
    }

    companion object {
        const val TAG = "DriveDataStorage"
    }
}