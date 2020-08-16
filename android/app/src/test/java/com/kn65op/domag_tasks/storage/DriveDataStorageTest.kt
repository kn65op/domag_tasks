package com.kn65op.domag_tasks.storage

import com.kn65op.domag_tasks.tasks.Id
import com.kn65op.domag_tasks.tasks.Task
import com.kn65op.domag_tasks.tasks.TasksDeserializer
import com.kn65op.domag_tasks.utils.filesystem.FileIo
import com.kn65op.domag_tasks.utils.filesystem.FileIoError
import com.kn65op.domag_tasks.utils.platform.PlatformWrapper
import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import com.natpryce.hamkrest.has
import com.nhaarman.mockitokotlin2.*
import org.junit.Test
import java.io.File
import kotlin.test.assertFailsWith

open class DriveDataStorageBaseFixture {
    private val filesDir = File("Directory/Internal")
    protected val platform: PlatformWrapper = mock()
    protected val fileIo: FileIo = mock()
    protected val taskDeserializer: TasksDeserializer = mock()
    protected val storage = DriveDataStorage(filesDir, platform, fileIo, taskDeserializer)

    protected val task: Task = mock()
    protected val taskFromFile: Task = mock()
    private val firstId: Id = 3
    protected val secondId: Id = 8
    protected val oneTime = 1
    protected val oneItem = 1
    protected val serializedData = "Some data"
    private val tasksFile = "Tasks.txt"
    private val taskType = "TASK TYPE"
    protected val versionText = """
        |version: 1
        |
        |
    """.trimMargin()
    protected val taskSeparator = """
        |
        |
        |========== $taskType
        |
        |
    """.trimMargin()
    protected val taskFile = File(filesDir, tasksFile)
    protected val taskData = "Task data"
    protected val taskDataFromFile = """$taskType
        |
        |Task data""".trimMargin()
    protected val secondTaskDataFromFile = """$taskType
        |
        |$serializedData""".trimMargin()
    protected val fileContentWithOneTask = versionText + taskSeparator + taskData
    private val someText = "someText"
    private val done = true
    private val notDone = false

    init {
        whenever(task.id).thenReturn(firstId)
        whenever(task.type).thenReturn(taskType)
        whenever(task.done).thenReturn(notDone)
        whenever(taskFromFile.type).thenReturn(taskType)
        whenever(taskFromFile.id).thenReturn(secondId)
        whenever(taskFromFile.done).thenReturn(done)
        whenever(platform.getString(any())).thenReturn(someText)
    }
}

class DriveDataStorageTest : DriveDataStorageBaseFixture() {
    @Test
    fun `Task should be serialized with version`() {
        whenever(task.serializeToString()).thenReturn(serializedData)

        storage.store(task)

        verify(task, times(oneTime)).serializeToString()
        val textToWrite = "$versionText$taskSeparator$serializedData"
        verify(fileIo, times(oneTime)).writeToFile(taskFile, textToWrite)
    }

    @Test
    fun `When task has id not 0 StoreTask should return the same task`() {
        whenever(task.serializeToString()).thenReturn(serializedData)

        storage.store(task)

        verify(task, times(0)).id = 1
    }

    @Test
    fun `When task has id 0 Store task should return task with id set`() {
        whenever(task.serializeToString()).thenReturn(serializedData)
        whenever(task.id).thenReturn(0)

        storage.store(task)

        verify(task, times(oneItem)).id = 1
    }

    @Test
    fun `Should throw when unable to process IO file`() {
        whenever(task.serializeToString()).thenReturn(serializedData)
        whenever(fileIo.writeToFile(any(), any())).then { throw FileIoError("UT error") }

        assertFailsWith<UnableToStoreTask> { storage.store(task) }
    }

    @Test
    fun `Should return empty tasks when file does not exist`() {
        whenever(fileIo.exists(taskFile)).thenReturn(false)

        assertThat(storage.loadTasks().tasks, equalTo(emptyList()))
    }

    @Test
    fun `Should write only version when clearing tasks`() {
        storage.clearTasks()

        verify(fileIo, times(oneTime)).writeToFile(taskFile, versionText)
    }
}

class DriveDataStorageTestWhenFileExists : DriveDataStorageBaseFixture() {
    init {
        whenever(fileIo.exists(taskFile)).thenReturn(true)
    }

    @Test
    fun `When unable to parse file should remove file content`() {
        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileContentWithOneTask)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).then { throw FileIoError("UT error") }

        storage.loadTasks()

        verify(fileIo, times(oneTime)).writeToFile(taskFile, versionText)
        verify(platform, times(oneTime)).showToast(any())
    }

    @Test
    fun `Should return empty tasks when file has only version`() {
        val onlyVersion = versionText
        whenever(fileIo.readFromFile(taskFile)).thenReturn(onlyVersion)

        assertThat(storage.loadTasks().tasks, equalTo(emptyList()))

        verify(fileIo, times(oneTime)).readFromFile(taskFile)
    }

    @Test
    fun `Should throw unable to load task when loading failed`() {
        whenever(fileIo.readFromFile(any())).then { throw FileIoError("UT error") }

        assertFailsWith<UnableToLoadTasks> { storage.loadTasks() }
    }

    @Test
    fun `Should return one task when there is one in file`() {
        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileContentWithOneTask)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(task)

        val tasks = storage.loadTasks().tasks
        assertThat(tasks, has(List<Task>::size, equalTo(oneItem)))
        assertThat(tasks, equalTo(listOf(task)))
    }

    @Test
    fun `Should add another task to existing tasks on store task`() {
        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileContentWithOneTask)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(taskFromFile)
        whenever(taskFromFile.serializeToString()).thenReturn(taskData)
        whenever(task.serializeToString()).thenReturn(serializedData)
        val textToWrite = "$versionText$taskSeparator$taskData$taskSeparator$serializedData"

        storage.store(task)

        verify(fileIo, times(oneTime)).writeToFile(taskFile, textToWrite)
        verify(taskDeserializer, times(oneTime)).deserializeTask(taskDataFromFile)
    }

    @Test
    fun `Should use biggest id as base id when adding new task`() {
        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileContentWithOneTask)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(taskFromFile)
        whenever(taskFromFile.serializeToString()).thenReturn(taskData)
        whenever(task.serializeToString()).thenReturn(serializedData)
        whenever(task.id).thenReturn(0)

        storage.store(task)

        verify(task, times(oneTime)).id = secondId + 1
    }

    @Test
    fun `Should replace task if task with the same id already exists`() {
        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileContentWithOneTask)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(taskFromFile)
        whenever(taskFromFile.serializeToString()).thenReturn(taskData)
        val textToWrite = "$versionText$taskSeparator$taskData"

        storage.store(taskFromFile)

        verify(fileIo, times(oneTime)).writeToFile(taskFile, textToWrite)
    }

    @Test
    fun `Should replace task if task with the same id already exists and is not first in list`() {
        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileContentWithOneTask)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(taskFromFile)
        whenever(taskFromFile.serializeToString()).thenReturn(taskData)
        val textToWrite = "$versionText$taskSeparator$taskData"

        storage.store(taskFromFile)

        verify(fileIo, times(oneTime)).writeToFile(taskFile, textToWrite)
    }

    @Test
    fun `Should replace task if task with the same id already exists and is not first`() {
        val fileText = fileContentWithOneTask + taskSeparator + serializedData

        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileText)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(taskFromFile)
        whenever(taskDeserializer.deserializeTask(secondTaskDataFromFile)).thenReturn(task)
        whenever(taskFromFile.serializeToString()).thenReturn(taskData)
        whenever(task.serializeToString()).thenReturn(serializedData)

        storage.store(task)

        verify(fileIo, times(oneTime)).writeToFile(taskFile, fileText)
    }

    @Test
    fun `Should remove task`() {
        val fileText = fileContentWithOneTask + taskSeparator + serializedData

        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileText)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(taskFromFile)
        whenever(taskDeserializer.deserializeTask(secondTaskDataFromFile)).thenReturn(task)
        whenever(task.serializeToString()).thenReturn(taskData)

        storage.remove(taskFromFile)

        val expectedFileText = fileContentWithOneTask
        verify(fileIo, times(oneTime)).writeToFile(taskFile, expectedFileText)
    }

    @Test
    fun `Should remove done tasks`() {
        val fileText = fileContentWithOneTask + taskSeparator + serializedData
        whenever(fileIo.readFromFile(taskFile)).thenReturn(fileText)
        whenever(taskDeserializer.deserializeTask(taskDataFromFile)).thenReturn(taskFromFile)
        whenever(taskDeserializer.deserializeTask(secondTaskDataFromFile)).thenReturn(task)
        whenever(task.serializeToString()).thenReturn(taskData)

        storage.removeDoneTasks()

        val expectedFileText = fileContentWithOneTask
        verify(fileIo, times(oneTime)).writeToFile(taskFile, expectedFileText)
    }
}