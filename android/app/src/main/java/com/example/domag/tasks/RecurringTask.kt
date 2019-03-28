package com.example.domag.tasks

import com.example.domag.utils.serializer.PeriodSerializer
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
    @Serializable(with = PeriodSerializer::class)
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

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as RecurringTask

        if (summary != other.summary) return false
        if (nextDeadline != other.nextDeadline) return false
        if (period != other.period) return false
        if (id != other.id) return false

        return true
    }

    override fun hashCode(): Int {
        var result = summary.hashCode()
        result = 31 * result + nextDeadline.hashCode()
        result = 31 * result + period.hashCode()
        result = 31 * result + id
        return result
    }
}