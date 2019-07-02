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
import com.example.domag.tasks.SimpleTask
import com.example.domag.tasks.Task
import kotlinx.android.synthetic.main.task_edit.*
import java.time.LocalDate
import java.time.LocalTime
import java.time.ZoneId
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

class TaskEditActivity(
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy"),
    private val hardcodedHour: LocalTime = LocalTime.of(12, 0)
) : AppCompatActivity(),
    DatePickerFragment.DatePickerListener,
    AdapterView.OnItemSelectedListener {

    private lateinit var task: Task
    private lateinit var storage: DataStorage

    fun showDatePicker(view: View) {
        Log.i(LOG_TAG, "Show date picker on ${view.id}")
        val datePicker = DatePickerFragment()
        datePicker.date = LocalDate.parse(add_task_deadline_date.text, timeFormatter)
        datePicker.show(supportFragmentManager, "DatePickerFragment")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.task_edit)
        setupActionBar()
        prepareTaskTypeSpinner()

        val dataPassed = intent.getSerializableExtra("Task")
        task = dataPassed as? Task ?: SimpleTask("", ZonedDateTime.now())
        add_task_deadline_date.text = task.nextDeadline.format(timeFormatter)
        newTaskName.replaceText(task.summary)

        storage = DataStorageFactory().createDriveDataStorageFactory(applicationContext)
    }

    private fun setConfirmToCreateSimpleTask() {
        val information : LinearLayout = findViewById(R.id.recurring_information_layout)
        information.visibility = LinearLayout.GONE
        config_simple_task_button.setOnClickListener {
            val deadline = ZonedDateTime.of(
                LocalDate.parse(add_task_deadline_date.text, timeFormatter),
                hardcodedHour,
                ZoneId.systemDefault()
            )
            task.summary = newTaskName.text.toString()
            task.nextDeadline = deadline
            storage.store(task)
            finish()
        }
    }

    private fun setConfirmToCreateRecurringTask() {
        val information : LinearLayout = findViewById(R.id.recurring_information_layout)
        information.visibility = LinearLayout.VISIBLE
        config_simple_task_button.setOnClickListener {
            Log.i(LOG_TAG, "Recurring")
            finish();
        }
    }

    private fun prepareTaskTypeSpinner() {
        val spinner: Spinner = findViewById(R.id.task_type_selection_spinner)
        ArrayAdapter.createFromResource(
            this, R.array.task_types,
            android.R.layout.simple_spinner_item
        ).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = adapter
            spinner.onItemSelectedListener = this
        }
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

    override fun onItemSelected(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
        when (position) {
            0 -> setConfirmToCreateSimpleTask()
            1 -> setConfirmToCreateRecurringTask()
        }

    }

    override fun onNothingSelected(parent: AdapterView<*>?) {
        TODO("nothing selection not implemented") //To change body of created functions use File | Settings | File Templates.
    }

    companion object {
        private const val LOG_TAG = "TaskEditActivity"
    }
}
