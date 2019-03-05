package com.example.domag.gui

import android.annotation.TargetApi
import android.app.DatePickerDialog
import android.app.Dialog
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.DatePicker
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.DialogFragment
import com.example.domag.R
import com.example.domag.storage.DriveDataStorage
import com.example.domag.tasks.JsonTaskDeserializer
import com.example.domag.tasks.SimpleTask
import com.example.domag.utils.platform.AndroidWrapper
import kotlinx.android.synthetic.main.activity_add_task.*
import java.lang.Exception
import java.time.LocalDate
import java.time.LocalTime
import java.time.ZoneId
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

val hardocodedHour: LocalTime = LocalTime.of(12, 0)

class AddTaskActivity(
    val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy")
) : AppCompatActivity() {

    class DatePickerFragment(
        private val dateField: TextView,
        private val timeFormatter: DateTimeFormatter
    ) : DialogFragment(), DatePickerDialog.OnDateSetListener {

        override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
            val dateWritten = LocalDate.parse(dateField.text, timeFormatter)
            Log.i(LOG_TAG, "Create date from: ${dateField.text}")
            if (context == null)
                throw Exception("Dialog cannot be created without context")

            return DatePickerDialog(context!!, this, dateWritten.year, dateWritten.monthValue - 1, dateWritten.dayOfMonth)
        }

        override fun onDateSet(view: DatePicker, year: Int, month: Int, day: Int) {
            val currentTime = ZonedDateTime.of(LocalDate.of(year, month + 1, day), hardocodedHour, ZoneId.systemDefault())

            dateField.text = currentTime.format(timeFormatter)
        }

        companion object {
            private const val LOG_TAG = "DataPickerFragment"
        }
    }

    fun showDatePicker() {
        DatePickerFragment(add_task_deadline_date, timeFormatter).show(supportFragmentManager, "AddTaskDatePicker")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_add_task)
        setupActionBar()
        add_task_deadline_date.text = ZonedDateTime.now().format(timeFormatter)

        addTaskButton.setOnClickListener {
            val storage = DriveDataStorage(applicationContext, AndroidWrapper(applicationContext), taskDeserializer = JsonTaskDeserializer())
            val deadline = ZonedDateTime.of(LocalDate.parse(add_task_deadline_date.text, timeFormatter), hardocodedHour, ZoneId.systemDefault())
            storage.store(SimpleTask(newTaskName.text.toString(), deadline))
            finish()
        }
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB)
    private fun setupActionBar() {
        supportActionBar?.setDisplayHomeAsUpEnabled(true)
    }
}
