package com.example.domag.utils.platform.localization

interface Localization {
    class NotSupportedPlural(val s: String) : Throwable() {
        override fun toString(): String {
            return "Not found plural for $s"
        }
    }

    fun getPluralWithNumberFor(baseText: String, numberOfItems: Int): String
}