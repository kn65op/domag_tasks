package com.example.domag.notifications

import android.app.AlarmManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.icu.util.Calendar
import android.util.Log

class Alarm {
    fun start(context: Context?) {
        val alarmMgr = alarmManager(context)
        val alarmIntent = alarmIntent(context)
        val calendar = wakeUpDate()

        createAlarm(alarmMgr, calendar, alarmIntent)

        Log.i(LOG_TAG, "Alarm has been set")
    }

    private fun createAlarm(
        alarmMgr: AlarmManager,
        calendar: Calendar,
        alarmIntent: PendingIntent
    ) {
        Log.i(LOG_TAG, "Alarm will be triggered at ${calendar.time}")
        alarmMgr.setInexactRepeating(
            AlarmManager.RTC_WAKEUP,
            calendar.timeInMillis,
            AlarmManager.INTERVAL_DAY,
            alarmIntent
        )
    }

    private fun wakeUpDate(): Calendar = Calendar.getInstance().apply {
        timeInMillis = System.currentTimeMillis()
        set(Calendar.HOUR_OF_DAY, 21)
        set(Calendar.MINUTE, 30)
        set(Calendar.SECOND, 0)
    }

    private fun alarmIntent(context: Context?): PendingIntent =
        Intent(context, TodayAndPastTasksNotification::class.java).let { intent ->
            PendingIntent.getBroadcast(context, 0, intent, 0)
        }

    private fun alarmManager(context: Context?): AlarmManager =
        context?.getSystemService(Context.ALARM_SERVICE) as AlarmManager

    companion object {
        const val LOG_TAG = "Alarm"
    }
}