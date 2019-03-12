package com.example.domag.tasks

import android.util.Log
import org.junit.Test
import java.time.LocalDate
import java.time.ZonedDateTime
import java.time.format.DateTimeFormatter

class SimpleTaskTest {
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd MMMM yyyy")

    @Test
    fun TestParser()
    {
        Log.logEnabled = true
        val dateNow = ZonedDateTime.now()
        Log.i("TAG", dateNow.format(timeFormatter))
        val text = "Pn 22 grudnia 2932"

        LocalDate.parse(text, timeFormatter)
    }
}
