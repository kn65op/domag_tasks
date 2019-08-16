package com.example.domag.UiTestUtils

import java.time.ZonedDateTime
import java.time.format.TextStyle
import java.util.*

const val year1 = 2019
const val month1 = 2
const val monthName1 = "February"
const val month2 = 7
const val monthName2 = "July"
const val day1 = 3
const val day2 = 8
const val day3 = 28
const val zeroTask = "0 task Zero"
const val firstTask = "A first task"
const val secondTask = "B second task"
const val thirdTask = "C task"
const val threeTasks = 3
val date1 = Date(day1, month1, year1, monthName1)
val date2 = Date(day2, month2, year1, monthName2)
val date3 = Date(day3, month2, year1, monthName2)
const val daysAdvance = 8
val now: ZonedDateTime = ZonedDateTime.now()
val nowPlusDays: ZonedDateTime = now.plusDays(daysAdvance.toLong())
val dateAdvancedByDays =
    Date(
        nowPlusDays.dayOfMonth,
        nowPlusDays.monthValue,
        nowPlusDays.year,
        nowPlusDays.month.getDisplayName(TextStyle.FULL, Locale.ENGLISH)
    )

