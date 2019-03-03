package com.example.domag.utils.filesystem

import java.io.File

class FileIoError(message: String) : Exception(message)

interface FileIo {
    fun exists(file: File): Boolean
    fun writeToFile(file: File, data: String)
    fun readFromFile(file: File): String
}