package com.example.domag.tasks

import java.time.ZonedDateTime

typealias Id = Int

interface Task {
    var id: Id
    val name: String
    val type: String
    val nextDeadline: ZonedDateTime

    fun serializeToString(): String
}
