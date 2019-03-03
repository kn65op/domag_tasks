package com.example.domag.utils

import kotlinx.serialization.*
import kotlinx.serialization.internal.StringDescriptor
import java.time.ZoneId
import java.time.ZoneOffset
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

class ZoneDateTimeSerializer : KSerializer<ZonedDateTime> {
    companion object {
        private val timeFormatter = DateTimeFormatter.ISO_OFFSET_DATE_TIME
    }
    override val descriptor: SerialDescriptor =
        StringDescriptor.withName("ZoneDateTime")

    override fun serialize(encoder: Encoder, obj: ZonedDateTime) {
        encoder.encodeString(obj.withZoneSameInstant(ZoneOffset.UTC).format(timeFormatter))
    }

    override fun deserialize(decoder: Decoder): ZonedDateTime {
        return ZonedDateTime.parse(decoder.decodeString(), timeFormatter).withZoneSameInstant(ZoneId.systemDefault())
    }
}
