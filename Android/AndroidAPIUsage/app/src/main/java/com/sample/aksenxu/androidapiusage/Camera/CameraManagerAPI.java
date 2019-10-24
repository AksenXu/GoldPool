package com.sample.aksenxu.androidapiusage.Camera;

import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.SurfaceView;

import java.util.ArrayList;
import java.util.List;

public class CameraManagerAPI {
    private static final String TAG = "API_Camera";

    private static final int CAMERA_OPENED = 0;
    private static final int CAMERA_CLOSED = 1;
    private static final int CAMERA_ERROR = 2;

    private final Context mContext;

    private android.hardware.camera2.CameraManager mCameraManager;
    private CameraStateCallback mStateCallback = new CameraStateCallback();
    private String[] mCameraIDs;
    private List<BaseCamera> mCameraList = new ArrayList<>();

    private CameraManagerListener mCameraManagerListener;
    public interface CameraManagerListener {
        void onCameraManagerLIstener(String cameraid, int status);
    }

    public CameraManagerAPI(Context context) {
        mContext = context;
    }

    public void setCameraManagerListener(CameraManagerListener listener) {
        mCameraManagerListener = listener;
    }

    /**
     * call CameraManager inside get cameralist, you can choose valid camera id to open later
     *
     * @return String[] cameralist if successful, null if something wrong
     */
    public String[] getCameraList() {
        mCameraManager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
        try {
            mCameraIDs = mCameraManager.getCameraIdList();
            for (String aMCameraList : mCameraIDs) {
                CameraCharacteristics characteristics = mCameraManager.getCameraCharacteristics(aMCameraList);
                Log.d(TAG, "camera " + aMCameraList + " chracteristics: " +  characteristics);
            }
        } catch (CameraAccessException e) {
            Log.e(TAG, "CameraManagerAPI exception:", e);
        }
        return mCameraIDs;
    }

    @SuppressLint("MissingPermission")
    public int openCamera(String cameraId, SurfaceView previewSurface) {
        try {
            mCameraManager.openCamera(cameraId, mStateCallback, null);
            addCamera(cameraId, previewSurface);
            Log.i(TAG, "openCamera camearid:" + cameraId + " previewSurface:" + previewSurface);
        } catch (CameraAccessException e) {
            Log.e(TAG, "openCamera " + cameraId + "failed: ", e);
            return -1;
        }
        return 0;
    }

    private void addCamera(String cameraId, SurfaceView surface) {
        synchronized (this) {
            BaseCamera info = new BaseCamera(cameraId);
            info.setPreviewSurface(surface.getHolder().getSurface());
            mCameraList.add(info);
        }
    }

    public BaseCamera getCamera(String cameraId) {
        synchronized (this) {
            for (BaseCamera info : mCameraList) {
                if (info.getCameraID().equals(cameraId)) {
                    return info;
                }
            }
        }
        return null;
    }

    public void closeCamera(String cameraId) {
        synchronized (this) {
            BaseCamera info = getCamera(cameraId);
            if (info != null) {
                info.close();
                mCameraList.remove(info);
            } else {
                Log.w(TAG, "closeCamera " + cameraId + " failed");
            }
        }
    }

    private class CameraStateCallback extends CameraDevice.StateCallback {
        @Override
        public void onOpened(@NonNull CameraDevice camera) {
            Log.i(TAG, "onOpened cameraId:" + camera.getId());

            BaseCamera info = getCamera(camera.getId());
            assert info != null;
            info.setCameraDevice(camera);

            if (mCameraManagerListener != null) {
                mCameraManagerListener.onCameraManagerLIstener(camera.getId(), CAMERA_OPENED);
            }
            info.startPreview();
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice camera) {
            Log.i(TAG, "onDisconnected cameraId:" + camera.getId());
            closeCamera(camera.getId());
            if (mCameraManagerListener != null) {
                mCameraManagerListener.onCameraManagerLIstener(camera.getId(), CAMERA_CLOSED);
            }

        }

        @Override
        public void onError(@NonNull CameraDevice camera, int error) {
            Log.i(TAG, "onError cameraId:" + camera.getId() + " error: " + error);
            closeCamera(camera.getId());
            if (mCameraManagerListener != null) {
                mCameraManagerListener.onCameraManagerLIstener(camera.getId(), CAMERA_ERROR);
            }
        }
    }
}
