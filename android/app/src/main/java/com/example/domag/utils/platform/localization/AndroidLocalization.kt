package com.example.domag.utils.platform.localization

import android.content.Context
import com.example.domag.R
import com.example.domag.utils.platform.localization.Localization.NotSupportedPlural

class AndroidLocalization(private val context: Context) :
    Localization {

    override fun getPluralWithNumberFor(baseText: String, numberOfItems: Int): String =
        context.resources.getQuantityString(
            toId(baseText),
            numberOfItems,
                    numberOfItems
        )

    private fun toId(baseText: String) = when (baseText) {
        "Week" -> R.plurals.each_week
        "Month" -> R.plurals.each_month
        "Day" -> R.plurals.each_day
        "Year" -> R.plurals.each_year
        else -> throw NotSupportedPlural("$baseText")
    }

}