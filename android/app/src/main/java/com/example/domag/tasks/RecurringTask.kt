package com.example.domag.tasks

import java.time.ZonedDateTime

class RecurringTask(
    override var summary: String,
    override var nextDeadline: ZonedDateTime = ZonedDateTime.now(),
    override var id: Id = 0,
    override var done: Boolean = false
) : Task {
    companion object {
        const val type = "RECURRING TASK"
    }

    override val type
        get() = Companion.type

    override fun serializeToString(): String {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }
}