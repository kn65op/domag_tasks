package io.github.kn65op.domag_tasks.tasks

import io.github.kn65op.domag_tasks.utils.platform.localization.Localization
import io.github.kn65op.domag_tasks.utils.serializer.ZoneDateTimeWithoutZoneChangeSerializer
import kotlinx.serialization.Required
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import java.time.ZonedDateTime

@Serializable
class SimpleTask(
    override var summary: String,
    @Serializable(with = ZoneDateTimeWithoutZoneChangeSerializer::class) override var nextDeadline: ZonedDateTime? = ZonedDateTime.now(),
    @Required
    override var id: Id = 0,
    override var done: Boolean = false
) : Task {
    companion object {
        const val type = "SIMPLE TASK"
    }

    override val type
        get() = Companion.type

    override fun nextDeadlineText(localization: Localization) =
        "${nextDeadline?.format(taskTimeFormat)}"

    override fun serializeToString(): String = Json.encodeToString(serializer(), this)

    override fun toString(): String {
        return "$type($id): [$done] $summary - $nextDeadline"
    }

    override fun equals(other: Any?): Boolean {
        if (other is SimpleTask) {
            return other.id == id
                    && other.summary == summary
                    && other.done == done
                    && nextDeadline == other.nextDeadline
        }
        return false
    }

    override fun hashCode(): Int {
        var result = summary.hashCode()
        result = 31 * result + nextDeadline.hashCode()
        result = 31 * result + id
        result = 31 * result + done.hashCode()
        return result
    }
}