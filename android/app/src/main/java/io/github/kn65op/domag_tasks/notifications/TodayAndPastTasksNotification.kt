package io.github.kn65op.domag_tasks.notifications

import android.app.NotificationManager
import android.app.PendingIntent
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.text.Html
import android.util.Log
import androidx.core.app.NotificationCompat
import androidx.core.app.NotificationManagerCompat
import androidx.core.app.TaskStackBuilder
import io.github.kn65op.domag_tasks.R
import io.github.kn65op.domag_tasks.gui.MainActivity
import io.github.kn65op.domag_tasks.storage.DataStorageFactory
import io.github.kn65op.domag_tasks.tasks.Tasks
import io.github.kn65op.domag_tasks.tasks.getOverdueNotDone
import io.github.kn65op.domag_tasks.tasks.getTodayNotDone

class TodayAndPastTasksNotification : BroadcastReceiver() {
    override fun onReceive(context: Context?, intent: Intent?) {
        Log.i(LOG_TAG, "Received trigger")
        if (context != null) {
            notifyTasks(context)
        } else {
            Log.e(LOG_TAG, """Can't notify, context is null""")
        }
    }

    fun notifyTasks(context: Context) {
        Log.i(LOG_TAG, "Notify today tasks")
        val tasks = DataStorageFactory().createDriveDataStorageFactory(context).loadTasks()
        val pair = createNotificationText(tasks, context)
        val tasksCount = pair.first
        val text = pair.second
        Log.i(LOG_TAG, "NEW")
        if (tasksCount > 0) {
            Log.i(LOG_TAG, "There is $tasksCount to notify, clear notification")
            val pendingIntent = createNotificationIntent(context)
            buildNotification(context, tasksCount, text, pendingIntent)
        } else {
            clearNotification(context)
        }
    }

    private fun clearNotification(context: Context) {
        Log.i(LOG_TAG, "There is no tasks to notify, clear notification")
        val notificationManager: NotificationManager =
            context.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        notificationManager.cancel(NotificationId.TodayTasksNotification.ordinal)
    }

    private fun createNotificationText(
        tasks: Tasks,
        context: Context
    ): Pair<Int, String> {
        val pastTasksToNotify = tasks.getOverdueNotDone()
        val todayTasksToNotify = tasks.getTodayNotDone()
        val tasksCount = pastTasksToNotify.size + todayTasksToNotify.size
        var text = ""
        if (pastTasksToNotify.isNotEmpty()) {
            text += pastTasksToNotify.fold(
                "${Html.fromHtml(
                    context.getString(R.string.overdue_tasks),
                    Html.FROM_HTML_MODE_COMPACT
                )}:"
            ) { acc, task -> "$acc\n- ${task.summary}" }
        }
        if (todayTasksToNotify.isNotEmpty()) {
            if (text.isNotEmpty()) {
                text += "\n"
            }
            text += todayTasksToNotify.fold("${context.getString(R.string.today_tasks)}: ") { acc, task -> "$acc\n- ${task.summary}" }
        }
        return Pair(tasksCount, text)
    }

    private fun createNotificationIntent(context: Context): PendingIntent? {
        val intent = Intent(context, MainActivity::class.java)
        return TaskStackBuilder.create(context).run {
            addNextIntentWithParentStack(intent)
            getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT)
        }
    }

    private fun buildNotification(
        context: Context,
        tasksCount: Int,
        text: String,
        pendingIntent: PendingIntent?
    ) {
        val builder =
            createNotificationBuilder(context, tasksCount, text, pendingIntent)

        val notificationId = NotificationId.TodayTasksNotification.ordinal
        with(NotificationManagerCompat.from(context)) {
            notify(notificationId, builder.build())
        }
    }

    private fun createNotificationBuilder(
        context: Context,
        tasksCount: Int,
        text: String,
        pendingIntent: PendingIntent?
    ): NotificationCompat.Builder {
        return NotificationCompat.Builder(context, NotificationChannels.TodayTasks.name)
            .setSmallIcon(R.drawable.ic_launcher_foreground)
            .setContentTitle(context.getString(R.string.tasks_to_do))
            .setContentText(
                context.resources.getQuantityString(
                    R.plurals.tasks_to_do_with_number,
                    tasksCount,
                    tasksCount
                )
            )
            .setStyle(NotificationCompat.BigTextStyle().bigText(text))
            .setPriority(NotificationCompat.PRIORITY_DEFAULT)
            .setContentIntent(pendingIntent)
            .setOnlyAlertOnce(true)
    }

    companion object {
        const val LOG_TAG = "TodayTasksNotification"
    }
}