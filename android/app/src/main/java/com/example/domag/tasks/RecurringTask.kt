package com.example.domag.tasks

import com.example.domag.utils.serializer.ZoneDateTimeWithoutZoneChangeSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.Transient
import kotlinx.serialization.json.Json
import java.time.Period
import java.time.ZonedDateTime

@Serializable
class RecurringTask(
    override var summary: String,
    @Serializable(with = ZoneDateTimeWithoutZoneChangeSerializer::class)
    override var nextDeadline: ZonedDateTime = ZonedDateTime.now(),
    @Transient
    private val period: Period = Period.of(1,0,0),
    override var id: Id = 0
) : Task {
    companion object {
        const val type = "RECURRING TASK"
    }

    @Transient
    override val type
        get() = Companion.type

    @Transient
    override var done: Boolean
        get() = false
        set(value) {
            if (value) nextDeadline = ZonedDateTime.now().plus(period)
        }

    override fun serializeToString(): String = Json.stringify(serializer(), this)
}