package com.kn65op.domag_tasks.notifications

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.util.Log

class RestartAlarmsOnBoot : BroadcastReceiver() {

    override fun onReceive(context: Context?, intent: Intent?) {
        if (intent?.action == "android.intent.action.BOOT_COMPLETED") {
            Log.i(LOG_TAG, "System boot - set alarm")
            Alarm().start(context)
            if (context != null) {
                Notifications().registerChannels(context)
            } else {
                Log.e(LOG_TAG, "Context is empty, cannot register notification channel")
            }
        }
    }

    companion object {
        const val LOG_TAG = "RestartAlarmsOnBoot"
    }
}