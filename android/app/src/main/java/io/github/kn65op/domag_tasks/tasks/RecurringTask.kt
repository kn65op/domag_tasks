package io.github.kn65op.domag_tasks.tasks

import io.github.kn65op.domag_tasks.utils.platform.localization.Localization
import io.github.kn65op.domag_tasks.utils.serializer.ZoneDateTimeWithoutZoneChangeSerializer
import io.github.kn65op.domag_tasks.utils.time.Period
import kotlinx.serialization.Required
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import java.time.ZonedDateTime

@Serializable
class RecurringTask(
    override var summary: String,
    @Serializable(with = ZoneDateTimeWithoutZoneChangeSerializer::class)
    override var nextDeadline: ZonedDateTime? = ZonedDateTime.now(),
    var period: Period = Period.ofDays(1),
    @Required
    override var id: Id = 0,
    @Serializable(with = DeadlineCalculationStrategySerializer::class)
    var deadlineCalculationStrategy: DeadlineCalculationStrategy = DeadlineCalculationStrategyFactory().createStrategy(
        0
    )
) : Task {
    companion object {
        const val type = "RECURRING TASK"
    }

    override val type
        get() = Companion.type

    override var done: Boolean
        get() = false
        set(value) {
            if (value) nextDeadline =
                deadlineCalculationStrategy.calculateDeadline(nextDeadline!!, period)
        }

    override fun nextDeadlineText(localization: Localization): String =
        "${nextDeadline?.format(taskTimeFormat)} (${period.toHumanReadableString(localization)})"


    override fun serializeToString(): String = Json.encodeToString(serializer(), this)

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as RecurringTask

        if (summary != other.summary) return false
        if (nextDeadline != other.nextDeadline) return false
        if (period != other.period) return false
        if (id != other.id) return false
        if (deadlineCalculationStrategy.getType() != other.deadlineCalculationStrategy.getType()) return false

        return true
    }

    override fun hashCode(): Int {
        var result = summary.hashCode()
        result = 31 * result + (nextDeadline?.hashCode() ?: 0)
        result = 31 * result + period.hashCode()
        result = 31 * result + id
        result = 31 * result + deadlineCalculationStrategy.hashCode()
        return result
    }

}