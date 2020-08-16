package com.kn65op.domag_tasks.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import com.natpryce.hamkrest.lessThan
import com.nhaarman.mockitokotlin2.mock
import com.nhaarman.mockitokotlin2.whenever
import org.junit.Test
import java.time.ZoneId
import java.time.ZonedDateTime

class SortedByDoneAndDateTasksTest {
    private val initiallyEmptyTasks = SortedByDoneAndDateTasks(emptyList())

    private val firstDate = ZonedDateTime.of(
        2012,
        12,
        3,
        10,
        15,
        40,
        0,
        ZoneId.of("Europe/Paris")
    )
    private val secondDate = firstDate.withYear(2013)
    private val thirdDate = firstDate.withYear(2015)
    private val earlierDate = firstDate.withYear(2010)

    private val firstSummary = "A"
    private val secondSummary = "B"

    private val earliestTask: Task = mock()
    private val middleTaskWithFirstSummary: Task = mock()
    private val middleTaskWithSecondSummary: Task = mock()
    private val latestTask: Task = mock()
    private val earliestDoneTask: Task = mock()

    private val unsortedTasks =
        listOf(
            earliestDoneTask,
            latestTask,
            middleTaskWithSecondSummary,
            middleTaskWithFirstSummary,
            earliestTask
        )
    private val sortedTasks = listOf(
        earliestTask,
        middleTaskWithFirstSummary,
        middleTaskWithSecondSummary,
        latestTask,
        earliestDoneTask
    )

    private val notDone = false
    private val done = true

    init {
        whenever(earliestTask.nextDeadline).thenReturn(firstDate)
        whenever(middleTaskWithFirstSummary.nextDeadline).thenReturn(secondDate)
        whenever(middleTaskWithSecondSummary.nextDeadline).thenReturn(secondDate)
        whenever(latestTask.nextDeadline).thenReturn(thirdDate)
        whenever(earliestDoneTask.nextDeadline).thenReturn(earlierDate)

        whenever(earliestTask.summary).thenReturn(secondSummary)
        whenever(middleTaskWithFirstSummary.summary).thenReturn(firstSummary)
        whenever(middleTaskWithSecondSummary.summary).thenReturn(secondSummary)
        whenever(latestTask.summary).thenReturn(firstSummary)
        whenever(earliestDoneTask.summary).thenReturn(firstSummary)

        whenever(earliestTask.done).thenReturn(notDone)
        whenever(middleTaskWithFirstSummary.done).thenReturn(notDone)
        whenever(middleTaskWithSecondSummary.done).thenReturn(notDone)
        whenever(latestTask.done).thenReturn(notDone)
        whenever(earliestDoneTask.done).thenReturn(done)
    }

    @Test
    fun `Compare dates`() {
        assertThat(firstDate, lessThan(secondDate))
        assertThat(secondDate, lessThan(thirdDate))
    }

    @Test
    fun `Should return empty tasks`() {
        assertThat(initiallyEmptyTasks.tasks, equalTo(emptyList()))
    }

    @Test
    fun `Should return sorted tasks when created with many tasks`() {
        val tasks = SortedByDoneAndDateTasks(unsortedTasks)

        assertThat(tasks.tasks, equalTo(sortedTasks))
    }

    @Test
    fun `Should sort tasks when set`() {
        initiallyEmptyTasks.tasks = unsortedTasks

        assertThat(initiallyEmptyTasks.tasks, equalTo(sortedTasks))
    }
}