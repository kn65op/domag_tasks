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
import com.example.domag.tasks.RecurringTask
import com.example.domag.tasks.SimpleTask
import com.example.domag.tasks.Task
import com.example.domag.utils.time.Period
import com.example.domag.utils.time.PeriodType
import kotlinx.android.synthetic.main.task_edit.*
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

class TaskEditActivity(
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy"),
    private val hardcodedHour: LocalTime = LocalTime.of(12, 0)
) : AppCompatActivity(),
    DatePickerFragment.DatePickerListener {

    private lateinit var simpleTask: SimpleTask
    private lateinit var recurringTask: RecurringTask
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

        val simpleTaskPassed = intent.getSerializableExtra(SimpleTask.type)
        val recurringTaskPassed = intent.getSerializableExtra(RecurringTask.type)
        if (simpleTaskPassed != null && recurringTaskPassed != null) {
            Log.e(LOG_TAG, "passed two different task!")
            finish()
        } else if (recurringTaskPassed != null) {
            recurringTask = recurringTaskPassed as? RecurringTask ?: RecurringTask("")
            Log.i(LOG_TAG, "edit recurring task: ${recurringTask.summary}")
            task = recurringTask
            setCommonFieldsFromTask()
            setRecurringTaskFields()
            task_type_selection_spinner.setSelection(1)
        } else {
            simpleTask = simpleTaskPassed as? SimpleTask ?: SimpleTask("")
            Log.i(LOG_TAG, "edit simple task: ${simpleTask.summary}")
            task = simpleTask
            setCommonFieldsFromTask()
            task_type_selection_spinner.setSelection(0)
        }
    }

    private fun setRecurringTaskFields() {
        val unit = when (recurringTask.period.type) {
            PeriodType.Day -> day_type
            PeriodType.Month -> month_type
            PeriodType.Week -> week_type
            PeriodType.Year -> year_type
        }
        Log.i(LOG_TAG, "type is $unit with value ${recurringTask.period.count}")
        task_period_value.replaceText("${recurringTask.period.count}")
        Log.i(LOG_TAG, "set: $unit")
        task_period_type_spinner.setSelection(unit)
    }

    private fun setCommonFieldsFromTask() {
        val t = task
        add_task_deadline_date.text = t.nextDeadline.format(timeFormatter)
        newTaskName.replaceText(t.summary)
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
        recurringTask = RecurringTask(summary = readSummary(), nextDeadline = readTime(), id = task.id)
        task = recurringTask
        setRecurringTaskFields()
        val information: LinearLayout = findViewById(R.id.recurring_information_layout)
        information.visibility = LinearLayout.VISIBLE
        config_simple_task_button.setOnClickListener {
            Log.i(LOG_TAG, "store recurring task")
            recurringTask.summary = readSummary()
            recurringTask.nextDeadline = readTime()
            val periodValue =
                try {
                    task_period_value.text.toString().toInt()
                } catch (e: NumberFormatException) {
                    Log.w(LOG_TAG, "No number in ${task_period_value.text}, return 0")
                    0
                }
            recurringTask.period = when (periodType) {
                day_type -> Period.ofDays(periodValue)
                week_type -> Period.ofWeeks(periodValue)
                month_type -> Period.ofMonths(periodValue)
                year_type -> Period.ofYears(periodValue)
                else -> {
                    Log.e(LOG_TAG, "Invalid period type value ${periodType} defaultin to days")
                    Period.ofDays(periodValue)
                }
            }
            storage.store(recurringTask)
            finish()
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
        private const val daysInWeek = 7
    }
}

class TaskTypeListener(var editActivity: TaskEditActivity) : AdapterView.OnItemSelectedListener {
    override fun onItemSelected(p0: AdapterView<*>?, p1: View?, position: Int, p3: Long) {
        when (position) {
            0 -> editActivity.changeActivityToSimpleTask()
            1 -> editActivity.changeActivityToRecurringTask()
        }
    }

    override fun onNothingSelected(p0: AdapterView<*>?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}

class PeriodTypeListener(var editActivity: TaskEditActivity) : AdapterView.OnItemSelectedListener {
    override fun onItemSelected(p0: AdapterView<*>?, p1: View?, position: Int, p3: Long) {
        Log.i("helper", "$position")
        when (position) {
            0 -> editActivity.periodType = day_type
            1 -> editActivity.periodType = week_type
            2 -> editActivity.periodType = month_type
            3 -> editActivity.periodType = year_type
        }
    }

    override fun onNothingSelected(p0: AdapterView<*>?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}
