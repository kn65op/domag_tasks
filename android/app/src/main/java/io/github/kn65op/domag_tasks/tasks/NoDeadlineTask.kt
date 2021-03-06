package io.github.kn65op.domag_tasks.tasks

import io.github.kn65op.domag_tasks.utils.platform.localization.Localization
import kotlinx.serialization.Required
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import java.time.ZonedDateTime

@Serializable
class NoDeadlineTask(
    override var summary: String,
    @Required
    override var id: Id = 0,
    override var done: Boolean = false
) : Task {
    companion object {
        const val type = "NO DEADLINE TASK"
    }

    override val type
        get() = Companion.type

    override var nextDeadline: ZonedDateTime?
        get() = null
        set(_) {}

    override fun nextDeadlineText(localization: Localization) = "No deadline"

    override fun serializeToString(): String = Json.encodeToString(serializer(), this)

    override fun toString(): String {
        return "$type($id): [$done] $summary - no deadline"
    }

    override fun equals(other: Any?): Boolean {
        if (other is NoDeadlineTask) {
            return other.id == id
                    && other.summary == summary
                    && other.done == done
        }
        return false
    }

    override fun hashCode(): Int {
        var result = summary.hashCode()
        result = 31 * result + id
        result = 31 * result + done.hashCode()
        return result
    }
}