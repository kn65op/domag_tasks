package com.kn65op.domag_tasks.UiTestUtils

import java.time.ZonedDateTime

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
const val weeksAdvance = 8
val nowPlusWeeks: ZonedDateTime = now.plusWeeks(weeksAdvance.toLong())
const val monthsAdvance = 3
val nowPlusMonths: ZonedDateTime = now.plusMonths(monthsAdvance.toLong())
const val yearsAdvance = 821
val nowPlusYears: ZonedDateTime = now.plusYears(yearsAdvance.toLong())
const val years = "years"
