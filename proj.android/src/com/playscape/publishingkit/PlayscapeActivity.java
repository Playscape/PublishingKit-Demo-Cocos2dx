package com.playscape.publishingkit;

import org.cocos2dx.simplegame.SimpleGame;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import com.facebook.AppEventsLogger;

import com.playscape.lifecycle.ActivityLifeCycle;

/**
 * Playscape's default implementation to an activity
 * 
 * @author maximr
 *
 */
public class PlayscapeActivity extends SimpleGame {
	
	private ActivityLifeCycle mActivityLifeCycle;

	@Override
	public void onCreate(Bundle bundle) {
	
		super.onCreate(bundle);

		mActivityLifeCycle = Playscape.getActivityLifeCycle(this);
		mActivityLifeCycle.onCreate(bundle);
	}
	
	@Override
	public void onDestroy() {
	    super.onDestroy();
	    mActivityLifeCycle.onDestroy();
	}

    @Override
    public void onResume() {
        super.onResume();
        AppEventsLogger.activateApp(this);
        mActivityLifeCycle.onResume();
    }
    
    /**
     * Call from activity's onPause
     */
    @Override
    public void onPause() {
        super.onPause();
        AppEventsLogger.deactivateApp(this);
        mActivityLifeCycle.onPause();
    }
    
    
    @Override
	public void onStart() {
		super.onStart();
		mActivityLifeCycle.onStart();
	}
	
	@Override
	public void onStop() {
	    super.onStop();
	    mActivityLifeCycle.onStop();
	}
	
	@Override
	public void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		mActivityLifeCycle.onSaveInstanceState(outState);
	}
	
	@Override
	public void onBackPressed() {
	    mActivityLifeCycle.onBackPressed();
	}
	
	/**
	 * Call from activity's onActivityResult
	 * 
	 * @param requestCode
	 * @param resultCode
	 * @param data
	 */
	@Override
	public void onActivityResult(
	    	int requestCode,
			int resultCode,
			Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		mActivityLifeCycle.onActivityResult(requestCode, resultCode, data);
	}
	
	/**
	 * Call from activity's onNewIntent
	 * 
	 * @param intent
	 */
    @Override
	public void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		mActivityLifeCycle.onNewIntent(intent);
	}
}