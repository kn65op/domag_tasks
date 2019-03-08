package com.example.domag.gui

import android.annotation.TargetApi
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.DialogFragment
import com.example.domag.R
import com.example.domag.gui.utils.replaceText
import com.example.domag.storage.DataStorage
import com.example.domag.storage.DriveDataStorage
import com.example.domag.tasks.JsonTaskDeserializer
import com.example.domag.tasks.SimpleTask
import com.example.domag.tasks.Task
import com.example.domag.utils.platform.AndroidWrapper
import kotlinx.android.synthetic.main.simple_task_edit.*
import java.time.LocalDate
import java.time.LocalTime
import java.time.ZoneId
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

class SimpleTaskEditActivity(
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy"),
    private val hardcodedHour: LocalTime = LocalTime.of(12, 0)
) : AppCompatActivity(),
    DatePickerFragment.DatePickerListener {

    private lateinit var task: Task;
    private lateinit var storage : DataStorage

    fun showDatePicker(view: View) {
        val datePicker = DatePickerFragment()
        datePicker.date = LocalDate.parse(add_task_deadline_date.text, timeFormatter)
        datePicker.show(supportFragmentManager, "DatePickerFragment")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.simple_task_edit)
        setupActionBar()
        val dataPassed = intent.getSerializableExtra("Task")
        task = dataPassed as? Task ?: SimpleTask("", ZonedDateTime.now())
        add_task_deadline_date.text = task.nextDeadline.format(timeFormatter)
        newTaskName.replaceText(task.summary)

        storage = DriveDataStorage(
            applicationContext,
            AndroidWrapper(applicationContext),
            taskDeserializer = JsonTaskDeserializer()
        )

        addTaskButton.setOnClickListener {
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
            R.id.remove_task_menu_item ->
            {
                storage.remove(task)
                finish()
                return true
            }
            else -> super.onOptionsItemSelected(item)
        }
    }

    companion object {
        private const val LOG_TAG = "SimpleTaskEditActivity"
    }
}
