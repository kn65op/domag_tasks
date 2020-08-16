package io.github.kn65op.domag_tasks.UiTestUtils

import io.github.kn65op.domag_tasks.tasks.DeadlineCalculationStrategyType

fun toDeadlineStrategyText(strategy: DeadlineCalculationStrategyType) =
    when (strategy) {
        DeadlineCalculationStrategyType.FromNow -> "Next deadline from task done date"
        DeadlineCalculationStrategyType.FromLastDeadline -> "Next deadline from last deadline"
    }
