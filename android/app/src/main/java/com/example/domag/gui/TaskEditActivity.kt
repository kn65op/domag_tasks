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
import kotlinx.android.synthetic.main.task_edit.*
import java.time.LocalDate
import java.time.LocalTime
import java.time.ZoneId
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

internal enum class PeriodType(val d: Int) {
    Day(0),
    Week(1),
    Month(2),
    Year(3)
}

class TaskEditActivity(
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy"),
    private val hardcodedHour: LocalTime = LocalTime.of(12, 0)
) : AppCompatActivity(),
    DatePickerFragment.DatePickerListener {

    private lateinit var simpleTask: SimpleTask
    private lateinit var recurringTask: RecurringTask
    private lateinit var task: Task
    private lateinit var storage: DataStorage
    private lateinit var spinner: Spinner
    internal var periodType = PeriodType.Day

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.task_edit)
        setupActionBar()
        prepareTaskTypeSpinner()
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
            //val periodUnits =
            spinner.setSelection(1)
        } else {
            simpleTask = simpleTaskPassed as? SimpleTask ?: SimpleTask("")
            Log.i(LOG_TAG, "edit simple task: ${simpleTask.summary}")
            task = simpleTask
            setCommonFieldsFromTask()
            spinner.setSelection(0)
        }
    }

    private fun setCommonFieldsFromTask() {
        val t = task
        add_task_deadline_date.text = t.nextDeadline.format(timeFormatter)
        newTaskName.replaceText(t.summary)
    }

    private fun prepareTaskTypeSpinner() {
        spinner = findViewById(R.id.task_type_selection_spinner)
        ArrayAdapter.createFromResource(
            this, R.array.task_types,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = adapter
            spinner.onItemSelectedListener = TaskTypeListener(this)
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
        val information: LinearLayout = findViewById(R.id.recurring_information_layout)
        information.visibility = LinearLayout.VISIBLE
        config_simple_task_button.setOnClickListener {
            Log.i(LOG_TAG, "store recurring task")
            recurringTask.summary = readSummary()
            recurringTask.nextDeadline = readTime()
            storage.store(recurringTask)
            finish()
        }
        preparePeriodTypeSpinner()
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
        when (position) {
            0 -> editActivity.periodType = PeriodType.Day
            1 -> editActivity.periodType = PeriodType.Week
            2 -> editActivity.periodType = PeriodType.Month
            3 -> editActivity.periodType = PeriodType.Year
        }
    }

    override fun onNothingSelected(p0: AdapterView<*>?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}
