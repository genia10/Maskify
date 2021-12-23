package ru.`object`.detection.camera

import android.content.Context
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Matrix
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import ru.`object`.detection.detection.DetectionResult
import ru.`object`.detection.detection.ObjectDetector
import ru.`object`.detection.util.ImageUtil
import ru.`object`.detection.util.YuvToRgbConverter
import java.util.concurrent.atomic.AtomicInteger

class ObjectDetectorAnalyzer(
    private val context: Context,
    private val config: Config,
    private val onDetectionResult: (Result) -> Unit
) : ImageAnalysis.Analyzer {

    companion object {
        private const val TAG = "ObjectDetectorAnalyzer"
        private val DEBUG = false
    }

    private val iterationCounter = AtomicInteger(0)

    private val debugHelper = DebugHelper(
        saveResult = false,
        context = context,
        resultHeight = config.inputSize,
        resultWidth = config.inputSize
    )

    private val yuvToRgbConverter = YuvToRgbConverter(context)

    private val uiHandler = Handler(Looper.getMainLooper())

    private var inputArray = IntArray(config.inputSize * config.inputSize)
    private var inputArrayKeypoints = IntArray(224 * 224)
    private var objectDetector: ObjectDetector? = null

    private var rgbBitmap: Bitmap? = null
    private var resizedBitmap =
        Bitmap.createBitmap(config.inputSize, config.inputSize, Bitmap.Config.ARGB_8888)
    private var resizedBitmapKeypoints = Bitmap.createBitmap(224, 224, Bitmap.Config.ARGB_8888)

    private var matrixToInput: Matrix? = null

    override fun analyze(image: ImageProxy) {

        val rotationDegrees = image.imageInfo.rotationDegrees

        val iteration = iterationCounter.getAndIncrement()

        val rgbBitmap = getArgbBitmap(image.width, image.height)

        yuvToRgbConverter.yuvToRgb(image, rgbBitmap)

        val transformation = getTransformation(rotationDegrees, image.width, image.height)
        val transormation_keypoints =
            getTransformationKeypoints(rotationDegrees, image.width, image.height)
        image.close()
//        var tImage = TensorImage(DataType.FLOAT32)
//        var tensorImage = tImage.load(resized)
//        var byteBuffer = tensorImage.buffer
        Canvas(resizedBitmap).drawBitmap(rgbBitmap, transformation, null)
//        Canvas(resizedBitmap).draw
//        Bitmap.createBitmap()
        Canvas(resizedBitmapKeypoints).drawBitmap(rgbBitmap, transormation_keypoints, null)

        ImageUtil.storePixels(resizedBitmap, inputArray)
        ImageUtil.storePixels(resizedBitmapKeypoints, inputArrayKeypoints)

        val objects = detect(inputArray)
        val all_keypoints = mutableListOf<KeypointData>()
        var flag = false
//        var res = NewResult()
        var res_objects = mutableListOf<DetPoints>()
        var it = 0
        for (obj in objects) {
            if (obj.title == "cat") {
                val scaleFactorX = image.width / 300
                val scaleFactorY = image.height / 300
                val w = ((obj.location.right - obj.location.left) * scaleFactorX).toInt()
                val h = ((obj.location.bottom - obj.location.top) * scaleFactorY).toInt()
                val l = (obj.location.left * scaleFactorX).toInt()
                val u = (obj.location.top * scaleFactorY).toInt()
                val cur_transformation = getTransformationKeypoints(rotationDegrees, w, h)
                if (!(h in 1..image.height && w in 0..image.width && l in 0..image.width && u in 1..image.height && h + u in 1..image.height && w + l in 1..image.width))
                    continue
                Log.e(
                    "width height",
                    rgbBitmap.width.toString() + " " + rgbBitmap.height.toString()
                )
                val cur_bitmap1 = Bitmap.createBitmap(rgbBitmap, l, u, w, h)

                Canvas(resizedBitmapKeypoints).drawBitmap(cur_bitmap1, cur_transformation, null)
                ImageUtil.storePixels(resizedBitmapKeypoints, inputArrayKeypoints)
                flag = true

                val keypoints = detectKeypoints(inputArrayKeypoints)
                res_objects.add(
                    DetPoints(
                        obj,
                        keypoints
                    )
                )
                all_keypoints.add(KeypointData(keypoints, it))
            }
            if (!flag)
                res_objects.add(DetPoints(obj, null))
            it++
        }
//        val all_keypoints = detectKeypoints(inputArrayKeypoints)
        if (DEBUG) {
            debugHelper.saveResult(iteration, resizedBitmap, objects)
        }
        Log.d(TAG, "detection objects($iteration): $all_keypoints")
        Log.d(TAG, "detection objects($iteration): $objects")

        val result = Result(
            objects = objects,
            imageWidth = config.inputSize,
            imageHeight = config.inputSize,
            imageRotationDegrees = rotationDegrees,
            keypoints = all_keypoints
        )
//        val new_result = NewResult(
//            objects = res_objects,
//            imageWidth = config.inputSize,
//            imageHeight = config.inputSize,
//            imageRotationDegrees = rotationDegrees
//        )


        uiHandler.post {
            onDetectionResult.invoke(result)
        }
    }

    private fun getTransformation(rotationDegrees: Int, srcWidth: Int, srcHeight: Int): Matrix {
        var toInput = matrixToInput
        if (toInput == null) {
            toInput = ImageUtil.getTransformMatrix(
                rotationDegrees,
                srcWidth,
                srcHeight,
                config.inputSize,
                config.inputSize
            )
            matrixToInput = toInput
        }
        return toInput
    }

    private fun getTransformationKeypoints(
        rotationDegrees: Int,
        srcWidth: Int,
        srcHeight: Int
    ): Matrix {
        var toInput = matrixToInput
        if (toInput == null) {
            toInput = ImageUtil.getTransformMatrix(
                rotationDegrees,
                srcWidth,
                srcHeight,
                224,
                224
            )
            matrixToInput = toInput
        }
        return toInput
    }

    private fun getArgbBitmap(width: Int, height: Int): Bitmap {
        var bitmap = rgbBitmap
        if (bitmap == null) {
            bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888) as Bitmap
            rgbBitmap = bitmap
        }
        return bitmap
    }

    private fun detect(inputArray: IntArray): List<DetectionResult> {
        var detector = objectDetector
        if (detector == null) {
            detector = ObjectDetector(
                assetManager = context.assets,
                isModelQuantized = config.isQuantized,
                inputSize = config.inputSize,
                labelFilename = config.labelsFile,
                modelFilename = config.modelFile,
                numDetections = config.numDetection,
                minimumConfidence = config.minimumConfidence,
                numThreads = 1,
                useNnapi = false
            )
            objectDetector = detector
        }
        var output = detector.detect(inputArray)
        return output
    }

    private fun detectKeypoints(inputArray: IntArray): List<Float> {
        var detector = objectDetector
        if (detector == null) {
            detector = ObjectDetector(
                assetManager = context.assets,
                isModelQuantized = config.isQuantized,
                inputSize = config.inputSize,
                labelFilename = config.labelsFile,
                modelFilename = config.modelFile,
                numDetections = config.numDetection,
                minimumConfidence = config.minimumConfidence,
                numThreads = 1,
                useNnapi = false
            )
            objectDetector = detector
        }
        var output = detector.detectKeypoints(inputArray)
        return output
    }

    data class Config(
        val minimumConfidence: Float,
        val numDetection: Int,
        val inputSize: Int,
        val isQuantized: Boolean,
        val modelFile: String,
        val labelsFile: String
    )

    data class Result(
        val objects: List<DetectionResult>,
        val imageWidth: Int,
        val imageHeight: Int,
        val imageRotationDegrees: Int,
        val keypoints: List<KeypointData>?
//        val keypoints: List<Float>?
    )

    data class DetPoints(
        val cur_object: DetectionResult,
        val keypoints: List<Float>?
    )

    data class NewResult(
        val objects: List<DetPoints>,
        val imageWidth: Int,
        val imageHeight: Int,
        val imageRotationDegrees: Int
    )

    data class KeypointData(
        val keypoints: List<Float>?,
        val id: Int

    )
}