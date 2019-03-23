package com.example.domag.notifications

import android.app.PendingIntent
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.util.Log
import androidx.core.app.NotificationCompat
import androidx.core.app.NotificationManagerCompat
import androidx.core.app.TaskStackBuilder
import com.example.domag.MainActivity
import com.example.domag.R
import com.example.domag.storage.DataStorageFactory
import com.example.domag.tasks.getOverdueNotDone
import com.example.domag.tasks.getTodayNotDone

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
        val tasksToNotify = tasks.getOverdueNotDone() + tasks.getTodayNotDone()
        if (tasksToNotify.isEmpty()) {
            Log.i(LOG_TAG, "There is no tasks to notify")
        } else {
            val text = tasksToNotify.fold("") { acc, task -> "$acc${task.summary}\n" }
            val tasksCount = tasksToNotify.size

            val intent = Intent(context, MainActivity::class.java)
            val pendingIntent = TaskStackBuilder.create(context).run {
                addNextIntentWithParentStack(intent)
                getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT)
            }


            val builder =
                NotificationCompat.Builder(context, NotificationChannels.TodayTasks.name)
                    .setSmallIcon(R.drawable.ic_launcher_foreground)
                    .setContentTitle(context.getString(R.string.todays_tasks))
                    .setContentText(context.resources.getQuantityString(R.plurals.tasks_to_do, tasksCount, tasksCount))
                    .setStyle(NotificationCompat.BigTextStyle().bigText(text))
                    .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                    .setContentIntent(pendingIntent)

            val notificationId = NotificationId.TodayTasksNotification.ordinal
            with(NotificationManagerCompat.from(context)) {
                notify(notificationId, builder.build())
            }
        }
    }

    companion object {
        const val LOG_TAG = "TodayTasksNotification"
    }
}