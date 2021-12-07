package ru.`object`.detection.util

import android.content.Context
import android.util.DisplayMetrics

import android.widget.TextView

import androidx.recyclerview.widget.RecyclerView

import androidx.annotation.NonNull

import android.view.ViewGroup

import android.view.LayoutInflater
import android.view.View
import org.tensorflow.lite.examples.detection.R
import ru.`object`.detection.MaskChoiceActivity




class RecyclerViewAdapter internal constructor(context: Context?, data: Array<String>, absent: Array<String> ) :
    RecyclerView.Adapter<RecyclerViewAdapter.ViewHolder>() {
    private val mData: Array<String>
    private val mAbsent: Array<String>
    private val mInflater: LayoutInflater
    private var mClickListener: ItemClickListener? = null

    // inflates the cell layout from xml when needed
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view: View = mInflater.inflate(R.layout.recyclerview_item, parent, false)
        val layoutParams: ViewGroup.LayoutParams = view.layoutParams
        layoutParams.width = (parent.width * 0.33).toInt()
        layoutParams.height = layoutParams.width
        view.layoutParams = layoutParams
        return ViewHolder(view)
    }

    // binds the data to the TextView in each cell
    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.myTextView.text = mData[position]
        if (mAbsent.contains(mData[position]))
            holder.myTextView.alpha = 0.3f
    }

    // total number of cells
    override fun getItemCount(): Int {
        return mData.size
    }

    // stores and recycles views as they are scrolled off screen
    inner class ViewHolder internal constructor(itemView: View) : RecyclerView.ViewHolder(itemView),
        View.OnClickListener {
        var myTextView: TextView
        override fun onClick(view: View?) {
            if (mClickListener != null) mClickListener!!.onItemClick(view, adapterPosition)
        }

        init {
            myTextView = itemView.findViewById(R.id.info_text)
            itemView.setOnClickListener(this)
        }
    }

    // convenience method for getting data at click position
    fun getItem(id: Int): String {
        return mData[id]
    }

    // allows clicks events to be caught
    fun setClickListener(itemClickListener: MaskChoiceActivity) {
        mClickListener = itemClickListener
    }

    // parent activity will implement this method to respond to click events
    interface ItemClickListener {
        fun onItemClick(view: View?, position: Int)
    }

    // data is passed into the constructor
    init {
        mInflater = LayoutInflater.from(context)
        mData = data
        mAbsent = absent
    }
}