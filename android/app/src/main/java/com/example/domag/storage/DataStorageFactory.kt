package com.example.domag.storage

import android.content.Context
import com.example.domag.tasks.JsonTaskDeserializer
import com.example.domag.utils.platform.AndroidWrapper

class DataStorageFactory {
    fun createDriveDataStorageFactory(applicationContext : Context) = DriveDataStorage( applicationContext.filesDir, AndroidWrapper(applicationContext), taskDeserializer = JsonTaskDeserializer() )
}