package io.github.kn65op.domag_tasks.tasks

class DeadlineCalculationStrategyFactory {
    private fun createStrategy(type: DeadlineCalculationStrategyType) =
        when (type) {
            DeadlineCalculationStrategyType.FromLastDeadline -> FromPreviousDeadlineCalculationStrategy()
            DeadlineCalculationStrategyType.FromNow -> FromNowDeadlineCalculationStrategy()
            DeadlineCalculationStrategyType.NoSkip -> NoSkipDeadlineCalculationStrategy()
        }

    fun createStrategy(strategy: Int) = createStrategy(
        when (strategy) {
            0 -> DeadlineCalculationStrategyType.FromNow
            1 -> DeadlineCalculationStrategyType.FromLastDeadline
            2 -> DeadlineCalculationStrategyType.NoSkip
            else -> DeadlineCalculationStrategyType.FromNow
        }
    )


}

