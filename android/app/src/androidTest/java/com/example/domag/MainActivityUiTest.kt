package com.example.domag

import android.view.View
import androidx.recyclerview.widget.RecyclerView
import androidx.test.core.app.launchActivity
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.matcher.BoundedMatcher
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.ext.junit.runners.AndroidJUnit4
import org.hamcrest.Description
import org.hamcrest.Matcher
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class MainActivityUiTest {
    @Test
    fun shouldBeCreated() {
        launchActivity<MainActivity>()
        onView(withId(R.id.MainTasksList)).check(matches(atPosition(0, hasDescendant(withText("SIMPLE TASK(79): SomeTask")))))
    }

    private fun atPosition(i: Int, withText: Matcher<View>?): Matcher<in View>? {
        checkNotNull(withText)

        return object : BoundedMatcher<View, RecyclerView>(RecyclerView::class.java) {
            override fun describeTo(description: Description?) {
                description?.appendText("has item on position $i ")
                withText?.describeTo(description)
            }

            override fun matchesSafely(item: RecyclerView?): Boolean {
                val viewHolder = item?.findViewHolderForAdapterPosition(i)
                viewHolder ?: return false
                return withText?.matches(viewHolder.itemView) ?: return false
            }
        }
    }
}