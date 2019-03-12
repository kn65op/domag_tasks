package com.example.domag.notifications

import android.app.Notification
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.util.Log
import androidx.core.app.NotificationCompat
import androidx.core.app.NotificationManagerCompat
import com.example.domag.R
import com.example.domag.storage.DataStorageFactory
import java.time.LocalDate

class TodayTaskNotification : BroadcastReceiver() {
    override fun onReceive(context: Context?, intent: Intent?) {
        Log.i(LOG_TAG, "Received something")
        if (context != null) {
            notifyTasks(context)
        } else {
            Log.e(LOG_TAG, """Can't notify, context is null""")
        }
    }

    private fun notifyTasks(context: Context) {
        Log.i(LOG_TAG, "Notify today tasks")
        val tasks = DataStorageFactory().createDriveDataStorageFactory(context).loadTasks()

        val todayTasks = tasks.tasks.filter { it.nextDeadline.toLocalDate() == LocalDate.now() }
        if (todayTasks.isEmpty()) {
            Log.i(LOG_TAG, "There is no tasks for today")
        } else {
            val text = todayTasks.fold("") { acc, task -> "$acc${task.summary}\n" }

            val builder =
                Notification.Builder(context, NotificationChannels.TodayTasks.name)
                    .setSmallIcon(R.drawable.ic_launcher_foreground)
                    .setContentTitle(context.getString(R.string.todays_tasks))
                    .setContentText(text)
                    .setPriority(NotificationCompat.PRIORITY_DEFAULT)

            val notificationId = 2
            with(NotificationManagerCompat.from(context)) {
                notify(notificationId, builder.build())
            }
        }
    }

    companion object {
        const val LOG_TAG = "TodayTasksNotification"
    }
}