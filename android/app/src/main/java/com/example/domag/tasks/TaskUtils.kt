package com.example.domag.tasks

import java.time.LocalDate

fun Tasks.getTodayNotDone(): List<Task> {
    val notDone = false
    val today = LocalDate.now()
    return tasks.filter {
        it.nextDeadline?.toLocalDate() == today && it.done == notDone
    }
}

fun Tasks.getOverdueNotDone(): List<Task> {
    val today = LocalDate.now()
    val notDone = false
    return tasks.filter fil@{
        (it.nextDeadline?.toLocalDate() ?: return@fil false) < today && it.done == notDone
    }
}

fun Tasks.filterOutDone(): List<Task> {
    val done = true
    return tasks.filter { it.done != done }
}

fun Tasks.filterHasDeadline(): List<Task> =
    tasks.filter { it.nextDeadline != null }

fun Tasks.filterNoDeadline(): List<Task> =
    tasks.filter { it.nextDeadline == null }
