package ru.`object`.detection

import android.app.Activity
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import org.tensorflow.lite.examples.detection.R
import android.view.View
import androidx.recyclerview.widget.GridLayoutManager

import androidx.recyclerview.widget.RecyclerView
import ru.`object`.detection.util.RecyclerViewAdapter
import android.widget.TextView

import android.view.WindowManager
import android.widget.ProgressBar
import android.content.Intent
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.awaitAll
import kotlinx.coroutines.launch
import ru.`object`.detection.util.Mask
import ru.`object`.detection.util.ServerCommunication
import java.io.File
import java.net.Socket
import java.util.concurrent.Callable
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors
import kotlin.concurrent.thread


class MaskChoiceActivity : AppCompatActivity(), RecyclerViewAdapter.ItemClickListener {
    var adapter: RecyclerViewAdapter? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mask_choice)
        var server = ServerCommunication()
        var availableMasks = Mask.getCurrentMasks()
        var absent = arrayOf("789", "7ff89")
        var thread = Thread {
            val client = server.connect()
            server.sendCurrentMasks(availableMasks, client)
            //absent = server.getMissingList(client)
            server.disconnect(client)
        }
        thread.start()
        thread.join()
        var availableMaskList = availableMasks.toMutableList()
        //var absent = arrayOf("789", "7ff89")
        availableMaskList.addAll(absent)
        availableMasks = availableMaskList.toTypedArray()
        createGrid(availableMasks, absent)
        /*var data = arrayOf("123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89",
            "123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89",
            "123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89",
            "123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89")*/
    }

    private fun createGrid(data : Array<String>, absent : Array<String>) {
        val recyclerView = findViewById<RecyclerView>(R.id.rvNumbers)
        val numberOfColumns = 3
        recyclerView.layoutManager = GridLayoutManager(this, numberOfColumns)
        try
        {
            adapter = RecyclerViewAdapter(this, data, absent)
        }
        catch (e: Exception)
        {
            throw ClassCastException(e.message);
        }
        adapter!!.setClickListener(this)
        recyclerView.adapter = adapter
    }

    override fun onItemClick(view: View?, position: Int) {
        val recyclerView = findViewById<RecyclerView>(R.id.rvNumbers)
        val alpha = (recyclerView.findViewHolderForAdapterPosition(position)?.itemView?.findViewById(R.id.info_text) as TextView).alpha
        Log.e(
            "123",
            "You clicked  " + adapter!!.getItem(position) + ", which is at cell position " + position + ", with alpha = " + alpha.toString()
        )
        if(alpha != 1.0f) {
            (recyclerView.findViewHolderForAdapterPosition(position)?.itemView?.findViewById(R.id.info_text) as TextView).visibility = View.GONE
            (recyclerView.findViewHolderForAdapterPosition(position)?.itemView?.findViewById(R.id.progress_bar) as ProgressBar).visibility = View.VISIBLE
            getWindow().setFlags(
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE)
            android.os.Handler().postDelayed(
                {
                    loadMask(view, position)
                    (recyclerView.findViewHolderForAdapterPosition(position)?.itemView?.findViewById(R.id.info_text) as TextView).visibility = View.VISIBLE
                    (recyclerView.findViewHolderForAdapterPosition(position)?.itemView?.findViewById(R.id.progress_bar) as ProgressBar).visibility = View.GONE
                    (recyclerView.findViewHolderForAdapterPosition(position)?.itemView?.findViewById(R.id.info_text) as TextView).alpha = 1.0f
                    getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
                    val intent = Intent()
                    intent.putExtra("chosenMask", adapter!!.getItem(position))
                    setResult(Activity.RESULT_OK, intent)
                    finish()
                }, 2000)

        }
        else {
            val intent = Intent()
            intent.putExtra("chosenMask", adapter!!.getItem(position))
            setResult(Activity.RESULT_OK, intent)
            finish()
        }
    }

    private fun loadMask(view: View?, position: Int) {
        var server = ServerCommunication()
        var chosenMask = adapter!!.getItem(position)
        //Загрузка маски с сервера
        var thread = Thread {
            val client = server.connect()
            server.sendMaskDownloadRequest(chosenMask, client)
            server.getMask(chosenMask, client)
            server.disconnect(client)
        }
        thread.start()
        thread.join()
    }
}