package com.example.domag.tasks

class SortedByDoneAndDateTasks(
    t: List<Task> = emptyList()
) : Tasks {
    override var tasks: List<Task> = emptyList()
        set(value) {
            field = value.sortedWith(comparator = compareBy({ it.nextDeadline }, { it.summary }))
        }

    init {
        tasks = t
    }
}