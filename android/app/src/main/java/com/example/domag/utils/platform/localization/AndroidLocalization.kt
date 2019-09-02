package com.example.domag.utils.platform.localization

import android.content.Context
import com.example.domag.R
import com.example.domag.utils.platform.localization.Localization.NotSupportedPlural

class AndroidLocalization(private val context: Context) :
    Localization {

    override fun getPluralFor(baseText: String, numberOfItems: Int): String =
        context.resources.getQuantityString(
            toId(baseText),
            numberOfItems
        )

    private fun toId(baseText: String) = when (baseText) {
        "Week" -> R.plurals.week
        "Month" -> R.plurals.month
        "Day" -> R.plurals.day
        "Year" -> R.plurals.year
        else -> throw NotSupportedPlural("$baseText")
    }

}
