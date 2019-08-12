package com.sample.aksenxu.androidapiusage;

import android.app.Activity;
import android.graphics.*;
import android.os.Bundle;
import android.os.Handler;
import android.os.MemoryFile;
import android.os.RemoteException;
import android.util.Log;
import android.widget.ImageView;

import java.io.*;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class captureYUVActivity extends Activity {

    private Handler mHanlder;
    private String TAG = "captureYUVActivity";
    private int MEMORY_LENGTH = 1920*1080*2;
    private MemoryFile memoryFile = null;
    private FileDescriptor fileDescriptor;
    private Integer fd;
    private Thread mThread;
    private int width;
    private int height;
    private int size;
    ImageView mImageView;
    byte[] yuvdata = null;
    private int mExit = 0;

    public captureYUVActivity() {

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mHanlder = new Handler(this.getMainLooper());
        setContentView(R.layout.captureyuv);
        mImageView = findViewById(R.id.yuvView);
        startShareMemory();
    }

    @Override
    protected void onPause() {
        super.onPause();
        try {
            mExit = 1;
            if(mThread.isAlive()) mThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void startShareMemory(){
        Log.i(TAG, "开始创建yuv共享内存!!");
        try {
            memoryFile = new MemoryFile("yuv_memory", MEMORY_LENGTH);
        } catch (IOException e) {
            e.printStackTrace();
            Log.i(TAG,"创建yuv共享内存失败，message=" + e.getMessage());
            return;
        }

        Log.i(TAG, "创建共享内存成功:" + memoryFile);

        try {
            Method method = MemoryFile.class.getDeclaredMethod("getFileDescriptor");
            fileDescriptor = (FileDescriptor) method.invoke(memoryFile);
        } catch (Exception e) {
            Log.i(TAG,"获取yuv共享内存getFileDescriptor失败，message=" + e.getMessage());
            return;
        }

        Log.i(TAG, "getFileDescriptor : fileDescriptor = " + fileDescriptor);

        if(!setShareFD2("android.hardware.Camera", fileDescriptor)) {
            Log.i(TAG,"调用setShareFD2失败");
            return;
        }

        try {
            Method md = FileDescriptor.class.getDeclaredMethod("getInt$");
            fd = (Integer) md.invoke(fileDescriptor);
        }catch (Exception e){
            e.printStackTrace();
            Log.i(TAG,"获取yuv共享内存fd失败，message=" + e.getMessage());
            return;
        }

        mThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (mExit == 0) {
                    try {
                        receiveData();
                        Thread.sleep(300);
                    } catch (Exception e) {
                        Log.e(TAG, "receiveData exception: " + e);
                        e.printStackTrace();
                        break;
                    }
                }
                Log.i(TAG, "mThread = " + mThread + " exit!");
            }
        });
        mThread.start();
    }

    private void receiveData() throws IOException, RemoteException {
        int length = 16;
        byte[] buf = new byte[length];
        memoryFile.readBytes(buf,0, 0,16);

        if (buf[0] == 1) {
            int type = buf[1];
            int padding = buf[2];
            width = (int)buf[5] << 8 | ((int)buf[4] & 0xFF);
            height = (int)buf[7] << 8 | ((int)buf[6] & 0xFF);
            size = width * height * 3 / 2;
//            Log.i(TAG, "receiveData buf =" + buf[4] + " " + buf[5] + " " + buf[6] + " " + buf[7]);
            Log.i(TAG, "receiveData type = " + type + " padding = " + padding + " width = " + width + " height = " + height);
            //long timestamp = buf[4] << 8 | buf[3];
//            if(padding == 0)
//            {
//                padding = 1;
//            }
            if(yuvdata == null) {
                yuvdata = new byte[size];
            }

            Log.i(TAG, "memoryFile length = " + memoryFile.length() + " yuvdata length = " + yuvdata.length);
            memoryFile.readBytes(yuvdata,  16, 0, size);
            saveYUV2Bitmap(yuvdata);
            buf[0] = 0;
            memoryFile.getOutputStream().write(buf, 0, 1);
        }
    }

    private static void YV12toNV21(final byte[] input, final byte[] output, final int width, final int height) {
        long startMs = System.currentTimeMillis();
        final int frameSize = width * height;
        final int qFrameSize = frameSize / 4;
        final int tempFrameSize = frameSize * 5 / 4;

        System.arraycopy(input, 0, output, 0, frameSize); // Y

        for (int i = 0; i < qFrameSize; i++) {
            output[frameSize + i * 2] = input[frameSize + i]; // Cb (U)
            output[frameSize + i * 2 + 1] = input[tempFrameSize + i]; // Cr (V)
        }
    }

    public static String getTimeStamp() {
        SimpleDateFormat sdfFileName = new SimpleDateFormat("HHmmss_SSS", Locale.getDefault());
        // Create a media file name
        return sdfFileName.format(new Date());
    }

    private void saveYUV2Bitmap(byte[] yuvdata) {
        Log.d(TAG, "saveYUV2Bitmap");
        ByteArrayOutputStream stream = null;
        try {
            byte[] output = new byte[size];
            YV12toNV21(yuvdata, output, width, height);
            YuvImage yuvImage = new YuvImage(output, ImageFormat.NV21, width, height, null);
            //YuvImage yuvImage = new YuvImage(mBufferBean.mBuffer, ImageFormat.NV21, 1280, 720, null);
            Log.d(TAG, "readShareBufferMsg yuvImage:" + yuvImage);
            stream = new ByteArrayOutputStream();
            yuvImage.compressToJpeg(new Rect(0, 0, width, height), 100, stream);
            final Bitmap bmp = BitmapFactory.decodeByteArray(stream.toByteArray(), 0, stream.size());

            mHanlder.post(new Runnable() {
                @Override
                public void run() {
                    mImageView.setImageBitmap(bmp);
                }
            });

            File saveFile = new File(File.separator + "storage" + File.separator + "sdcard0/"
                    + "IMG_" + getTimeStamp() + ".jpg");

            Log.d(TAG, "saveFile:" + saveFile);
            try {
                FileOutputStream fileOutputStream = new FileOutputStream(saveFile);
                bmp.compress(Bitmap.CompressFormat.JPEG, 85, fileOutputStream);
                fileOutputStream.flush();
                fileOutputStream.close();

            } catch (Exception e) {
                Log.d(TAG, "error: " + e.getMessage());
                e.printStackTrace();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (stream != null) {
                try {
                    stream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * 设置属性
     * @param className 类的全名
     * @param fileDescriptor fd值
     */
    public boolean setShareFD2(String className, FileDescriptor fileDescriptor){
        if(className == null || className.equals("")){
            return false;
        }

        Log.i(TAG, "setShareFD2: className = " + className + " fileDescriptor = " + fileDescriptor);
        try {
            Class<?> c = Class.forName(className);
            Method setShareFD = null;
            Method[] ms = c.getDeclaredMethods();
            for (Method m : ms) {
                Log.i(TAG, "m.getName = " + m.getName());
                if (m.getName().equals("setShareFD2")) {
                    setShareFD = m;
                    Log.i(TAG, "Found setShareFD2 function");
                    break;
                }
            }

            invokeMethod( null, setShareFD, fileDescriptor);
        } catch (Exception e) {
            e.printStackTrace();
            Log.i(TAG, "设置共享内存句柄给系统失败（setShareFD），message=" + e.getMessage());
            return false;
        }

        return true;
    }

    public static final int CAMERA_SHARE_BUFFER_PREVIEW = 1 << 0;
    public static final int CAMERA_SHARE_BUFFER_RECORD =  1 << 1;
    public void invokeMethod(Object instance, Method m, FileDescriptor fileDescriptor){
        if(m == null){
            Log.i(TAG, "invokeMethod m is null");
            throw new IllegalArgumentException("method 不能为空");
        }
        m.setAccessible(true);
        try {
            Log.i(TAG, "invokeMethod m " + m + " captureYUVActivity.class = " + captureYUVActivity.class.toString() + " instance = " + instance);
            m.invoke(instance, fileDescriptor, captureYUVActivity.class.toString(), CAMERA_SHARE_BUFFER_RECORD);
        } catch (Exception e){
            e.printStackTrace();
        }
    }

}
