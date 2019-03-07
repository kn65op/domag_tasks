package com.example.domag.gui

import android.annotation.TargetApi
import android.app.DatePickerDialog
import android.app.Dialog
import android.content.Context
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.DatePicker
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.DialogFragment
import com.example.domag.R
import com.example.domag.storage.DriveDataStorage
import com.example.domag.tasks.JsonTaskDeserializer
import com.example.domag.tasks.SimpleTask
import com.example.domag.utils.platform.AndroidWrapper
import kotlinx.android.synthetic.main.activity_add_task.*
import java.time.LocalDate
import java.time.LocalTime
import java.time.ZoneId
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

class DatePickerFragment() : DialogFragment(), DatePickerDialog.OnDateSetListener {
    internal lateinit var listener: DatePickerListener

    lateinit var date: LocalDate

    interface DatePickerListener {
        fun onDialogPositiveClick(dialog: DialogFragment)
        fun onDialogNegativeClick(dialog: DialogFragment)
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        try {
            listener = context as DatePickerListener
        } catch (e: ClassCastException) {
            throw ClassCastException(
                (context.toString() + " must implement DatePickerListener")
            )
        }
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        Log.i(LOG_TAG, "Create date from: ${date}")
        if (context == null)
            throw Exception("Dialog cannot be created without context")

        return DatePickerDialog(
            context!!,
            this,
            date.year,
            date.monthValue - 1,
            date.dayOfMonth
        )
    }

    override fun onDateSet(view: DatePicker, year: Int, month: Int, day: Int) {
        date = LocalDate.of(year, month + 1, day)
        listener.onDialogPositiveClick(this)
    }

    companion object {
        private const val LOG_TAG = "DataPickerFragment"
    }
}

class AddTaskActivity(
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy"),
    private val hardcodedHour: LocalTime = LocalTime.of(12, 0)
) : AppCompatActivity(),
    DatePickerFragment.DatePickerListener {

    fun showDatePicker(view: View) {
        val datePicker = DatePickerFragment()
        datePicker.date = LocalDate.parse(add_task_deadline_date.text, timeFormatter)
        datePicker.show(supportFragmentManager, "DatePickerFragment")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_add_task)
        setupActionBar()
        add_task_deadline_date.text = ZonedDateTime.now().format(timeFormatter)

        addTaskButton.setOnClickListener {
            val storage = DriveDataStorage(
                applicationContext,
                AndroidWrapper(applicationContext),
                taskDeserializer = JsonTaskDeserializer()
            )
            val deadline = ZonedDateTime.of(
                LocalDate.parse(add_task_deadline_date.text, timeFormatter),
                hardcodedHour,
                ZoneId.systemDefault()
            )
            storage.store(SimpleTask(newTaskName.text.toString(), deadline))
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

    companion object {
        private const val LOG_TAG = "AddTaskActivity"
    }
}
