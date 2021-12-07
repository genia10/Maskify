package ru.`object`.detection

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import org.tensorflow.lite.examples.detection.R
import android.view.View
import androidx.recyclerview.widget.GridLayoutManager

import androidx.recyclerview.widget.RecyclerView
import ru.`object`.detection.util.RecyclerViewAdapter
import android.widget.Toast
import android.widget.TextView

import android.app.ProgressDialog
import android.opengl.Visibility


class MaskChoiceActivity : AppCompatActivity() ,RecyclerViewAdapter.ItemClickListener {
    var adapter: RecyclerViewAdapter? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mask_choice)
        var availableMasks = getCurrentMasks()
        var absent = arrayOf("789", "7ff89")
        var data = arrayOf("123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89",
            "123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89",
            "123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89",
            "123", "456", "789", "12ds3", "4fds56", "7ff89", "12sdf3", "45hj6", "7k89", "1sdfsdf23", "45sdfsf6", "7sdf89")
        createGrid(data, absent)
        /*thread {
            val client = Socket("194.85.173.14", 7777)
            var abc = client.getInputStream().readBytes()
            File("/storage/emulated/0/Android/media/org.tensorflow.lite.examples.detection/ObjectDetectionDemo/1.jpg").writeBytes(abc)
            client.close()
        }
        var a = "1 2 dssdgsdgs"
        var list1 = a.split(" ");
        var list = listOf<String>("1", "2", "dssdgsdgs")*/
        //Log.e("123", list.toString().substring(1,list.toString().length - 1).replace(',', ' '));
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
        //(recyclerView.findViewHolderForAdapterPosition(position)?.itemView?.findViewById(R.id.info_text) as TextView).visibility = GONE
    }

    private fun getCurrentMasks(): Array<out String>? {
        return assets.list("masks")
    }
}