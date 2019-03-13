package com.example.domag

import android.view.View
import androidx.recyclerview.widget.RecyclerView
import androidx.test.core.app.ApplicationProvider
import androidx.test.core.app.launchActivity
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.Espresso.openActionBarOverflowOrOptionsMenu
import androidx.test.espresso.action.ViewActions.*
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.matcher.BoundedMatcher
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.ext.junit.runners.AndroidJUnit4
import org.hamcrest.Description
import org.hamcrest.Matcher
import org.junit.Test
import org.junit.runner.RunWith
import java.lang.Thread.sleep

@RunWith(AndroidJUnit4::class)
class MainActivityUiTest {
    private fun atPosition(i: Int, withText: Matcher<View>?): Matcher<in View>? {
        checkNotNull(withText)

        return object : BoundedMatcher<View, RecyclerView>(RecyclerView::class.java) {
            override fun describeTo(description: Description?) {
                description?.appendText("has item on position $i ")
                withText.describeTo(description)
            }

            override fun matchesSafely(item: RecyclerView?): Boolean {
                val viewHolder = item?.findViewHolderForAdapterPosition(i)
                viewHolder ?: return false
                return withText.matches(viewHolder.itemView)
            }
        }
    }

    private fun hasNoElements(): Matcher<in View>? {
        return object : BoundedMatcher<View, RecyclerView>(RecyclerView::class.java) {
            override fun describeTo(description: Description?) {
                description?.appendText("is empty")
            }

            override fun matchesSafely(item: RecyclerView?): Boolean = item?.adapter?.itemCount == 0
        }
    }

    private val firstTask = "A first task"
    private val secondTask = "B second task"
    private val thirdTask = "C task"

    @Test
    fun whenNoTasks_shouldBeEmpty() {
        prepareEmptyTasks()

        onView(withId(R.id.MainTasksList)).check(matches(hasNoElements()))
    }

    @Test
    fun whenThereIsTask_shouldBeOnList() {
        prepareOneTask()
        checkTaskOnPosition(0, firstTask)
    }

    private fun checkTaskOnPosition(position: Int, summary: String) {
        onView(withId(R.id.MainTasksList)).check(matches(atPosition(position, hasDescendant(withText(summary)))))
    }

    @Test
    fun whenThereAreFewTassk_shouldBeSorted() {
        prepareEmptyTasks()
        createTask(thirdTask)
        createTask(firstTask)
        createTask(secondTask)

        checkTaskOnPosition(0, firstTask)
        checkTaskOnPosition(1, secondTask)
        checkTaskOnPosition(2, thirdTask)
    }

    private fun prepareOneTask() {
        prepareEmptyTasks()
        createTask(firstTask)
    }

    private fun createTask(taskSummary: String) {
        onView(withId(R.id.addNewTaskButton)).perform(click())
        onView(withId(R.id.newTaskName)).perform(typeText(taskSummary), closeSoftKeyboard())
        onView(withId(R.id.addTaskButton)).perform(click())
        sleep(1000) //TODO: Seems that another add task cannot be run so fast
        // as click for add new task does not work fine
    }

    private fun prepareEmptyTasks() {
        launchActivity<MainActivity>()
        openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext())
        onView(withText("Remove tasks")).perform(click())
    }
}