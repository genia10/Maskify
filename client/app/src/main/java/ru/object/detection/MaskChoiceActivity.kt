package ru.`object`.detection

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import org.tensorflow.lite.examples.detection.R
import android.view.View
import androidx.recyclerview.widget.GridLayoutManager

import androidx.recyclerview.widget.RecyclerView
import ru.`object`.detection.util.RecyclerViewAdapter

class MaskChoiceActivity : AppCompatActivity() ,RecyclerViewAdapter.ItemClickListener {
    var adapter: RecyclerViewAdapter? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mask_choice)
        var availableMasks = getCurrentMasks()
        //costructTable()
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

        val recyclerView = findViewById<RecyclerView>(R.id.rvNumbers)
        val numberOfColumns = 3
        recyclerView.layoutManager = GridLayoutManager(this, numberOfColumns)
        try {
            adapter = RecyclerViewAdapter(this, availableMasks as Array<String>)
        }
        catch (e: Exception) {
            throw ClassCastException(e.message);
        }
        adapter!!.setClickListener(this)
        recyclerView.adapter = adapter
    }

    override fun onItemClick(view: View?, position: Int) {
        Log.i(
            "TAG",
            "You clicked number " + adapter!!.getItem(position) + ", which is at cell position " + position
        )
    }

    private fun getCurrentMasks(): Array<out String>? {
        return assets.list("masks")
    }
}