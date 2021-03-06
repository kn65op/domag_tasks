package io.github.kn65op.domag_tasks.gui

import android.annotation.TargetApi
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.LinearLayout
import android.widget.Spinner
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.DialogFragment
import io.github.kn65op.domag_tasks.R
import io.github.kn65op.domag_tasks.databinding.TaskEditBinding
import io.github.kn65op.domag_tasks.gui.utils.replaceText
import io.github.kn65op.domag_tasks.storage.DataStorage
import io.github.kn65op.domag_tasks.storage.DataStorageFactory
import io.github.kn65op.domag_tasks.tasks.*
import io.github.kn65op.domag_tasks.utils.time.Period
import io.github.kn65op.domag_tasks.utils.time.PeriodType
import java.io.Serializable
import java.time.LocalDate
import java.time.LocalTime
import java.time.ZoneId
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

private const val day_type = 0
private const val week_type = 1
private const val month_type = 2
private const val year_type = 3

private const val simple_task_type = 0
private const val recurring_task_type = 1
private const val no_deadline_task_type = 2

private const val from_now_strategy_as_default = 0

class TaskEditActivity(
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy"),
    private val hardcodedHour: LocalTime = LocalTime.of(12, 0)
) : AppCompatActivity(),
    DatePickerFragment.DatePickerListener {

    private lateinit var simpleTask: SimpleTask
    private lateinit var recurringTask: RecurringTask
    private lateinit var noDeadlineTask: NoDeadlineTask
    private lateinit var task: Task
    private lateinit var storage: DataStorage
    private lateinit var binding: TaskEditBinding
    internal var periodType = day_type
    internal var deadlineStrategyType = from_now_strategy_as_default

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = TaskEditBinding.inflate(layoutInflater)
        setContentView(binding.root)
        setupActionBar()
        prepareTaskTypeSpinner()
        preparePeriodTypeSpinner()
        prepareDeadlineStrategySpinner()
        storage = DataStorageFactory().createDriveDataStorageFactory(applicationContext)

        fillFields()
    }

    private fun fillFields() {
        val passedTaskType = intent.getStringExtra(taskTypeIntentName)
        Log.i(LOG_TAG, "Passed task type: $passedTaskType")
        when (passedTaskType) {
            null -> fillSimpleTaskFields(null)
            RecurringTask.type -> fillRecurringTaskFields(
                intent.getSerializableExtra(taskIntentName)
            )
            NoDeadlineTask.type -> fillNoDeadlineTaskFields(
                intent.getSerializableExtra(taskIntentName)
            )
            SimpleTask.type -> fillSimpleTaskFields(
                intent.getSerializableExtra(taskIntentName)
            )
            else -> fillSimpleTaskFields(null)
        }
    }

    private fun fillSimpleTaskFields(simpleTaskPassed: Serializable?) {
        simpleTask = simpleTaskPassed as? SimpleTask ?: SimpleTask("")
        Log.i(LOG_TAG, "edit simple task: ${simpleTask.summary}")
        task = simpleTask
        setCommonFieldsFromTask(simple_task_type)
    }

    private fun fillNoDeadlineTaskFields(noDeadlineTaskPassed: Serializable?) {
        noDeadlineTask = noDeadlineTaskPassed as? NoDeadlineTask ?: NoDeadlineTask("")
        Log.i(LOG_TAG, "edit no deadline task: ${noDeadlineTask.summary}")
        task = noDeadlineTask
        setCommonFieldsFromTask(no_deadline_task_type)
    }

    private fun fillRecurringTaskFields(recurringTaskPassed: Serializable?) {
        recurringTask = recurringTaskPassed as? RecurringTask ?: RecurringTask(
            summary = "",
            deadlineCalculationStrategy = createStrategy()
        )
        Log.i(LOG_TAG, "edit recurring task: ${recurringTask.summary}")
        task = recurringTask
        fillRecurringTaskFields()
        setCommonFieldsFromTask(recurring_task_type)
    }

    private fun fillRecurringTaskFields() {
        val unit = when (recurringTask.period.type) {
            PeriodType.Day -> day_type
            PeriodType.Month -> month_type
            PeriodType.Week -> week_type
            PeriodType.Year -> year_type
        }
        Log.i(
            LOG_TAG,
            "type is $unit (${recurringTask.period.type} with value ${recurringTask.period.count}"
        )
        binding.taskPeriodValue.replaceText("${recurringTask.period.count}")
        binding.taskPeriodTypeSpinner.setSelection(unit)
        periodType = unit
        deadlineStrategyType = recurringTask.deadlineCalculationStrategy.getType().ordinal
        Log.i(
            LOG_TAG,
            "deadline strategy: ${recurringTask.deadlineCalculationStrategy.getType().ordinal}"
        )
        binding.nextDeadlineStrategyTypeSpinner.setSelection(recurringTask.deadlineCalculationStrategy.getType().ordinal)
    }

    private fun setCommonFieldsFromTask(taskType: Int) {
        val t = task
        binding.addTaskDeadlineDate.text =
            (t.nextDeadline ?: ZonedDateTime.now()).format(timeFormatter)
        binding.newTaskName.replaceText(t.summary)
        binding.taskTypeSelectionSpinner.setSelection(taskType)
    }

    private fun prepareTaskTypeSpinner() {
        ArrayAdapter.createFromResource(
            this, R.array.task_types,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            binding.taskTypeSelectionSpinner.adapter = adapter
            binding.taskTypeSelectionSpinner.onItemSelectedListener = TaskTypeListener(this)
        }
    }

    private fun prepareDeadlineStrategySpinner() {
        val spinner: Spinner = findViewById(R.id.next_deadline_strategy_type_spinner)
        ArrayAdapter.createFromResource(
            this, R.array.deadline_strategies,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = adapter
            spinner.onItemSelectedListener = DeadlineStrategyListener(this)
        }
    }

    private fun showViewItems(ids: List<Int>) {
        changeItemsVisibility(ids, LinearLayout.VISIBLE)
    }

    private fun hideViewItems(ids: List<Int>) {
        changeItemsVisibility(ids, LinearLayout.GONE)
    }

    private fun changeItemsVisibility(ids: List<Int>, visibility: Int) {
        ids.forEach {
            val field: View = findViewById(it)
            field.visibility = visibility
        }
    }

    internal fun changeActivityToSimpleTask() {
        Log.i(LOG_TAG, "Editing simple task")
        simpleTask = SimpleTask(summary = readSummary(), nextDeadline = readTime(), id = task.id)
        task = simpleTask

        showViewItems(listOf(R.id.add_task_date))
        hideViewItems(
            listOf(
                R.id.next_deadline_strategy_type_spinner,
                R.id.task_period_type_spinner,
                R.id.task_period_value,
                R.id.task_period_inpout_layout,
            )
        )

        binding.configSimpleTaskButton.setOnClickListener {
            Log.i(LOG_TAG, "store simple task")
            simpleTask.summary = readSummary()
            simpleTask.nextDeadline = readTime()
            storage.store(simpleTask)
            finish()
        }
    }

    private fun readSummary() = binding.newTaskName.text.toString()

    private fun readTime(): ZonedDateTime {
        return ZonedDateTime.of(
            LocalDate.parse(binding.addTaskDeadlineDate.text, timeFormatter),
            hardcodedHour,
            ZoneId.systemDefault()
        )
    }

    internal fun changeActivityToRecurringTask() {
        Log.i(LOG_TAG, "Editing recurring task")
        recurringTask = RecurringTask(
            summary = readSummary(),
            nextDeadline = readTime(),
            id = task.id,
            period = translateToPeriod(),
            deadlineCalculationStrategy = createStrategy()
        )
        task = recurringTask
        fillRecurringTaskFields()

        showViewItems(
            listOf(
                R.id.add_task_date,
                R.id.next_deadline_strategy_type_spinner,
                R.id.task_period_type_spinner,
                R.id.task_period_value,
                R.id.task_period_inpout_layout,
            )
        )

        val dateInformation: LinearLayout = findViewById(R.id.add_task_date)
        dateInformation.visibility = LinearLayout.VISIBLE
        binding.configSimpleTaskButton.setOnClickListener {
            Log.i(LOG_TAG, "store recurring task")
            recurringTask.summary = readSummary()
            recurringTask.nextDeadline = readTime()
            recurringTask.period = translateToPeriod()
            recurringTask.deadlineCalculationStrategy =
                DeadlineCalculationStrategyFactory().createStrategy(deadlineStrategyType)
            storage.store(recurringTask)
            finish()
        }
    }

    internal fun changeActivityToNoDeadlineTask() {
        Log.i(LOG_TAG, "Editing no deadline task")
        noDeadlineTask = NoDeadlineTask(
            summary = readSummary(),
            id = task.id
        )
        task = noDeadlineTask

        hideViewItems(
            listOf(
                R.id.add_task_date,
                R.id.next_deadline_strategy_type_spinner,
                R.id.task_period_type_spinner,
                R.id.task_period_value,
                R.id.task_period_inpout_layout,
            )
        )

        binding.configSimpleTaskButton.setOnClickListener {
            Log.i(LOG_TAG, "store recurring task")
            noDeadlineTask.summary = readSummary()
            storage.store(noDeadlineTask)
            finish()
        }
    }

    private fun translateToPeriod(): Period {
        val periodValue =
            readPeriodValue()
        Log.i(LOG_TAG, "Translate from $periodType")
        return when (periodType) {
            day_type -> Period.ofDays(periodValue)
            week_type -> Period.ofWeeks(periodValue)
            month_type -> Period.ofMonths(periodValue)
            year_type -> Period.ofYears(periodValue)
            else -> {
                Log.e(LOG_TAG, "Invalid period type value $periodType defaulting to days")
                Period.ofDays(periodValue)
            }
        }
    }

    private fun readPeriodValue(): Int {
        return try {
            binding.taskPeriodValue.text.toString().toInt()
        } catch (e: NumberFormatException) {
            Log.w(LOG_TAG, "No number in ${binding.taskPeriodValue.text}, return 0")
            0
        }
    }

    private fun preparePeriodTypeSpinner() {
        val spinner: Spinner = findViewById(R.id.task_period_type_spinner)
        ArrayAdapter.createFromResource(
            this, R.array.task_period_types,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = adapter
            spinner.onItemSelectedListener = PeriodTypeListener(this)
        }
    }

    fun showDatePicker(view: View) {
        Log.i(LOG_TAG, "Show date picker on ${view.id}")
        val datePicker = DatePickerFragment()
        datePicker.date = LocalDate.parse(binding.addTaskDeadlineDate.text, timeFormatter)
        datePicker.show(supportFragmentManager, "DatePickerFragment")
    }

    override fun onDialogNegativeClick(dialog: DialogFragment) {
        Log.i(LOG_TAG, "Do nothing")
    }

    override fun onDialogPositiveClick(dialog: DialogFragment) {
        dialog as DatePickerFragment
        binding.addTaskDeadlineDate.text = dialog.date.format(timeFormatter)
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB)
    private fun setupActionBar() {
        supportActionBar?.setDisplayHomeAsUpEnabled(true)
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.edit_task_menu, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        Log.i(LOG_TAG, "Menu item selected")
        return when (item.itemId) {
            R.id.remove_task_menu_item -> {
                storage.remove(task)
                finish()
                return true
            }
            else -> super.onOptionsItemSelected(item)
        }
    }

    private fun createStrategy() =
        DeadlineCalculationStrategyFactory().createStrategy(deadlineStrategyType)

    companion object {
        private const val LOG_TAG = "TaskEditActivity"
        const val taskTypeIntentName = "Task_type"
        const val taskIntentName = "Task"
    }
}

class TaskTypeListener(private var editActivity: TaskEditActivity) :
    AdapterView.OnItemSelectedListener {
    override fun onItemSelected(p0: AdapterView<*>?, p1: View?, position: Int, p3: Long) {
        when (position) {
            simple_task_type -> editActivity.changeActivityToSimpleTask()
            recurring_task_type -> editActivity.changeActivityToRecurringTask()
            no_deadline_task_type -> editActivity.changeActivityToNoDeadlineTask()
        }
    }

    override fun onNothingSelected(p0: AdapterView<*>?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}

class PeriodTypeListener(private var editActivity: TaskEditActivity) :
    AdapterView.OnItemSelectedListener {
    override fun onItemSelected(p0: AdapterView<*>?, p1: View?, position: Int, p3: Long) {
        Log.i("helper", "$position")
        editActivity.periodType = position
    }

    override fun onNothingSelected(p0: AdapterView<*>?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}

class DeadlineStrategyListener(private var editActivity: TaskEditActivity) :
    AdapterView.OnItemSelectedListener {
    override fun onItemSelected(p0: AdapterView<*>?, p1: View?, position: Int, p3: Long) {
        Log.i("helper", "$position")
        editActivity.deadlineStrategyType = position
    }

    override fun onNothingSelected(p0: AdapterView<*>?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}
