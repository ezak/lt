package com.iz.blackwater.lt.database;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;
import androidx.room.Update;

import java.util.List;

@Dao
public interface TorrentDao {

	@Query("SELECT * FROM torrent ORDER BY state")
	LiveData<List<TorrentEntry>> loadAllTorrents();

	@Insert(onConflict = OnConflictStrategy.REPLACE)
	void insertTorrent(TorrentEntry torrentEntry);

	@Update(onConflict = OnConflictStrategy.REPLACE)
	void updateTorrent(TorrentEntry torrentEntry);

	@Delete
	void deleteTorrent(TorrentEntry torrentEntry);

	@Query("SELECT * FROM torrent WHERE id = :id")
	LiveData<TorrentEntry> loadTorrentById(String id);
}
