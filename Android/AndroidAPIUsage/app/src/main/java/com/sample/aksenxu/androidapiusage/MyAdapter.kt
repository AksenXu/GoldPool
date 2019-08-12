package com.sample.aksenxu.androidapiusage

import android.content.Context
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.LinearLayout
import android.widget.TextView

class MyAdapter() : BaseAdapter() {

    var mContext: Context? = null;
    private var mList: ArrayList<HashMap<String, Any>>? = null

    constructor(context: Context, list: ArrayList<HashMap<String, Any>>) : this() {
        mContext = context;
        mList = list
    }
    override fun getItem(position: Int): Any {
        return mList!!.get(position);
    }

    override fun getItemId(position: Int): Long {
        return position.toLong()
    }

    override fun getCount(): Int {
        return mList!!.count()
    }

    internal inner class ViewHolder {
        var textItem: TextView? = null
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val holder: ViewHolder
        var view: View
        if (convertView == null) {
            view = LinearLayout
                .inflate(mContext, R.layout.listview, null)
            holder = ViewHolder()
            holder.textItem = view
                .findViewById(R.id.testItem) as TextView
            view.tag = holder
        } else {
            holder = convertView.tag as ViewHolder
            view = convertView
        }
        holder.textItem!!.setText(mList!!.get(position).get("title") as String)
        return view
    }
}