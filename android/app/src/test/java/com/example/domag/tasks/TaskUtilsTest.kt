package com.example.domag.tasks

import com.natpryce.hamkrest.assertion.assertThat
import com.natpryce.hamkrest.equalTo
import com.nhaarman.mockitokotlin2.mock
import com.nhaarman.mockitokotlin2.whenever
import org.junit.Test
import java.time.ZonedDateTime

class TaskUtilsTest {
    class TestTasks(override var tasks: List<Task>) : Tasks

    val today = ZonedDateTime.now()
    val notDone = false
    val done = true
    val todayNotDoneTask: Task = mock()
    val yesterdayNotDoneTask: Task = mock()
    val todayDoneTask: Task = mock()
    val yesterdayDoneTask: Task = mock()

    init {
        whenever(todayNotDoneTask.nextDeadline).thenReturn(today)
        whenever(todayNotDoneTask.done).thenReturn(notDone)
        whenever(yesterdayNotDoneTask.nextDeadline).thenReturn(today.minusDays(1))
        whenever(yesterdayNotDoneTask.done).thenReturn(notDone)
        whenever(todayDoneTask.nextDeadline).thenReturn(today)
        whenever(todayDoneTask.done).thenReturn(done)
        whenever(yesterdayDoneTask.nextDeadline).thenReturn(today.minusDays(1))
        whenever(yesterdayDoneTask.done).thenReturn(done)
    }

    @Test
    fun `getTodayNotDone given empty list should return empty list`() {
        assertThat(TestTasks(emptyList()).getTodayNotDone(), equalTo(emptyList()))
    }

    @Test
    fun `getTodayNotDone given one today not done task should return it`() {
        assertThat(TestTasks(listOf(todayNotDoneTask)).getTodayNotDone(), equalTo(listOf(todayNotDoneTask)))
    }

    @Test
    fun `getTodayNotDone given one not today not done task should return nothing`() {
        assertThat(TestTasks(listOf(yesterdayNotDoneTask)).getTodayNotDone(), equalTo(emptyList()))
    }

    @Test
    fun `getTodayNotDone given today done task should return nothing`() {
        assertThat(TestTasks(listOf(todayDoneTask)).getTodayNotDone(), equalTo(emptyList()))
    }

    @Test
    fun `getOverdueNotDone given empty list should return empty list`() {
        assertThat(TestTasks(emptyList()).getOverdueNotDone(), equalTo(emptyList()))
    }

    @Test
    fun `getOverdueNotDone given today not done task should return empty list`() {
        assertThat(TestTasks(listOf(todayNotDoneTask)).getOverdueNotDone(), equalTo(emptyList()))
    }

    @Test
    fun `getOverdueNotDone given yesterday not done task should return it`() {
        assertThat(TestTasks(listOf(yesterdayNotDoneTask)).getOverdueNotDone(), equalTo(listOf(yesterdayNotDoneTask)))
    }

    @Test
    fun `getOverdueNotDone given yesterday done task should return empty list`() {
        assertThat(TestTasks(listOf(yesterdayDoneTask)).getOverdueNotDone(), equalTo(emptyList()))
    }
}
