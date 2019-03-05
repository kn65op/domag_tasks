package com.example.domag.tasks

import com.example.domag.utils.ZoneDataTimeWithoutZoneChangeSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.Transient
import kotlinx.serialization.json.Json
import java.time.ZonedDateTime

@Serializable
class SimpleTask(
    override val summary: String,
    @Serializable(with= ZoneDataTimeWithoutZoneChangeSerializer::class) override val nextDeadline: ZonedDateTime = ZonedDateTime.now(),
    override var id: Id = 0,
    override val done : Boolean = false
) : Task {
    companion object {
        const val type = "SIMPLE TASK"
    }

    @Transient
    override val type
        get() = Companion.type

    override fun serializeToString(): String = Json.stringify(serializer(), this)

    override fun toString(): String {
        return "$type($id): $summary"
    }

    override fun equals(other: Any?): Boolean {
        if (other is SimpleTask) {
            return other.id == id && other.summary == summary
        }
        return false
    }

    override fun hashCode(): Int {
        var result = summary.hashCode()
        result = 31 * result + id
        return result
    }
}