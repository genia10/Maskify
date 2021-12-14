package ru.`object`.detection.util

import android.util.Log
import java.io.File
import java.io.FileOutputStream
import java.io.InputStream
import java.io.OutputStream
import java.lang.StringBuilder
import java.net.Socket
import java.util.*
import kotlin.concurrent.thread

class ServerCommunication {
    private val host : String = "194.85.173.14"
    private val port: Int = 7777
    private var client: Socket? = null
    private var inputStream : InputStream? = null
    private var outputStream : OutputStream? = null

    fun connect()
    {
        client = Socket(host, port)
        inputStream = client!!.getInputStream()
        outputStream = client!!.getOutputStream()
    }

    fun disconnect()
    {
        client?.close()
        inputStream
    }

    fun checkServerAlive() : Boolean {
        var reader = Scanner(inputStream)
        outputStream?.write("HELLO\n".toByteArray())
        var res = reader.nextLine() == "HELLOOK"
        //reader.close()
        return res
    }

    fun sendCurrentMasks(masks: Array<String>)
    {
        var list = masks.toMutableList().toString()
        var sendMasksString = list.substring(1, list.toString().length - 1).replace(", ", " ")
        outputStream?.write("GETALL\n".toByteArray())
        outputStream?.write(sendMasksString.toByteArray())
        outputStream?.write("\nENDALL\n".toByteArray())
    }

    fun getMissingList(): Array<String> {
        var maskArray = mutableListOf<String>()
        var reader = Scanner(inputStream)
        if(reader.nextLine() != "OKALL") {
            return emptyArray()
        }
        var string = reader.nextLine()
        if(string == "ENDALL") {
            return emptyArray()
        }
        maskArray.addAll(string.split(" "))
        outputStream?.write("EXIT\n".toByteArray())
        return maskArray.toTypedArray()
    }

    fun sendMaskDownloadRequest(maskName: String) {
        outputStream?.write("GETONE\n".toByteArray())
        outputStream?.write(maskName.toByteArray())
        outputStream?.write("\nENDONE\n".toByteArray())
    }

    fun addBytes(list: MutableList<Byte>, bytes: ByteArray, count:Int) : MutableList<Byte> {
        var k = 0;
        for(byte in bytes) {
            if(k++ == count) break;
            list.add(byte)
        }
        return list
    }

    fun getMask(maskName: String) : Boolean {
        var mask = Mask(maskName)
        var reader = Scanner(inputStream)
        var a = reader.nextLine()
        //var out = File("/storage/emulated/0/Android/media/org.tensorflow.lite.examples.detection/Masks/$maskName.png").outputStream()
        var data = mutableListOf<Byte>()
        val bytes = ByteArray(16178)
        var count: Int =0
        while (inputStream!!.read(bytes).also({ count = it }) > 0) {
            var lastBytes = bytes.copyOfRange(count-7, count-1)
            if(String(lastBytes, Charsets.UTF_8) == "ENDONE") {
                data = addBytes(data, bytes, count - 8)
                //out.write(bytes.copyOfRange(0, count - 8), 0, count - 8)
                break
            }
            data = addBytes(data, bytes, count)
            //out.write(bytes, 0, count)
        }
        mask.save(data.toByteArray())
        outputStream?.write("EXIT\n".toByteArray())
        return true
    }
}