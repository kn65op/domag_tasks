package com.example.domag.gui

import android.app.DatePickerDialog
import android.app.Dialog
import android.content.Context
import android.os.Bundle
import android.util.Log
import android.widget.DatePicker
import androidx.fragment.app.DialogFragment
import java.time.LocalDate

class DatePickerFragment : DialogFragment(), DatePickerDialog.OnDateSetListener {
    private lateinit var listener: DatePickerListener

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
                ("$context must implement DatePickerListener")
            )
        }
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        Log.i(LOG_TAG, "Create date from: $date")
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