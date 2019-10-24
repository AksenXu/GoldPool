package com.sample.aksenxu.androidapiusage

import android.content.Intent
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.View
import android.widget.AdapterView
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity(), AdapterView.OnItemClickListener {
    val APIListData = ArrayList<HashMap<String, Any>>()

    override fun onItemClick(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
        val map = APIList.adapter.getItem(position) as Map<*, *>
        val intent = map["intent"] as Intent
        startActivity(intent)
        finish()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        initListData();

        //APIList define in the activity_main xml
        APIList.adapter = MyAdapter(this, APIListData);
        APIList.onItemClickListener = this;
    }

    private fun initListData() {
        addItem(const.NAME_SHARE_FD, Intent(this, captureYUVActivity::class.java))
        addItem(const.MediaRecorder, Intent(this, CameraActivity::class.java))
        //TODO: add  more activity here
    }

    protected fun addItem(name: String, intent: Intent) {
        val temp = java.util.HashMap<String, Any>()
        temp.put("title", name)
        temp.put("intent", intent)

        APIListData.add(temp)
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
