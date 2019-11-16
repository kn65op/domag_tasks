package com.example.domag.tasks

import kotlinx.serialization.*
import kotlinx.serialization.internal.StringDescriptor
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

class DeadlineCalculationStrategySerializer : KSerializer<DeadlineCalculationStrategy> {
    override val descriptor: SerialDescriptor =
        StringDescriptor.withName("DeadlineCalculationStrategy")

    override fun serialize(encoder: Encoder, obj: DeadlineCalculationStrategy) {
        encoder.encodeInt(obj.getType().ordinal)
    }

    override fun deserialize(decoder: Decoder): DeadlineCalculationStrategy =
        DeadlineCalculationStrategyFactory().createStrategy(decoder.decodeInt())
}
