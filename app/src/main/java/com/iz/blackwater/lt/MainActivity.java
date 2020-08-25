package com.iz.blackwater.lt;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.iz.blackwater.lt.database.AppDatabase;
import com.iz.blackwater.lt.database.TorrentEntry;

import java.io.File;
import java.util.List;

public class MainActivity extends AppCompatActivity implements TorrentAdapter.ItemClickListener{
	private static final int STORAGE_PERMISSION_CODE = 101;
	private static final int PICKFILE_RESULT_CODE = 8777;

	private RecyclerView recyclerView;
	private RecyclerView.Adapter mAdapter;
	private RecyclerView.LayoutManager layoutManager;


	private TorrentAdapter torrentAdapter;
	private AppDatabase appDatabase;


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// check permission
		checkPermission( Manifest.permission.WRITE_EXTERNAL_STORAGE, STORAGE_PERMISSION_CODE);

		recyclerView = findViewById(R.id.my_recycler_view);
		recyclerView.setLayoutManager(new LinearLayoutManager(this));

		torrentAdapter = new TorrentAdapter(this, this);
		recyclerView.setAdapter(torrentAdapter);

		DividerItemDecoration decoration = new DividerItemDecoration(getApplicationContext(), DividerItemDecoration.VERTICAL);
		recyclerView.addItemDecoration(decoration);

		appDatabase = AppDatabase.getInstance(getApplicationContext());
		setupViewModel();


	}

	private void setupViewModel(){
		MainViewModel mainViewModel = ViewModelProviders.of(this).get(MainViewModel.class);

		mainViewModel.getTorrents().observe(this, new Observer<List<TorrentEntry>>() {
			@Override
			public void onChanged(List<TorrentEntry> torrentEntries) {
				Log.d("TAG", "Updating list of tasks from LiveData in ViewModel");
				torrentAdapter.setTorrents(torrentEntries);
			}
		});
	}

	@Override
	public void onItemClickListener(String itemId) {
		// TODO implement body
	}

	@Override
	public void onRequestPermissionsResult(int requestCode,
	                                       @NonNull String[] permissions,
	                                       @NonNull int[] grantResults) {

		super.onRequestPermissionsResult(requestCode,
						permissions,
						grantResults);


		if (requestCode == STORAGE_PERMISSION_CODE) {
			if (grantResults.length > 0
					&& grantResults[0] == PackageManager.PERMISSION_GRANTED) {
				Toast.makeText(MainActivity.this,
						"Storage Permission Granted",
						Toast.LENGTH_SHORT)
						.show();
			}
			else {
				Toast.makeText(MainActivity.this,
						"Storage Permission Denied",
						Toast.LENGTH_SHORT)
						.show();
			}
		}
	}

	public void checkPermission(String permission, int requestCode) {
		if (ContextCompat.checkSelfPermission(MainActivity.this, permission)
				== PackageManager.PERMISSION_DENIED) {

			// Requesting the permission
			ActivityCompat.requestPermissions(MainActivity.this,
					new String[] { permission },
					requestCode);
		}
		else {
			Toast.makeText(MainActivity.this,
					"Permission already granted",
					Toast.LENGTH_SHORT)
					.show();
		}
	}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater menuInflater = getMenuInflater();
		menuInflater.inflate(R.menu.main_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(@NonNull MenuItem item) {
		switch (item.getItemId()) {
			case R.id.menu_id_add_torrent:
				addTorrent();
				return true;

			case R.id.menu_id_start_lt:
				Intent intent = new Intent(getApplicationContext(), LibtorrentService.class);
				if (!LibtorrentService.isRunning()) {
					startService(intent);
				} else {
					stopService(intent);
				}
				return true;

			default:
				return super.onOptionsItemSelected(item);
		}
	}


	private void addTorrent(){
		Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
		intent.addCategory(Intent.CATEGORY_OPENABLE);
		intent.setType("application/x-bittorrent");

		// Optionally, specify a URI for the file that should appear in the
		// system file picker when it loads.
		//intent.putExtra(DocumentsContract.EXTRA_INITIAL_URI, pickerInitialUri);

		startActivityForResult(intent, PICKFILE_RESULT_CODE);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		if (requestCode == PICKFILE_RESULT_CODE && resultCode == Activity.RESULT_OK) {
			Uri uri = null;

			if (data != null) {
				uri = data.getData();
				// Perform operations on the document using its URI.


				final String id = DocumentsContract.getDocumentId(uri);
				final Uri contentUri = ContentUris.withAppendedId(
						Uri.parse("content://downloads/public_downloads"), Long.parseLong(id));

				String[] projection = { MediaStore.Downloads.DATA };
				@SuppressLint("Recycle")
				Cursor cursor = getContentResolver().query(contentUri, projection, null, null, null);
				assert cursor != null;
				int column_index = cursor.getColumnIndexOrThrow(MediaStore.Downloads.DATA);
				cursor.moveToFirst();


				LibtorrentService.addMagnet(cursor.getString(column_index));
			}
		}
	}


	private void listDirectory() {
		PackageManager packageManager = getPackageManager();
		String packageName = getPackageName();

		try {
			PackageInfo packageInfo = packageManager.getPackageInfo(packageName, 0);
			packageName = packageInfo.applicationInfo.dataDir;
		} catch (PackageManager.NameNotFoundException e) {
			e.printStackTrace();
		}

		Log.d("TAG", packageName);

		File f = new File(packageName);
		File[] files = f.listFiles();
		assert files != null;
		for(File file : files)
		{
			Log.d("List directory", file.getAbsolutePath());
		}
	}
}
