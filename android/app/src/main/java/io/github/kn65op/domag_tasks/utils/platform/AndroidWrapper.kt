package io.github.kn65op.domag_tasks.utils.platform

import android.content.Context
import android.widget.Toast

class AndroidWrapper(private val androidContext: Context) : PlatformWrapper {
    override fun showToast(message: String) {
        Toast.makeText(androidContext, message, Toast.LENGTH_LONG).show()
    }

    override fun getString(resourceId: Int): String = androidContext.getString(resourceId)
}