package io.github.kn65op.domag_tasks.utils.platform

interface PlatformWrapper {
    fun showToast(message: String)
    fun getString(resourceId: Int): String
}