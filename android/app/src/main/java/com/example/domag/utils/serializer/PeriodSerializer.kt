package com.example.domag.utils.serializer

import kotlinx.serialization.*
import kotlinx.serialization.internal.StringDescriptor
import java.time.Period

class PeriodSerializer : KSerializer<Period> {
    override val descriptor: SerialDescriptor =
        StringDescriptor.withName("Period")

    override fun serialize(encoder: Encoder, obj: Period) {
        encoder.encodeString(obj.toString())
    }

    override fun deserialize(decoder: Decoder): Period {
        return Period.parse(decoder.decodeString())
    }
}
