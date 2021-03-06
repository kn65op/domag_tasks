package io.github.kn65op.domag_tasks.storage

import android.util.Log
import io.github.kn65op.domag_tasks.R
import io.github.kn65op.domag_tasks.tasks.*
import io.github.kn65op.domag_tasks.utils.filesystem.FileIo
import io.github.kn65op.domag_tasks.utils.filesystem.FileIoError
import io.github.kn65op.domag_tasks.utils.filesystem.FileIoImpl
import io.github.kn65op.domag_tasks.utils.platform.PlatformWrapper
import io.github.kn65op.domag_tasks.tasks.*
import java.io.File

class DriveDataStorage(
    private val storageDirectory: File,
    private val platform: PlatformWrapper,
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
        get() = File(storageDirectory, "Tasks.txt")

    override fun store(task: Task) {
        Log.i(TAG, "Storing $task")
        val currentTasks = loadTasks()
        updateIdIfNeeded(task)
        Log.i(TAG, "Storing $task")
        val tasks = tasksWithoutTask(currentTasks, task) + task
        storeTasks(tasks)
        Log.i(TAG, "Store completed: ${tasks.size}.")
    }

    private fun storeTasks(
        tasks: List<Task>
    ) {
        Log.i(TAG, "Storing ${tasks.size}")
        val serializedTasks =
            tasks.fold(String()) { acc, currentTask -> acc + taskSeparator(currentTask.type) + currentTask.serializeToString() }
        val dataToStore = version + serializedTasks
        storeData(dataToStore)
    }

    private fun tasksWithoutTask(
        currentTasks: Tasks,
        task: Task
    ) = currentTasks.tasks.filter { it.id != task.id }

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

    override fun loadTasks(): Tasks {
        val dataFromFile = readFromFile()
        val fileData = dataFromFile.split(taskSeparatorText)
        val versionSize = 1
        val tasksData = fileData.drop(versionSize)
        return try {
            val tasks = tasksData.flatMap { listOf(taskDeserializer.deserializeTask(it.trim())) }
            tasks.forEach { if (it.id > maxTaskId) maxTaskId = it.id }
            SortedByDoneAndDateTasks(tasks)
        } catch (e: Exception) {
            Log.e(TAG, platform.getString(R.string.task_file_broken))
            platform.showToast(platform.getString(R.string.task_file_broken))
            clearTasks()
            SortedByDoneAndDateTasks()
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

    override fun remove(task: Task) {
        Log.i(TAG, "Removing $task")
        storeTasks(tasksWithoutTask(loadTasks(), task))
    }

    override fun removeDoneTasks() {
        storeTasks(loadTasks().filterOutDone())
    }

    companion object {
        const val TAG = "DriveDataStorage"
    }
}

