package com.example.domag.tasks

import java.io.Serializable
import java.time.ZonedDateTime
import com.example.domag.utils.platform.localization.Localization

typealias Id = Int

interface Task : Serializable {
    var id: Id
    var summary: String
    val type: String
    var done: Boolean
    var nextDeadline: ZonedDateTime

    fun nextDeadlineText(localization: Localization): String
    fun serializeToString(): String
}
