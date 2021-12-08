package ru.`object`.detection.util.view

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.util.Log
import android.view.View
import ru.`object`.detection.camera.ObjectDetectorAnalyzer

class RecognitionResultOverlayView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : View(context, attrs, defStyleAttr) {

    private val boxPaint = Paint().apply {
        color = Color.GREEN
        style = Paint.Style.STROKE
        strokeWidth = 2f
    }
    private val boxPaint2 = Paint().apply {
        color = Color.RED
        style = Paint.Style.STROKE
        strokeWidth = 2f
    }
    private val pointPaint = Paint().apply {
        color = Color.RED
        style = Paint.Style.STROKE
        strokeWidth = 2f
    }
    private val textPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = Color.GREEN
        textSize = 25f
    }

    private val textPaint2 = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = Color.RED
        textSize = 100f
    }

    private var result: ObjectDetectorAnalyzer.Result? = null

    fun updateResults(result: ObjectDetectorAnalyzer.Result) {
        this.result = result
        invalidate()
    }

    override fun onDraw(canvas: Canvas) {
        val result = result ?: return

        val scaleFactorX = measuredWidth / result.imageWidth.toFloat()
        val scaleFactorY = measuredHeight / result.imageHeight.toFloat()

        Log.e("Width", "$measuredWidth $measuredHeight")

        var tmp = 0
        var it = 0
//        result.objects.forEach { obj ->
//            val left = obj.location.left * scaleFactorX
//            val top = obj.location.top * scaleFactorY
//            val right = obj.location.right * scaleFactorX
//            val bottom = obj.location.bottom * scaleFactorY
//
//            canvas.drawRect(left, top, right, bottom, boxPaint)
//            canvas.drawText(obj.text, left, top - 25f, textPaint)
//
//        }

        result.keypoints?.forEach { obj ->
            for (i in 0 until 9) {
                val left = result.objects[obj.id].location.left * scaleFactorX
                val top = result.objects[obj.id].location.top * scaleFactorY
                val right = result.objects[obj.id].location.right * scaleFactorX
                val bottom = result.objects[obj.id].location.bottom * scaleFactorY
                canvas.drawRect(left, top, right, bottom, boxPaint2)
                val cur_w = right - left
                val cur_h = bottom - top

                val scaleFactorXKey = cur_w / 224.toFloat()
                val scaleFactorYKey = cur_h / 224.toFloat()
                //            val x = (result.keypoints?.get(2 * i) ?: 0.0) * scaleFactorXKey
                //            val y = (result.keypoints?.get(2 * i + 1) ?: 0.0) * scaleFactorYKey
                val x = left + (obj.keypoints?.get(2 * i) ?: 0.0.toFloat()) * scaleFactorXKey
                val y = top + (obj.keypoints?.get(2 * i + 1) ?: 0.0.toFloat()) * scaleFactorYKey

                //                canvas.drawPoint(x, y, pointPaint)
                canvas.drawText("x", x, y, textPaint2)
                canvas.drawCircle(x, y, 40.0.toFloat(), pointPaint)
            }
            it += 1
        }
//        for(i in 0 until 9){
////            val scaleFactorXKey = 480 / 224.toFloat()
////            val scaleFactorYKey = 640 / 224.toFloat()
//            val scaleFactorXKey = 640 / 224.toFloat()
//            val scaleFactorYKey = 480 / 224.toFloat()
//
//            //            val x = (result.keypoints?.get(2 * i) ?: 0.0) * scaleFactorXKey
//            //            val y = (result.keypoints?.get(2 * i + 1) ?: 0.0) * scaleFactorYKey
//            val x = (result.keypoints?.get(2 * i) ?: 0.0.toFloat()) * scaleFactorXKey
//            val y = (result.keypoints?.get(2 * i + 1) ?: 0.0.toFloat()) * scaleFactorYKey
//
//            //                canvas.drawPoint(x, y, pointPaint)
//            canvas.drawText("x", x, y, textPaint2)
//            canvas.drawCircle(x, y, 40.0.toFloat(), pointPaint)
//        }
    }
//        Log.e("TMP", tmp.toString())
//        result.keypoints?.forEach { obj ->
//            for (i in 0 until 9) {
//
//                //            val x = (result.keypoints?.get(2 * i) ?: 0.0) * scaleFactorXKey
//                //            val y = (result.keypoints?.get(2 * i + 1) ?: 0.0) * scaleFactorYKey
//                val x = obj[2 * i] * scaleFactorXKey
//                val y = obj[2 * i + 1] * scaleFactorYKey
//
//                //                canvas.drawPoint(x, y, pointPaint)
//                canvas.drawText("x", x, y, textPaint2)
//                canvas.drawCircle(x, y, 40.0.toFloat(), pointPaint)
//
//            }
//        }
}


