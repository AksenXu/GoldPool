package com.sample.aksenxu.androidapiusage.MediaRecorder;

import android.media.MediaCodec;
import android.media.MediaRecorder;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import com.sample.aksenxu.androidapiusage.Camera.BaseCamera;

import java.io.File;
import java.io.IOException;

public class Recorder implements MediaRecorder.OnInfoListener, MediaRecorder.OnErrorListener {
    private static final String TAG = "API_Recorder";
    private final String RECORD_FILE = Environment.getExternalStorageDirectory() + "/APITest/" + "record.mp4";
    private final BaseCamera mCamera;
    private int mWidth = 1920;
    private int mHeight = 1080;
    private int mBitrate = 10000000;
    private MediaRecorder mMediaRecorder;
    private Surface mRecordSurface;
    private int mFps = 30;

    public Recorder(BaseCamera camera) {
        mCamera = camera;
    }

    /**
     * start MediaRecorder inside, can be called after Recorder created
     *
     * @return 0 if successful, -1 if something wrong
     */
    public int startRecorder() {
        //set the record surface to Camera, it will 'repeating' fill the surface
        try {
            MediaRecorder recorder = new MediaRecorder();
            File path = new File(RECORD_FILE.substring(0, RECORD_FILE.lastIndexOf("/")));
            if (!path.exists()) {
                path.mkdir();
            }

            recorder.setOnInfoListener(this);
            recorder.setOnErrorListener(this);

            //MUST call in this sequence
            recorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            recorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);
            recorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);

            //video info
            recorder.setVideoEncoder(MediaRecorder.VideoEncoder.DEFAULT);
            recorder.setVideoSize(mWidth, mHeight);
            recorder.setVideoEncodingBitRate(mBitrate);
            recorder.setVideoFrameRate(mFps);

            //audio info
            recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);
            recorder.setAudioSamplingRate(16000);

            //file info
            recorder.setOutputFile(RECORD_FILE);
            Log.i(TAG, "start record file: " + RECORD_FILE);

            recorder.prepare();

            mRecordSurface = recorder.getSurface();
            mCamera.setRecordSurface(mRecordSurface);

            recorder.start();
            mMediaRecorder = recorder;
        } catch (IOException | RuntimeException e) {
            Log.w(TAG, "startrecorder failed:", e);
            return -1;
        }
        Log.i(TAG, "startRecord cameraid" + mCamera.getCameraID() + " success");
        return 0;
    }

    /**
     * stop MediaRecorder inside
     */
    public void stopRecorder() {
        if (mCamera != null) {
            mCamera.setRecordSurface(null);
        }

        if (mMediaRecorder != null) {
            mMediaRecorder.stop();
            mMediaRecorder.release();
            mMediaRecorder = null;
        }
        Log.i(TAG, "stopRecorder cameraid" + mCamera.getCameraID() + " success");
    }

    public String getRecordFile() {
        return RECORD_FILE;
    }

    @Override
    public void onInfo(MediaRecorder mr, int what, int extra) {
        Log.i(TAG, "onInfo what: " + what + " extra: " + extra);
    }

    @Override
    public void onError(MediaRecorder mr, int what, int extra) {
        Log.i(TAG, "onError what: " + what + " extra: " + extra);
    }
}
