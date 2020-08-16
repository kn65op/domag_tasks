package com.kn65op.domag_tasks.UiTestUtils

import android.view.View
import androidx.recyclerview.widget.RecyclerView
import androidx.test.espresso.matcher.BoundedMatcher
import org.hamcrest.Description
import org.hamcrest.Matcher

fun atPosition(i: Int, withText: Matcher<View>?): Matcher<in View>? {
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
