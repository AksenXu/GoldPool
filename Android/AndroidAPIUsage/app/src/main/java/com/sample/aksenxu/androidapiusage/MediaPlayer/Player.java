package com.sample.aksenxu.androidapiusage.MediaPlayer;

import android.content.Context;
import android.widget.VideoView;

public class Player {

    private final Context mContext;
    private final VideoView mVideoView;
    private String mFileName;

    public Player(Context context, VideoView view) {
        mVideoView = view;
        mContext = context;
    }

    public void setDataSource(String file) {
        mFileName = file;
    }

    public void start() {
        mVideoView.setVideoPath(mFileName);
        mVideoView.start();
    }

    public void stop() {
        mVideoView.stopPlayback();
    }
}
