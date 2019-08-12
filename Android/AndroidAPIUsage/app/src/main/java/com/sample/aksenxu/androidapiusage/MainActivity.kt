package com.sample.aksenxu.androidapiusage

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.AdapterView
import kotlinx.android.synthetic.main.activity_main.*
import android.content.Intent
import kotlin.collections.HashMap;

class MainActivity : AppCompatActivity(), AdapterView.OnItemClickListener {
    override fun onItemClick(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
        val map = sample_text.adapter.getItem(position) as Map<*, *>
        val intent = map["intent"] as Intent
        startActivity(intent)
        finish()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.adapter = MyAdapter(this, getList());
        sample_text.onItemClickListener = this;

//        var mList: List<HashMap<String, String>>? = null
    }

    protected fun addItem(
        data: ArrayList<HashMap<String, Any>>, key: String, name: String,
        intent: Intent
    ) {
        val temp = java.util.HashMap<String, Any>()
        temp.put("title", name)
        temp.put("intent", intent)
        temp.put("result", key)

        data.add(temp)
    }

    protected fun activityIntent(cla: Class<captureYUVActivity>): Intent {
        val result = Intent()
        result.setClass(this, cla)
        return result
    }

    private fun getList(): ArrayList<HashMap<String, Any>> {
        val myData = ArrayList<HashMap<String, Any>>()
        addItem(myData, const.RESULT_SHARE_FD, const.NAME_SHARE_FD, activityIntent(captureYUVActivity::class.java))
        return myData
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
