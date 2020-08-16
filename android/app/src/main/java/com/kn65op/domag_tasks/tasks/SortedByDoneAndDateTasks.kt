package com.kn65op.domag_tasks.tasks

class SortedByDoneAndDateTasks(
    t: List<Task> = emptyList()
) : Tasks {
    override var tasks: List<Task> = emptyList()
        set(value) {
            field = value.sortedWith(
                comparator = compareBy({ it.done },
                    { it.nextDeadline },
                    { it.summary })
            )
        }

    init {
        tasks = t
    }
}