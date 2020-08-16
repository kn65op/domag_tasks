package io.github.kn65op.domag_tasks.utils.platform.localization

interface Localization {
    class NotSupportedPlural(private val s: String) : Throwable() {
        override fun toString(): String {
            return "Not found plural for $s"
        }
    }

    fun getPluralWithNumberFor(baseText: String, numberOfItems: Int): String
}