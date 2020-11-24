package io.github.kn65op.domag_tasks.tasks

import kotlinx.serialization.KSerializer
import kotlinx.serialization.descriptors.PrimitiveKind
import kotlinx.serialization.descriptors.PrimitiveSerialDescriptor
import kotlinx.serialization.descriptors.SerialDescriptor
import kotlinx.serialization.encoding.Decoder
import kotlinx.serialization.encoding.Encoder

class DeadlineCalculationStrategySerializer : KSerializer<DeadlineCalculationStrategy> {
    override val descriptor: SerialDescriptor =
        PrimitiveSerialDescriptor("DeadlineCalculationStrategy", PrimitiveKind.STRING)

    override fun serialize(encoder: Encoder, value: DeadlineCalculationStrategy) {
        encoder.encodeInt(value.getType().ordinal)
    }

    override fun deserialize(decoder: Decoder): DeadlineCalculationStrategy =
        DeadlineCalculationStrategyFactory().createStrategy(decoder.decodeInt())
}
