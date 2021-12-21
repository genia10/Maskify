package ru.`object`.detection

import android.util.Log
import org.junit.Assert.*

import org.junit.Test
import ru.`object`.detection.util.ServerCommunication

class ServerCommunicationTest {

    @Test
    fun sendCurrentMasksTest() {
        var server = ServerCommunication()
        var availableMasks = arrayOf("human")
        var result = false;
        server.connect()
        if(server.checkServerAlive())
            result = server.sendCurrentMasks(availableMasks)
        else Log.e("123", "Не удалось отправить текущие маски")
        server.disconnect()
        assertEquals(true,result)
    }

    @Test
    fun getMissingListTest() {
        var server = ServerCommunication()
        var availableMasks = arrayOf("human")
        var absent = arrayOf<String>()
        server.connect()
        if(server.checkServerAlive())
            if(server.sendCurrentMasks(availableMasks))
                absent = server.getMissingList()
        else Log.e("123", "Не удалось отправить текущие маски и получить отсутствующие")
        server.disconnect()
        assertNotEquals(0,absent.size)
    }

    @Test
    fun sendMaskDownloadRequestTest() {
        var server = ServerCommunication()
        server.connect()
        assertEquals(true, server.sendMaskDownloadRequest("human"))
        server.disconnect()
    }

    @Test
    fun getMaskTest() {
        var server = ServerCommunication()
        server.connect()
        server.sendMaskDownloadRequest("human")
        assertEquals(true,server.getMask("human", true))
        server.disconnect()
    }
}