package ru.`object`.detection.util

import android.util.Log
import java.lang.StringBuilder
import java.net.Socket
import kotlin.concurrent.thread

class ServerCommunication {
    val host : String = "194.85.173.14"
    val port: Int = 7771

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
        client.getOutputStream().write("GETALL\n".toByteArray())
        client.getOutputStream().write(sendMasksString.toByteArray())
        client.getOutputStream().write("\nENDALL\n".toByteArray())
    }

    fun getMissingList(client: Socket): Array<String> {
        var maskArray = mutableListOf<String>()
        var stringResponse = String(client.getInputStream().readBytes(), Charsets.UTF_8)
        var respBlock = stringResponse.split("\n",limit =  2)
        if(respBlock[0] != "OKALL") {
            Log.e("123", "Неизвестная команда")
            Log.e("123", respBlock[0])
        }
        do {
            respBlock = respBlock[1].split("\n")
            maskArray.addAll(respBlock[0].split(" "))
        }
        while (respBlock.count() != 1 && respBlock[1] != "ENDALL")
        return maskArray.toTypedArray()
    }

    fun sendMaskDownloadRequest(maskName: String, client : Socket) {
        client.getOutputStream().write("GETONE\n".toByteArray())
        client.getOutputStream().write(maskName.toByteArray())
        client.getOutputStream().write("\nENDONE\n".toByteArray())
    }

    fun getMask(maskName: String, client: Socket) : Boolean {

        /*while(client.getInputStream().bufferedReader().readLine() != "OKONE") {}
        var mask = Mask(maskName)
        var data = client.getInputStream().readBytes()
        mask.save(data)
        return true*/
        var mask = Mask(maskName)
        var stringData : String = ""
        var stringResponse = String(client.getInputStream().readBytes(), Charsets.UTF_8)
        var respBlock = stringResponse.split("\n",limit =  2)
        if(respBlock[0] != "OKONE") {
            Log.e("123", "Неизвестная команда")
            Log.e("123", respBlock[0])
            return false
        }
        do {
            respBlock = respBlock[1].split("\n")
            var respblock1 = respBlock.subList(0, respBlock.count() - 2)
            stringData += respblock1.joinToString("\n")
        }
        while (respBlock.count() != 1 && respBlock.last() != "ENDALL")
        var data = stringData.toByteArray()
        mask.save(data)
        return true
    }
}