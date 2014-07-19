package nz.net.plan9.can_hunter;

import java.io.IOException;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;

import android.app.Activity;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

public class MainActivity extends Activity implements CvCameraViewListener2 {
    private static final String    TAG = "CanHunter::Activity";

    private Mat                    mRgba;
    private Mat                    mGray;
    
    private MediaPlayer mp;
    
    private int decimate;
    
    private int lastSize = 0;

    private CameraBridgeViewBase   mOpenCvCameraView;
    
    private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");

                    // Load native library after(!) OpenCV initialization
                    System.loadLibrary("can_hunter_jni");

                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public MainActivity() {
        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        
        setContentView(R.layout.tutorial2_surface_view);
     
        try {
        	mp = MediaPlayer.create(MainActivity.this, R.raw.jaws);
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SecurityException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
      
    }

    /*public void onDestroy() {
        if (mp.isPlaying()) {
        	mp.stop();
        }
        super.onDestroy();
    }*/
    
    @Override
    public void onPause()
    {
        super.onPause();
    }

    @Override
    public void onResume()
    {
        super.onResume();
    }

    @Override
    public void onStart()
    {
        super.onStart();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_8, this, mLoaderCallback);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.tutorial2_activity_surface_view);
        mOpenCvCameraView.setCvCameraViewListener(this);
    }

    @Override
    public void onStop()
    {
        if (mp.isPlaying()) {
        	mp.stop();
        }
        if (mOpenCvCameraView != null) {
            mOpenCvCameraView.disableView();
        }
        super.onStop();
    }
    
    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat(height, width, CvType.CV_8UC4);
        mGray = new Mat(height, width, CvType.CV_8UC4);
        ((CameraView) mOpenCvCameraView).SetFlash(true);
    }

    public void onCameraViewStopped() {
        mRgba.release();
        mGray.release();
        if (mOpenCvCameraView != null)
        	((CameraView) mOpenCvCameraView).SetFlash(false);
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
    	++decimate;
    	if (decimate % 2 == 0)
    	{
    		mRgba = inputFrame.rgba();
    		int size = FindCan(mRgba.getNativeObjAddr(), mGray.getNativeObjAddr());
    		if (size > 0 && lastSize <= 0 && !mp.isPlaying()) {
    			mp.start();
    		}
    		lastSize = size;
    	}
        return mGray;
    }

    public native int FindCan(long matAddrGr, long matAddrRgba);
}
