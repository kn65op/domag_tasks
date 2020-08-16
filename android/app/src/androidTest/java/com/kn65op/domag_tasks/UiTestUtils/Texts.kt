package com.kn65op.domag_tasks.UiTestUtils

import com.kn65op.domag_tasks.tasks.DeadlineCalculationStrategyType

fun toDeadlineStrategyText(strategy: DeadlineCalculationStrategyType) =
    when (strategy) {
        DeadlineCalculationStrategyType.FromNow -> "Next deadline from task done date"
        DeadlineCalculationStrategyType.FromLastDeadline -> "Next deadline from last deadline"
    }
