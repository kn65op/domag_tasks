package com.example.domag.tasks

import java.time.Period
import java.time.ZonedDateTime

class RecurringTask(
    override var summary: String,
    firstDeadline: ZonedDateTime = ZonedDateTime.now(),
    private val period: Period,
    override var id: Id = 0
) : Task {
    companion object {
        const val type = "RECURRING TASK"
    }

    override var nextDeadline: ZonedDateTime = firstDeadline
    override val type
        get() = Companion.type
    override var done: Boolean
        get() = false
        set(value) { if (value) nextDeadline = ZonedDateTime.now().plus(period)}

    override fun serializeToString(): String {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }
}