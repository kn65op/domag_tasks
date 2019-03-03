package com.example.domag.tasks

import com.example.domag.utils.ZoneDateTimeSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.Transient
import kotlinx.serialization.json.Json
import java.time.ZonedDateTime

@Serializable
class SimpleTask(
    override var name: String,
    @Serializable(with= ZoneDateTimeSerializer::class) override val nextDeadline: ZonedDateTime = ZonedDateTime.now(),
    override var id: Id = 0
) : Task {
    companion object {
        const val type = "SIMPLE TASK"
    }

    @Transient
    override val type
        get() = Companion.type

    override fun serializeToString(): String = Json.stringify(serializer(), this)

    override fun toString(): String {
        return "$type($id): $name"
    }

    override fun equals(other: Any?): Boolean {
        if (other is SimpleTask) {
            return other.id == id && other.name == name
        }
        return false
    }

    override fun hashCode(): Int {
        var result = name.hashCode()
        result = 31 * result + id
        return result
    }
}