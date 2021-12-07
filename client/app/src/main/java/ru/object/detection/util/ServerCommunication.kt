package ru.`object`.detection.util

import android.util.Log
import java.net.Socket
import kotlin.concurrent.thread

class ServerCommunication {
    val host : String = "194.85.173.14"
    val port: Int = 7777

    fun connect() : Socket
    {
        return Socket(host, port)
    }

    fun disconnect(socket : Socket)
    {
        socket.close()
    }

    fun sendCurrentMasks(masks: Array<String>, client : Socket)
    {
        var list = masks.toMutableList().toString()
        var sendMasksString = list.substring(1, list.toString().length - 1).replace(", ", " ")
        client.getOutputStream().write(sendMasksString.toByteArray())
    }

    fun getMissingList(client: Socket): Array<String> {
        var stringResponse = String(client.getInputStream().readBytes(), Charsets.UTF_8)
        return stringResponse.split(" ").toTypedArray()
    }
}