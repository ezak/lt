package com.iz.blackwater.lt;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.IBinder;

import androidx.core.app.NotificationCompat;

public class LibtorrentService extends Service {

	static {
		System.loadLibrary("client_test");
	}

	public static final String CHANNEL_ID = "lt";

	private static LibtorrentService instance = null;

	private static long session;

	public static boolean isRunning() {
		return instance != null;
	}

	@Override
	public IBinder onBind(Intent intent) {
		// TODO: Return the communication channel to the service.
		throw new UnsupportedOperationException("Not yet implemented");
	}

	@Override
	public void onCreate() {
		instance = this;
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		String input = intent.getStringExtra("inputExtra");
		createNotificationChannel();
		Intent notificationIntent = new Intent(this, MainActivity.class);
		PendingIntent pendingIntent = PendingIntent.getActivity(this,
				0, notificationIntent, 0);
		Notification notification = new NotificationCompat.Builder(this, CHANNEL_ID)
				.setContentTitle("Foreground Service")
				.setContentText(input)
				.setSmallIcon(R.drawable.ic_launcher_foreground)
				.setContentIntent(pendingIntent)
				.build();
		startForeground(1, notification);

		Thread thread = new Thread(new Runnable() {
			@Override
			public void run() {
				session = startLibtorrent();
			}
		});
		thread.start();

		return START_STICKY;
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Thread thread = new Thread(new Runnable() {
			@Override
			public void run() {
				stopLibtorrent(session);
			}
		});

		thread.start();

		instance = null;
	}


	private void createNotificationChannel() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
			NotificationChannel serviceChannel = new NotificationChannel(
					CHANNEL_ID,
					"Foreground Service Channel",
					NotificationManager.IMPORTANCE_DEFAULT
			);
			NotificationManager manager = getSystemService(NotificationManager.class);
			assert manager != null;
			manager.createNotificationChannel(serviceChannel);
		}
	}


	private static native long startLibtorrent();

	private static native void stopLibtorrent(long ses);

	private static native void addMagnet(long jsession, String uri);

	public static void addMagnet(String uri){
		addMagnet(session, uri);
	}

}
