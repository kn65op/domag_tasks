package com.example.domag.tasks

import java.time.ZonedDateTime

typealias Id = Int

interface Task {
    var id: Id
    val summary: String
    val type: String
    var done : Boolean
    val nextDeadline: ZonedDateTime

    fun serializeToString(): String
}
