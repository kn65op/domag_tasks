package io.github.kn65op.domag_tasks.tasks

import kotlinx.serialization.*

class DeadlineCalculationStrategySerializer : KSerializer<DeadlineCalculationStrategy> {
    override val descriptor: SerialDescriptor =
        PrimitiveDescriptor("DeadlineCalculationStrategy", PrimitiveKind.STRING)

    override fun serialize(encoder: Encoder, value: DeadlineCalculationStrategy) {
        encoder.encodeInt(value.getType().ordinal)
    }

    override fun deserialize(decoder: Decoder): DeadlineCalculationStrategy =
        DeadlineCalculationStrategyFactory().createStrategy(decoder.decodeInt())
}
