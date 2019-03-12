package com.example.domag.notifications

import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.Context
import android.util.Log
import com.example.domag.R

class Notifications {
    fun registerChannels(context: Context) {
        val name = context.getString(R.string.todays_task_notification_name)
        val descriptionText = context.getString(R.string.todays_tasks_notification_description)
        val importance = NotificationManager.IMPORTANCE_DEFAULT
        val channel = NotificationChannel(NotificationChannels.TodayTasks.name, name, importance).apply {
            description = descriptionText
        }
        // Register the channel with the system
        val notificationManager: NotificationManager =
            context.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        notificationManager.createNotificationChannel(channel)
        Log.i(LOG_TAG, "Registered notification channel: ${NotificationChannels.TodayTasks.name}")
    }

    companion object {
        private const val LOG_TAG = "Notifications"
    }
}