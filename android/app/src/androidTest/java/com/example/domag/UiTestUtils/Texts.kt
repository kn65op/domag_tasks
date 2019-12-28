package com.example.domag.UiTestUtils

import com.example.domag.tasks.DeadlineCalculationStrategyType

fun toDeadlineStrategyText(strategy: DeadlineCalculationStrategyType) =
    when (strategy) {
        DeadlineCalculationStrategyType.FromNow -> "Next deadline from task done date"
        DeadlineCalculationStrategyType.FromLastDeadline -> "Next deadline from last deadline"
    }
