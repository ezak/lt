package com.iz.blackwater.lt;

import android.app.Application;
import android.util.Log;

import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;

import com.iz.blackwater.lt.database.AppDatabase;
import com.iz.blackwater.lt.database.TorrentEntry;

import java.util.List;

public class MainViewModel extends AndroidViewModel {

	// Constant for logging
	private static final String TAG = MainViewModel.class.getSimpleName();

	private LiveData<List<TorrentEntry>> torrents;

	public MainViewModel(Application application) {
		super(application);
		AppDatabase database = AppDatabase.getInstance(this.getApplication());
		Log.d(TAG, "Actively retrieving the tasks from the DataBase");
		torrents = database.torrentDao().loadAllTorrents();
	}

	public LiveData<List<TorrentEntry>> getTorrents() {
		return torrents;
	}
}
