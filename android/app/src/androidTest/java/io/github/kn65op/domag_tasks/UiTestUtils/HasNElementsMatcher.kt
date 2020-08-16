package io.github.kn65op.domag_tasks.UiTestUtils

import android.view.View
import androidx.recyclerview.widget.RecyclerView
import androidx.test.espresso.matcher.BoundedMatcher
import org.hamcrest.Description
import org.hamcrest.Matcher

fun hasNElements(n: Int): Matcher<in View>? {
    return object : BoundedMatcher<View, RecyclerView>(RecyclerView::class.java) {
        override fun describeTo(description: Description?) {
            description?.appendText("has $n elements")
        }

        override fun matchesSafely(item: RecyclerView?): Boolean = item?.adapter?.itemCount == n
    }
}

