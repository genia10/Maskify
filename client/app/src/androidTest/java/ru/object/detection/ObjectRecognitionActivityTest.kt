package ru.`object`.detection


import android.view.View
import android.view.ViewGroup
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.*
import androidx.test.espresso.assertion.ViewAssertions.*
import androidx.test.espresso.matcher.ViewMatchers.*
import androidx.test.filters.LargeTest
import androidx.test.rule.ActivityTestRule
import androidx.test.rule.GrantPermissionRule
import androidx.test.runner.AndroidJUnit4
import org.hamcrest.Description
import org.hamcrest.Matcher
import org.hamcrest.Matchers.allOf
import org.hamcrest.TypeSafeMatcher
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.tensorflow.lite.examples.detection.R

@LargeTest
@RunWith(AndroidJUnit4::class)
class ObjectRecognitionActivityTest {

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
    fun detectionResultOverlayAndPreviewTest() {
        val view = onView(
            allOf(
                withId(R.id.result_overlay),
                withParent(
                    allOf(
                        withId(R.id.frameLayout1),
                        withParent(withId(R.id.root_container))
                    )
                ),
                isDisplayed()
            )
        )
        view.check(matches(isDisplayed()))

        val frameLayout = onView(
            allOf(
                withId(R.id.preview_view),
                withParent(
                    allOf(
                        withId(R.id.frameLayout1),
                        withParent(withId(R.id.root_container))
                    )
                ),
                isDisplayed()
            )
        )
        frameLayout.check(matches(isDisplayed()))
    }

    @Test
    fun takePhotoTest() {
        val floatingActionButton = onView(
            allOf(
                withId(R.id.captureButton), withContentDescription("Capture"),
                childAtPosition(
                    allOf(
                        withId(R.id.root_container),
                        childAtPosition(
                            withId(android.R.id.content),
                            0
                        )
                    ),
                    2
                ),
                isDisplayed()
            )
        )
        floatingActionButton.perform(click())

        val imageView = onView(
            allOf(
                withId(R.id.iv_capture),
                withParent(
                    allOf(
                        withId(R.id.root_container),
                        withParent(withId(android.R.id.content))
                    )
                ),
                isDisplayed()
            )
        )
        imageView.check(matches(isDisplayed()))
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
