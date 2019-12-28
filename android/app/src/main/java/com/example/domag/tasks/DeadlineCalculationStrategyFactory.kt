package com.example.domag.tasks

class DeadlineCalculationStrategyFactory {
    private fun createStrategy(type: DeadlineCalculationStrategyType) =
        when (type) {
            DeadlineCalculationStrategyType.FromLastDeadline -> FromPreviousDeadlineCalculationStrategy()
            DeadlineCalculationStrategyType.FromNow -> FromNowDeadlineCalculationStrategy()
        }

    fun createStrategy(strategy: Int) = createStrategy(
        when (strategy) {
            0 -> DeadlineCalculationStrategyType.FromNow
            1 -> DeadlineCalculationStrategyType.FromLastDeadline
            else -> DeadlineCalculationStrategyType.FromNow
        }
    )


}

