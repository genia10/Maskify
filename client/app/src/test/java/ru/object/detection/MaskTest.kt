package ru.`object`.detection

import org.junit.Assert.*

import org.junit.Test
import ru.`object`.detection.util.Mask
import java.io.File
import java.nio.file.Paths

class MaskTest {

    @Test
    fun saveTest() {
        var maskname = "testMask"
        var mask = Mask(maskname)
        var path = Paths.get("").toAbsolutePath().toString()
        mask.save("testContent".toByteArray(), path)
        assertEquals(true, File("$path/$maskname.png").exists())
        File("$path/$maskname.png").delete()
    }

    @Test
    fun getCurrentMasksTest() {
        var path = Paths.get("").toAbsolutePath().toString() +"/Masks"
        File(path).mkdir()
        File("$path/123.png").writeText("testContent1")
        File("$path/456.png").writeText("testContent2")
        var masks = Mask.getCurrentMasks(path)
        assertEquals(2, masks.size)
        assertEquals("123", masks[0])
        assertEquals("456", masks[1])
        File("$path").deleteRecursively()
    }

    @Test
    fun getCurrentMasksNegative() {
        var path = Paths.get("").toAbsolutePath().toString() +"/Masks"
        File(path).mkdir()
        assertEquals(0, Mask.getCurrentMasks(path).size)
        File("$path").deleteRecursively()
    }
}