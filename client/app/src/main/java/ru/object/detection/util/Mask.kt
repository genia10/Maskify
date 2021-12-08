package ru.`object`.detection.util

import android.content.Context
import android.util.Log
import java.io.File


class Mask {
    private val name:String

    constructor(name: String) {
        this.name = name
    }

    fun save(data: ByteArray) {
        var folder = File("/storage/emulated/0/Android/media/org.tensorflow.lite.examples.detection/Masks")
        if(!folder.exists())
            folder.mkdir()
        File("/storage/emulated/0/Android/media/org.tensorflow.lite.examples.detection/Masks/$name").writeBytes(data)
    }

    companion object {
        fun getCurrentMasks(): Array<String> {
            val list = mutableListOf<String>()
            var folder = File("/storage/emulated/0/Android/media/org.tensorflow.lite.examples.detection/Masks")
            if(!folder.exists())
                folder.mkdir()
            File("/storage/emulated/0/Android/media/org.tensorflow.lite.examples.detection/Masks").walkTopDown().forEach { list.add(it.name.replace(".png", "")) }
            list.remove("Masks")
            return list.toTypedArray()
        }
    }
}