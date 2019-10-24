package com.sample.aksenxu.androidapiusage.Camera;

import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CaptureRequest;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.Surface;

import java.util.ArrayList;
import java.util.List;

public class BaseCamera {
    private static final String TAG = "API_BaseCamera";

    private static final int START_PREVIEW = 1;
    private static final int STOP_PREVIEW = 2;
    private static final int PREVIEW_SURFACE_CHANGED = 3;

    private static final int RECORD_SURFACE_CHANGED = 10;
    private static final int TAKE_PICTURE = 20;

    private String mCameraID;
    private Surface mPreviewSurface;
    private Surface mRecordSurface;

    private CameraDevice mCameraDevice;
    private CameraCaptureSession mCaptureSession;
    private CaptureRequest.Builder mRequestBuilder;
    private CaptureRequest mRequest;
    private Handler mWorkHandler;
    private HandlerThread mWorkThread;

    private int mPreviewStatus;

    public BaseCamera(String cameraId) {
        mCameraID = cameraId;
        createWorkThread();
    }

    public String getCameraID() {
        return mCameraID;
    }

    public void setPreviewSurface(Surface surface) {
        synchronized (this) {
            mPreviewSurface = surface;
            mWorkHandler.sendEmptyMessage(PREVIEW_SURFACE_CHANGED);
        }
    }

    public void close() {
        if (mCameraDevice != null) {
            mCameraDevice.close();
            mCameraDevice = null;
        }
        mWorkThread.quitSafely();
        mWorkThread = null;
    }

    public void setCameraDevice(CameraDevice camera) {
        mCameraDevice = camera;
    }

    void startPreview() {
        synchronized (this) {
            mWorkHandler.sendEmptyMessage(START_PREVIEW);
        }
    }

    void stopPreview() {
        synchronized (this) {
            mWorkHandler.sendEmptyMessage(STOP_PREVIEW);
        }
    }

    void takePicture() {
        synchronized (this) {
            mWorkHandler.sendEmptyMessage(TAKE_PICTURE);
        }
    }

    public void setRecordSurface(Surface surface) {
        synchronized (this) {
            mRecordSurface = surface;
            mWorkHandler.sendEmptyMessage(RECORD_SURFACE_CHANGED);
        }
    }

    private void createWorkThread() {
        mWorkThread = new HandlerThread("CameraThread_" + mCameraID);
        mWorkThread.start();
        mWorkHandler = new CamWorkHandler(mWorkThread.getLooper());
    }

    private void startCameraSession() {
        try {
            stopCameraSession();

            List<Surface> surfaceList = new ArrayList<>();

            // We set up a CaptureRequest.Builder with the output Surface.
            if (mRecordSurface != null) {
                mRequestBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_RECORD);
            } else {
                mRequestBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
            }

            if (mPreviewSurface != null) {
                mRequestBuilder.addTarget(mPreviewSurface);
                surfaceList.add(mPreviewSurface);
            }

            if (mRecordSurface != null) {
                mRequestBuilder.addTarget(mRecordSurface);
                surfaceList.add(mRecordSurface);
            }

            if(surfaceList.size() == 0) {
                Log.w(TAG, "no available surface set, cancel camera session");
                return;
            }
            Log.i(TAG, "startCameraSession: " + surfaceList);

            // Here, we create a CameraCaptureSession for camera preview.
            mCameraDevice.createCaptureSession(surfaceList,
                    new CameraCaptureSession.StateCallback() {
                        @Override
                        public void onConfigured(@NonNull CameraCaptureSession cameraCaptureSession) {
                            // When the session is ready, we start displaying the preview.
                            Log.d(TAG, "camera capture session configured, set repeating request");
                            mCaptureSession = cameraCaptureSession;
                            try {
                                mRequest = mRequestBuilder.build();
                                mCaptureSession.setRepeatingRequest(mRequest, null, mWorkHandler);
                            } catch (CameraAccessException e) {
                                e.printStackTrace();
                            }
                        }

                        @Override
                        public void onConfigureFailed(@NonNull CameraCaptureSession cameraCaptureSession) {
                            Log.w(TAG, "camera capture session configure failed");
                            mCaptureSession = null;
                            mRequest = null;
                        }
                    }, null
            );

        } catch (CameraAccessException e) {
            Log.e(TAG, "create capture session failed");
            e.printStackTrace();
        }
    }

    private void stopCameraSession() {
        if(mCaptureSession != null) {
            try {
                Log.i(TAG, "stopCameraSession: " + mCaptureSession);
                mCaptureSession.stopRepeating();
                mCaptureSession =  null;
            } catch (CameraAccessException e) {
                e.printStackTrace();
            }
        }
    }

    private class CamWorkHandler extends Handler {
        CamWorkHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case START_PREVIEW:
                    startCameraSession();
                    mPreviewStatus = 1;
                    break;
                case STOP_PREVIEW:
                    stopCameraSession();
                    mPreviewStatus = 0;
                    break;
                case PREVIEW_SURFACE_CHANGED:
                    if(mPreviewStatus == 1) {
                        startCameraSession();
                    }
                    break;
                case RECORD_SURFACE_CHANGED:
                    startCameraSession();
                    break;
            }
            super.handleMessage(msg);
        }
    }
}
