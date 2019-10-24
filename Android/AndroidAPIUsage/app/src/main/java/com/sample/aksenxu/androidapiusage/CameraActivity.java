package com.sample.aksenxu.androidapiusage;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Toast;
import android.widget.VideoView;
import com.sample.aksenxu.androidapiusage.Camera.CameraManagerAPI;
import com.sample.aksenxu.androidapiusage.MediaPlayer.Player;
import com.sample.aksenxu.androidapiusage.MediaRecorder.Recorder;

import java.util.Arrays;

public class CameraActivity extends Activity implements View.OnClickListener, CameraManagerAPI.CameraManagerListener {
    private static final String TAG = "API_MR";

    private static final int PERMISSTION_RESULT = 100;
    private static final int RECORD_PERMISSION_RESULT = 101;
    private static final int CAMERA_PERMISSION_RESULT = 102;

    private SurfaceView mSurfaceView;
    private CameraManagerAPI mCameraManagerAPI;
    private String mCameraID;

    private String mFilename;
    private Recorder mRecorder;
    private Player mPlayer;
    private VideoView mVideoView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.camera_activity);

        initView();
        initMember();
        openCamera();
    }

    private void initMember() {
        //need not permisstion
        mCameraManagerAPI = new CameraManagerAPI(this);
        mCameraManagerAPI.setCameraManagerListener(this);
        String[] list = mCameraManagerAPI.getCameraList();
        Log.d(TAG, "initMember list: " + Arrays.toString(list));

        if (list.length != 0) {
            mCameraID = list[0];
        }
    }

    private void reStartActivity() {
        Intent intent = getIntent();
        finish();
        startActivity(intent);
    }

    private void requestPermission(String permission, int result) {
        requestPermissions(new String[]{permission}, result);
    }

    private void requestPermission() {
        String[] permissions = {
                Manifest.permission.RECORD_AUDIO,
                Manifest.permission.CAMERA,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
        };

        int permissionNum = 0;
        String[] permissionsReq = new String[permissions.length];

        for (String permission : permissions) {
            if (checkSelfPermission(permission) != PackageManager.PERMISSION_GRANTED) {
                permissionsReq[permissionNum++] = permission;
            }
        }

        if (permissionNum != 0) {
            requestPermissions(Arrays.copyOfRange(permissionsReq, 0, permissionNum), PERMISSTION_RESULT);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case CAMERA_PERMISSION_RESULT:
                closeCamrea();
                openCamera();
                break;
            case RECORD_PERMISSION_RESULT:
                startRecorder();
                break;
            case PERMISSTION_RESULT:
                break;
        }
    }

    private void initView() {
        findViewById(R.id.startRecord).setOnClickListener(this);
        findViewById(R.id.stopRecord).setOnClickListener(this);
        findViewById(R.id.startPlay).setOnClickListener(this);
        findViewById(R.id.stopPlay).setOnClickListener(this);

        mSurfaceView = findViewById(R.id.cameraView);
        mVideoView = findViewById(R.id.videoView);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        closeCamrea();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.startRecord:
                startRecorder();
                break;
            case R.id.stopRecord:
                stopRecorder();
                break;
            case R.id.startPlay:
                startPlay();
                break;
            case R.id.stopPlay:
                stopPlay();
                break;
            default:
                break;
        }
    }

    private void startRecorder() {
        if (checkSelfPermission(Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            requestPermission(Manifest.permission.RECORD_AUDIO, RECORD_PERMISSION_RESULT);
            return;
        }

        if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            requestPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, RECORD_PERMISSION_RESULT);
            return;
        }

        mRecorder = new Recorder(mCameraManagerAPI.getCamera(mCameraID));
        mRecorder.startRecorder();
        mFilename = mRecorder.getRecordFile();
    }

    private void stopRecorder() {
        if (mRecorder != null) {
            mRecorder.stopRecorder();
            mRecorder = null;
        }
    }

    private void startPlay() {
        if(mRecorder != null) {
            Toast toast = Toast.makeText(this, "stopRecord first", Toast.LENGTH_SHORT);
            toast.show();
            return;
        }
        if(mFilename == null) {
            Toast toast = Toast.makeText(this, "startRecord first", Toast.LENGTH_SHORT);
            toast.show();
            return;
        }
        closeCamrea();

        mSurfaceView.setVisibility(View.INVISIBLE);
        mVideoView.setVisibility(View.VISIBLE);

        mPlayer = new Player(this, mVideoView);
        mPlayer.setDataSource(mFilename);
        mPlayer.start();
    }

    private void stopPlay() {
        if(mPlayer != null) {
            mPlayer.stop();
            mPlayer = null;
            openCamera();
        }

        mSurfaceView.setVisibility(View.VISIBLE);
        mVideoView.setVisibility(View.INVISIBLE);
    }


    private void openCamera() {
        if (checkSelfPermission(Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            requestPermission(Manifest.permission.CAMERA, CAMERA_PERMISSION_RESULT);
            return;
        }

        mCameraManagerAPI.openCamera(mCameraID, mSurfaceView);
    }

    private void closeCamrea() {
        mCameraManagerAPI.closeCamera(mCameraID);
    }

    @Override
    public void onCameraManagerLIstener(String cameraid, int status) {
        Log.i(TAG, "onCameraManagerLIstener cameraid: " + cameraid + " status: " + status);
    }
}
