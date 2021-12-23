package ru.`object`.detection.util

import android.content.Context
import android.util.Log
import java.io.File


class Mask {
    private val name:String

    constructor(name: String) {
        this.name = name
    }

    fun save(data: ByteArray, path: String) {
        var folder = File(path)
        if(!folder.exists())
            folder.mkdir()
        File("$path/$name.png").writeBytes(data)
    }

    companion object {
        fun getCurrentMasks(path: String): Array<String> {
            val list = mutableListOf<String>()
            var folder = File(path)
            if(!folder.exists())
                folder.mkdir()
            File(path).walkTopDown().forEach { list.add(it.name.replace(".png", "")) }
            list.remove("Masks")
            return list.toTypedArray()
        }
    }
}