package ru.`object`.detection


import androidx.test.espresso.DataInteraction
import androidx.test.espresso.ViewInteraction
import androidx.test.filters.LargeTest
import androidx.test.rule.ActivityTestRule
import androidx.test.rule.GrantPermissionRule
import androidx.test.runner.AndroidJUnit4
import android.view.View
import android.view.ViewGroup
import android.view.ViewParent

import androidx.test.InstrumentationRegistry.getInstrumentation
import androidx.test.espresso.Espresso.onData
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.Espresso.pressBack
import androidx.test.espresso.Espresso.openActionBarOverflowOrOptionsMenu
import androidx.test.espresso.contrib.RecyclerViewActions.actionOnItemAtPosition
import androidx.recyclerview.widget.RecyclerView.ViewHolder
import androidx.test.espresso.action.ViewActions.*
import androidx.test.espresso.assertion.ViewAssertions.*
import androidx.test.espresso.matcher.RootMatchers
import androidx.test.espresso.matcher.ViewMatchers.*

import org.tensorflow.lite.examples.detection.R

import org.hamcrest.Description
import org.hamcrest.Matcher
import org.hamcrest.Matchers
import org.hamcrest.Matchers.*
import org.hamcrest.TypeSafeMatcher
import org.hamcrest.core.IsInstanceOf
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import java.io.File

@LargeTest
@RunWith(AndroidJUnit4::class)
class MaskChoiceActivityTest {

    @Rule
    @JvmField
    var mActivityTestRule = ActivityTestRule(ObjectRecognitionActivity::class.java)

    @Rule
    @JvmField
    var mGrantPermissionRule =
        GrantPermissionRule.grant(
            "android.permission.CAMERA",
            "android.permission.WRITE_EXTERNAL_STORAGE"
        )

    @Test
    fun chooseMaskTest() {
        if(File("/storage/emulated/0/Android/media/Masks").list().isNullOrEmpty())
            return
        val floatingActionButton = onView(
            allOf(
                withId(R.id.masksButton),
                childAtPosition(
                    allOf(
                        withId(R.id.root_container),
                        childAtPosition(
                            withId(android.R.id.content),
                            0
                        )
                    ),
                    3
                ),
                isDisplayed()
            )
        )
        floatingActionButton.perform(click())

        val recyclerView = onView(
            allOf(
                withId(R.id.rvNumbers),
                childAtPosition(
                    withClassName(`is`("androidx.constraintlayout.widget.ConstraintLayout")),
                    0
                )
            )
        )
        recyclerView.perform(actionOnItemAtPosition<ViewHolder>(0, click()))
        onView(withText(startsWith("Текущая маска: "))).inRoot(
            RootMatchers.withDecorView(
                Matchers.not(
                    mActivityTestRule.getActivity().getWindow().getDecorView()
                )
            )
        ) .check(matches(isDisplayed()));
    }

    @Test
    fun loadMaskTest() {
        var file = File("/storage/emulated/0/Android/media/Masks/woodBoy.png")
        if(file.exists())
            file.delete()
        val floatingActionButton = onView(
            allOf(
                withId(R.id.masksButton),
                childAtPosition(
                    allOf(
                        withId(R.id.root_container),
                        childAtPosition(
                            withId(android.R.id.content),
                            0
                        )
                    ),
                    3
                ),
                isDisplayed()
            )
        )
        floatingActionButton.perform(click())
        val recyclerView = onView(
            allOf(
                withId(R.id.rvNumbers),
                childAtPosition(
                    withClassName(`is`("androidx.constraintlayout.widget.ConstraintLayout")),
                    0
                )
            )
        )
        recyclerView.perform(actionOnItemAtPosition<ViewHolder>(11, click()))
        onView(withText(startsWith("Текущая маска: "))).inRoot(
            RootMatchers.withDecorView(
                Matchers.not(
                    mActivityTestRule.getActivity().getWindow().getDecorView()
                )
            )
        ) .check(matches(isDisplayed()));
    }

    @Test
    fun loadMaskListTest() {
        if(File("/storage/emulated/0/Android/media/Masks").list().isNullOrEmpty())
            return
        val floatingActionButton = onView(
            allOf(
                withId(R.id.masksButton),
                childAtPosition(
                    allOf(
                        withId(R.id.root_container),
                        childAtPosition(
                            withId(android.R.id.content),
                            0
                        )
                    ),
                    3
                ),
                isDisplayed()
            )
        )
        floatingActionButton.perform(click())

        val textView = onView(
            allOf(
                withId(R.id.info_text), withText("woodBoy"),
                withParent(withParent(withId(R.id.rvNumbers))),
                isDisplayed()
            )
        )
        textView.check(matches(isDisplayed()))
    }

    private fun childAtPosition(
        parentMatcher: Matcher<View>, position: Int
    ): Matcher<View> {

        return object : TypeSafeMatcher<View>() {
            override fun describeTo(description: Description) {
                description.appendText("Child at position $position in parent ")
                parentMatcher.describeTo(description)
            }

            public override fun matchesSafely(view: View): Boolean {
                val parent = view.parent
                return parent is ViewGroup && parentMatcher.matches(parent)
                        && view == parent.getChildAt(position)
            }
        }
    }
}
