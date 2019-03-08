package com.example.domag.tasks

import java.io.Serializable
import java.time.ZonedDateTime

typealias Id = Int

interface Task : Serializable {
    var id: Id
    var summary: String
    val type: String
    var done: Boolean
    var nextDeadline: ZonedDateTime

    fun serializeToString(): String
}
