package ru.`object`.detection

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import org.tensorflow.lite.examples.detection.R

class MaskChoiceActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mask_choice)
        supportActionBar?.hide()
    }
}