package com.example.domag.gui

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
import com.example.domag.R
import com.example.domag.gui.utils.replaceText
import com.example.domag.storage.DataStorage
import com.example.domag.storage.DataStorageFactory
import com.example.domag.tasks.NoDeadlineTask
import com.example.domag.tasks.RecurringTask
import com.example.domag.tasks.SimpleTask
import com.example.domag.tasks.Task
import com.example.domag.utils.time.Period
import com.example.domag.utils.time.PeriodType
import kotlinx.android.synthetic.main.task_edit.*
import java.io.Serializable
import kotlin.NumberFormatException
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
    internal var periodType = day_type

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.task_edit)
        setupActionBar()
        prepareTaskTypeSpinner()
        preparePeriodTypeSpinner()
        storage = DataStorageFactory().createDriveDataStorageFactory(applicationContext)

        fillFields()
    }

    private fun fillFields() {
        val passedTasks = listOf(SimpleTask.type, RecurringTask.type, NoDeadlineTask.type).map {
            Pair(
                it,
                intent.getSerializableExtra(it)
            )
        }.filter { it.second != null }
        when {
            passedTasks.size > 1 -> {
                Log.e(LOG_TAG, "passed two different task!")
                finish()
            }
            passedTasks.isEmpty() -> fillSimpleTaskFields(null)
            passedTasks.first().first == RecurringTask.type -> fillRecurringTaskFields(
                passedTasks.first().second
            )
            passedTasks.first().first == NoDeadlineTask.type -> fillNoDeadlineTaskFields(
                passedTasks.first().second
            )
            passedTasks.first().first == SimpleTask.type -> fillNoDeadlineTaskFields(
                passedTasks.first().second
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
        recurringTask = recurringTaskPassed as? RecurringTask ?: RecurringTask("")
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
        task_period_value.replaceText("${recurringTask.period.count}")
        task_period_type_spinner.setSelection(unit)
        periodType = unit
    }

    private fun setCommonFieldsFromTask(taskType: Int) {
        val t = task
        add_task_deadline_date.text = (t.nextDeadline ?: ZonedDateTime.now()).format(timeFormatter)
        newTaskName.replaceText(t.summary)
        task_type_selection_spinner.setSelection(taskType)
    }

    private fun prepareTaskTypeSpinner() {
        ArrayAdapter.createFromResource(
            this, R.array.task_types,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            task_type_selection_spinner.adapter = adapter
            task_type_selection_spinner.onItemSelectedListener = TaskTypeListener(this)
        }
    }

    internal fun changeActivityToSimpleTask() {
        Log.i(LOG_TAG, "Editing simple task")
        simpleTask = SimpleTask(summary = readSummary(), nextDeadline = readTime(), id = task.id)
        task = simpleTask
        val information: LinearLayout = findViewById(R.id.recurring_information_layout)
        information.visibility = LinearLayout.GONE
        val date_information: LinearLayout = findViewById(R.id.add_task_date)
        date_information.visibility = LinearLayout.VISIBLE
        config_simple_task_button.setOnClickListener {
            Log.i(LOG_TAG, "store simple task")
            simpleTask.summary = readSummary()
            simpleTask.nextDeadline = readTime()
            storage.store(simpleTask)
            finish()
        }
    }

    private fun readSummary() = newTaskName.text.toString()

    private fun readTime(): ZonedDateTime {
        return ZonedDateTime.of(
            LocalDate.parse(add_task_deadline_date.text, timeFormatter),
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
            period = translateToPeriod()
        )
        task = recurringTask
        fillRecurringTaskFields()
        val information: LinearLayout = findViewById(R.id.recurring_information_layout)
        information.visibility = LinearLayout.VISIBLE
        val date_information: LinearLayout = findViewById(R.id.add_task_date)
        date_information.visibility = LinearLayout.VISIBLE
        config_simple_task_button.setOnClickListener {
            Log.i(LOG_TAG, "store recurring task")
            recurringTask.summary = readSummary()
            recurringTask.nextDeadline = readTime()
            recurringTask.period = translateToPeriod()
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
        val information: LinearLayout = findViewById(R.id.recurring_information_layout)
        information.visibility = LinearLayout.GONE
        val date_information: LinearLayout = findViewById(R.id.add_task_date)
        date_information.visibility = LinearLayout.GONE
        config_simple_task_button.setOnClickListener {
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
            task_period_value.text.toString().toInt()
        } catch (e: NumberFormatException) {
            Log.w(LOG_TAG, "No number in ${task_period_value.text}, return 0")
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
        datePicker.date = LocalDate.parse(add_task_deadline_date.text, timeFormatter)
        datePicker.show(supportFragmentManager, "DatePickerFragment")
    }

    override fun onDialogNegativeClick(dialog: DialogFragment) {
        Log.i(LOG_TAG, "Do nothing")
    }

    override fun onDialogPositiveClick(dialog: DialogFragment) {
        dialog as DatePickerFragment
        add_task_deadline_date.text = dialog.date.format(timeFormatter)
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

    companion object {
        private const val LOG_TAG = "TaskEditActivity"
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
        editActivity.periodType = position;
    }

    override fun onNothingSelected(p0: AdapterView<*>?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}
