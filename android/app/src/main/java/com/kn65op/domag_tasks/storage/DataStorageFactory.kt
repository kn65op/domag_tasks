package com.kn65op.domag_tasks.storage

import android.content.Context
import com.kn65op.domag_tasks.tasks.JsonTaskDeserializer
import com.kn65op.domag_tasks.utils.platform.AndroidWrapper

class DataStorageFactory {
    fun createDriveDataStorageFactory(applicationContext: Context) = DriveDataStorage(
        applicationContext.filesDir,
        AndroidWrapper(applicationContext),
        taskDeserializer = JsonTaskDeserializer()
    )
}