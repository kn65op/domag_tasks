package com.kn65op.domag_tasks.tasks

import kotlinx.serialization.*
import kotlinx.serialization.internal.StringDescriptor

class DeadlineCalculationStrategySerializer : KSerializer<DeadlineCalculationStrategy> {
    override val descriptor: SerialDescriptor =
        StringDescriptor.withName("DeadlineCalculationStrategy")

    override fun serialize(encoder: Encoder, obj: DeadlineCalculationStrategy) {
        encoder.encodeInt(obj.getType().ordinal)
    }

    override fun deserialize(decoder: Decoder): DeadlineCalculationStrategy =
        DeadlineCalculationStrategyFactory().createStrategy(decoder.decodeInt())
}
