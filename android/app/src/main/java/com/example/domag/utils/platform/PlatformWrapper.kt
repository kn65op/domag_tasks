package com.example.domag.utils.platform

interface PlatformWrapper {
    fun showToast(message: String)
    fun getString(resourceId: Int): String
}