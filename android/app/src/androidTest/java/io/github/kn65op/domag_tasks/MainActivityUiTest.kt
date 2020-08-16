package io.github.kn65op.domag_tasks

import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.platform.app.InstrumentationRegistry
import androidx.test.uiautomator.UiDevice
import io.github.kn65op.domag_tasks.UiTestUtils.*
import io.github.kn65op.domag_tasks.tasks.DeadlineCalculationStrategyType
import io.github.kn65op.domag_tasks.UiTestUtils.*
import io.github.kn65op.domag_tasks.utils.time.PeriodType
import org.junit.Before
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
open class MainActivityUiTest {
    @Before
    fun setup() {
        launchApp()
    }
}

class NoTasksTest : MainActivityUiTest() {
    @Test
    fun whenNoTasks_shouldBeEmpty() {
        prepareEmptyTasks()

        val noTasks = 0
        checkTasksSize(noTasks)
    }
}

class SimpleTaskSortingTest : MainActivityUiTest() {
    @Test
    fun whenThereIsTask_shouldBeOnList() {
        prepareOneSimpleTask()
        checkTaskOnPosition(0, firstTask)
    }

    @Test
    fun whenThereAreFewTasksWithSameDate_shouldBeSortedByName() {
        prepareThreeSimpleTasks()

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, thirdTask)
    }

    @Test
    fun whenThereAreFewTasksWithDifferentDates_shouldBeSortedByDate() {
        prepareEmptyTasks()
        createSimpleTask(thirdTask, date1)
        createSimpleTask(firstTask, date3)
        createSimpleTask(secondTask, date2)

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, thirdTask, date1)
        checkTaskOnPosition(1, secondTask, date2)
        checkTaskOnPosition(2, firstTask, date3)
    }
}

class SimpleTaskDoneUndoneTest : MainActivityUiTest() {
    @Test
    fun whenTaskIsMarkedAsDone_shouldBeOnBottom() {
        prepareThreeSimpleTasks()

        switchTaskDone(firstTask)

        checkTasksSize(threeTasks)
        checkTaskOnPosition(0, secondTask)
        checkTaskOnPosition(1, thirdTask)
        checkTaskOnPosition(2, firstTask)
    }

    @Test
    fun whenTaskIsUnmarkedAsDone_shouldBeInOwnPlaceBack() {
        prepareThreeSimpleTasks()

        switchTaskDone(firstTask)
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, thirdTask)
    }

    @Test
    fun afterRemovalOfTask_shouldHaveOnlyTwoTasks() {
        prepareThreeSimpleTasks()

        removeTask(secondTask)

        val remainingTasks = 2
        checkTasksSize(remainingTasks)
        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, thirdTask)
    }
}

class SimpleTaskEdit : MainActivityUiTest() {
    @Test
    fun taskSummaryEdit() {
        prepareOneSimpleTask()

        changeTaskSummary(firstTask, secondTask)

        checkTaskOnPosition(0, secondTask)
    }

    @Test
    fun taskDateEdit() {
        prepareOneSimpleTask()

        changeTaskDate(firstTask, date2)

        checkTaskOnPosition(0, firstTask, date2)
    }

    @Test
    fun tasksShouldBeSortedAfterChanges() {
        prepareThreeSimpleTasks()

        changeTaskDate(firstTask, date3)
        changeTaskSummary(thirdTask, zeroTask)

        checkTaskOnPosition(0, zeroTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, firstTask, date3)
    }
}

class TaskRemovalTest : MainActivityUiTest() {

    @Test
    fun shouldRemoveOnlyCompletedTasks() {
        prepareThreeSimpleTasks()
        switchTaskDone(firstTask)

        removeDoneTasks()

        val remainingTasks = 2
        checkTasksSize(remainingTasks)
        checkTaskOnPosition(0, secondTask)
        checkTaskOnPosition(1, thirdTask)
        val device = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation())
        checkTaskIsNotInView(firstTask, device)
    }
}

class RecurringTaskWithFromNowStrategyTest : MainActivityUiTest() {
    @Test
    fun recurringTaskShouldForwardDaysPeriod() {
        prepareEmptyTasks()
        createRecurringTask(firstTask, date1, daysAdvance, PeriodType.Day)
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask, toOwnDate(nowPlusDays))
    }

    @Test
    fun recurringTaskShouldForwardWeeksPeriod() {
        prepareEmptyTasks()
        createRecurringTask(firstTask, date1, daysAdvance, PeriodType.Week)
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask, toOwnDate(nowPlusWeeks))
    }

    @Test
    fun recurringTaskShouldForwardMonthsPeriod() {
        prepareEmptyTasks()
        createRecurringTask(firstTask, date1, monthsAdvance, PeriodType.Month)
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask, toOwnDate(nowPlusMonths))
    }

    @Test
    fun recurringTaskShouldForwardYearsPeriod() {
        prepareEmptyTasks()
        createRecurringTask(firstTask, date1, yearsAdvance, PeriodType.Year)
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask, toOwnDate(nowPlusYears))
    }
}

class RecurringTaskEditTest : MainActivityUiTest() {
    @Test
    fun recurringTaskEditShouldHaveGoodPeriodType() {
        prepareEmptyTasks()
        createRecurringTask(firstTask, date1, yearsAdvance, PeriodType.Year)
        clickOnTask(firstTask)

        checkPeriodTypeOnEdit(years)
    }

    @Test
    fun recurringTaskEditShouldHaveGoodNextDeadlineStrategy() {
        prepareEmptyTasks()
        createRecurringTask(
            firstTask,
            date1,
            yearsAdvance,
            PeriodType.Year,
            DeadlineCalculationStrategyType.FromLastDeadline
        )
        clickOnTask(firstTask)

        checkDeadlineStrategyTypeOnEdit(
            DeadlineCalculationStrategyType.FromLastDeadline
        )
    }
}

class RecurringTaskWithFromLastDeadlineStrategy : MainActivityUiTest() {
    @Test
    fun recurringTaskShouldForwardDaysPeriod() {
        val daysDifference = 1L
        prepareEmptyTasks()
        createRecurringTask(
            firstTask,
            toOwnDate(now.minusDays(daysDifference)),
            daysAdvance,
            PeriodType.Day,
            DeadlineCalculationStrategyType.FromLastDeadline
        )
        switchTaskDone(firstTask)

        checkTaskOnPosition(0, firstTask, toOwnDate(now.plusDays(daysAdvance - daysDifference)))
    }
}


class TaskWithoutDeadlineTest : MainActivityUiTest() {
    @Test
    fun taskWithoutDeadlineShouldBeShownInProperTab() {
        prepareEmptyTasks()
        createNoDeadlineTask(firstTask)

        val noTasks = 0
        checkTasksSize(noTasks)

        goToNoDeadlineTasks()

        checkTaskSummaryOnPosition(0, firstTask)
    }

    @Test
    fun whenOnTaskWithoutDeadlineTabShouldCreateProperTask() {
        prepareEmptyTasks()
        goToNoDeadlineTasks()
        createTask(firstTask)

        checkTaskSummaryOnPosition(0, firstTask)
    }

    @Test
    fun taskWithoutDeadlineShouldBeRemovedIfDone() {
        prepareEmptyTasks()
        goToNoDeadlineTasks()
        createNoDeadlineTask(firstTask)
        switchTaskDone(firstTask)

        removeDoneTasks()

        val noTasks = 0
        checkTasksSize(noTasks)
    }
}
