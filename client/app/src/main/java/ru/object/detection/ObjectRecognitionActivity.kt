package ru.`object`.detection

import android.Manifest
import android.graphics.Bitmap
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.os.Environment.DIRECTORY_DCIM
import android.os.Environment.DIRECTORY_PICTURES
import android.util.Log
import android.util.Size
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.*
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import androidx.lifecycle.LifecycleOwner
import com.google.android.material.snackbar.Snackbar
import kotlinx.android.synthetic.main.activity_object_recognition.*
import org.tensorflow.lite.examples.detection.R
import ru.`object`.detection.camera.CameraPermissionsResolver
import ru.`object`.detection.camera.ObjectDetectorAnalyzer
import java.io.File
import java.text.SimpleDateFormat
import java.util.*
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

class ObjectRecognitionActivity : AppCompatActivity() {

    private lateinit var previewView: PreviewView

    private var imageCapture: ImageCapture? = null

    private lateinit var executor: ExecutorService

    private val cameraPermissionsResolver = CameraPermissionsResolver(this)

    private val objectDetectorConfig = ObjectDetectorAnalyzer.Config(
            minimumConfidence = 0.5f,
            numDetection = 10,
            inputSize = 300,
            isQuantized = true,
            modelFile = "detect.tflite",
            labelsFile = "labelmap.txt"
    )
//    private
// 123
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_object_recognition)
        executor = Executors.newSingleThreadExecutor()

        previewView = findViewById(R.id.preview_view)

        cameraPermissionsResolver.checkAndRequestPermissionsIfNeeded(
                onSuccess = {
                    getProcessCameraProvider(::bindCamera)
                },
                onFail = ::showSnackbar
        )
    }

    override fun onDestroy() {
        executor.shutdown()
        super.onDestroy()
    }

    private fun bindCamera(cameraProvider: ProcessCameraProvider) {
        val preview = Preview.Builder()
                .setTargetAspectRatio(AspectRatio.RATIO_4_3)
                .build()

        val imageAnalysis = setImageAnalysis()

        imageCapture = setImageCapture();

        val cameraSelector = CameraSelector.Builder()
                .requireLensFacing(CameraSelector.LENS_FACING_BACK)
                .build()

        cameraProvider.unbindAll()

        cameraProvider.bindToLifecycle(
                this,
                cameraSelector,
                imageAnalysis,
                imageCapture,
                preview
        )

        preview.setSurfaceProvider(previewView.createSurfaceProvider())
    }

    private  fun setImageAnalysis():ImageAnalysis {
        val imageAnalysis = ImageAnalysis.Builder()
            .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
            .build()

        imageAnalysis.setAnalyzer(
            executor,
            ObjectDetectorAnalyzer(applicationContext, objectDetectorConfig, ::onDetectionResult)
        )

        return imageAnalysis
    }

    private  fun setImageCapture():ImageCapture {
        val imageCapture = ImageCapture.Builder()
            .setFlashMode(ImageCapture.FLASH_MODE_AUTO)
            .setCaptureMode(ImageCapture.CAPTURE_MODE_MAXIMIZE_QUALITY)
            .build()
        captureButton.setOnClickListener {
            takePhoto()
        }
        return imageCapture
    }

    private fun takePhoto() {
        Log.e("TAG", "Captured");
        val imageCapture = imageCapture ?: return

        val photoFile = File(getOutputDirectory() , SimpleDateFormat(FILENAME_FORMAT, Locale.US).format(System.currentTimeMillis()) + ".jpg") // You can store the image in the cache for example using `cacheDir.absolutePath` as a path.

        val outputOptions = ImageCapture.OutputFileOptions
            .Builder(photoFile)
            .build()

        Log.e("TAG", "Captured");

        imageCapture.takePicture(
            outputOptions,
            ContextCompat.getMainExecutor(this),
            object : ImageCapture.OnImageSavedCallback {
                override fun onError(exc: ImageCaptureException) {
                    Log.e("TAG", "Photo capture failed: ${exc.message}", exc)
                }

                override fun onImageSaved(output: ImageCapture.OutputFileResults) {
                    val savedUri = Uri.fromFile(photoFile)
                    iv_capture.setImageURI(savedUri);
                    iv_capture.rotation = 90.0F
                    frameLayout1.visibility = View.INVISIBLE;
                    iv_capture.visibility = View.VISIBLE;
                    Log.e("TAG", "Photo capture success: $savedUri");
                }
            })
    }

    private fun getProcessCameraProvider(onDone: (ProcessCameraProvider) -> Unit) {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(this)
        cameraProviderFuture.addListener(
                Runnable { onDone.invoke(cameraProviderFuture.get()) },
                ContextCompat.getMainExecutor(this)
        )
    }

    private fun onDetectionResult(result: ObjectDetectorAnalyzer.Result) {
        result_overlay.updateResults(result)
    }

    private fun showSnackbar(message: String) {
        Snackbar.make(root_container, message, Snackbar.LENGTH_LONG).show()
    }

    private fun getOutputDirectory(): File {
        val mediaDir = externalMediaDirs.firstOrNull()?.let {
            File(it, resources.getString(R.string.app_name)).apply { mkdirs() }
        }
        return if (mediaDir != null && mediaDir.exists())
            mediaDir else filesDir
    }

    companion object {
        const val TAG = "CameraX"
        const val FILENAME_FORMAT = "yyyy-MM-dd-HH-mm-ss-SSS"
    }
}