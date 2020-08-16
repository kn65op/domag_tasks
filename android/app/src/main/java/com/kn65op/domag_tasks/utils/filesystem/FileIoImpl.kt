package com.kn65op.domag_tasks.utils.filesystem

import java.io.File
import java.lang.Exception

class FileIoImpl : FileIo {
    override fun exists(file: File): Boolean = try {
        file.exists()
    } catch (e: Exception) {
        throw FileIoError("Unable to check file existence: ${e.message}")
    }

    override fun writeToFile(file: File, data: String) = try {
        file.writeText(data)
    } catch (e: Exception) {
        throw FileIoError("Unable to write to file: ${e.message}")
    }

    override fun readFromFile(file: File): String = try {
        file.readText()
    } catch (e: Exception) {
        throw FileIoError("Unable to read from file: ${e.message}")
    }
}