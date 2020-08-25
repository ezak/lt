package com.iz.blackwater.lt.database;


import androidx.annotation.NonNull;
import androidx.room.Entity;
import androidx.room.Ignore;
import androidx.room.PrimaryKey;

@Entity(tableName = "torrent")
public class TorrentEntry {

	@NonNull
	@PrimaryKey(autoGenerate = false)
	private String id;
	private String name;
	private String state;
	private float progress;

	@Ignore
	public TorrentEntry(String name, String state, float progress) {
		this.name = name;
		this.state = state;
		this.progress = progress;
	}

	public TorrentEntry(String id, String name, String state, float progress) {
		this.id = id;
		this.name = name;
		this.state = state;
		this.progress = progress;
	}


	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getState() {
		return state;
	}

	public void setState(String state) {
		this.state = state;
	}

	public float getProgress() {
		return progress;
	}

	public void setProgress(float progress) {
		this.progress = progress;
	}
}
