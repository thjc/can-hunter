package nz.net.plan9.can_hunter;

import org.opencv.android.JavaCameraView;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.Size;
import android.util.AttributeSet;
import android.util.Log;

public class CameraView extends JavaCameraView implements PictureCallback {

    private static final String TAG = "Sample::Tutorial3View";

    public CameraView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }
    public void SetFlash(boolean state)
    {
        Parameters p = mCamera.getParameters();

        if (state) {
        	p.setFlashMode(Parameters.FLASH_MODE_TORCH);
        } else {
        	p.setFlashMode(Parameters.FLASH_MODE_OFF);
        }

        p.setWhiteBalance(Parameters.WHITE_BALANCE_FLUORESCENT);
        p.setFocusMode(Parameters.FOCUS_MODE_INFINITY);
        p.setSceneMode(Parameters.SCENE_MODE_SPORTS);
                
        mCamera.setParameters(p);

    }
	@Override
	public void onPictureTaken(byte[] data, Camera camera) {
		// TODO Auto-generated method stub
		
	}
}

