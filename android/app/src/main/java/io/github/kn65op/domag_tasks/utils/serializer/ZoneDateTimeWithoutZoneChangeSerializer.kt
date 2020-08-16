package io.github.kn65op.domag_tasks.utils.serializer

import kotlinx.serialization.*
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

class ZoneDateTimeWithoutZoneChangeSerializer : KSerializer<ZonedDateTime> {
    companion object {
        private val timeFormatter = DateTimeFormatter.ISO_OFFSET_DATE_TIME
    }

    override val descriptor: SerialDescriptor =
        PrimitiveDescriptor("ZoneDateTime", PrimitiveKind.STRING)

    override fun serialize(encoder: Encoder, value: ZonedDateTime) {
        encoder.encodeString(value.format(timeFormatter))
    }

    override fun deserialize(decoder: Decoder): ZonedDateTime {
        return ZonedDateTime.parse(
            decoder.decodeString(),
            timeFormatter
        )
    }
}
