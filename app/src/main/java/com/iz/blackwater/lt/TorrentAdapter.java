package com.iz.blackwater.lt;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.iz.blackwater.lt.database.TorrentEntry;

import java.text.SimpleDateFormat;
import java.util.List;
import java.util.Locale;

public class TorrentAdapter extends RecyclerView.Adapter<TorrentAdapter.TorrentViewHolder> {

	private static final String DATE_FORMAT = "dd/MM/yyy";

	final private ItemClickListener itemClickListener;

	private List<TorrentEntry> torrentEntries;
	private Context context;
	// Date formatter
	private SimpleDateFormat dateFormat = new SimpleDateFormat(DATE_FORMAT, Locale.getDefault());

	TorrentAdapter(Context context, ItemClickListener itemClickListener){
		this.context = context;
		this.itemClickListener = itemClickListener;
	}

	@NonNull
	@Override
	public TorrentViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
		View view = LayoutInflater.from(this.context)
				.inflate(R.layout.my_text_view, parent, false);
		return new TorrentViewHolder(view);
	}

	@Override
	public void onBindViewHolder(@NonNull TorrentViewHolder holder, int position) {
		TorrentEntry torrentEntry = this.torrentEntries.get(position);
		String name = torrentEntry.getName();
		String state = torrentEntry.getState();
		float progress = torrentEntry.getProgress();

		holder.name.setText(name);
		holder.state.setText(state);
		holder.progress.setText(String.valueOf(progress));
	}

	@Override
	public int getItemCount() {
		if (this.torrentEntries == null) {
			return 0;
		}
		return this.torrentEntries.size();

	}

	public List<TorrentEntry> getTorrents() {
		return this.torrentEntries;
	}

	public void setTorrents(List<TorrentEntry > torrentEntries) {
		this.torrentEntries = torrentEntries;
		notifyDataSetChanged();
	}

	public interface ItemClickListener {
		void onItemClickListener(String itemId);
	}

	class TorrentViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
		TextView name, state, progress;


		public TorrentViewHolder(@NonNull View itemView) {
			super(itemView);
			name = itemView.findViewById(R.id.txt_id_name);
			state = itemView.findViewById(R.id.txt_id_state);
			progress = itemView.findViewById(R.id.txt_id_progress);

		}

		@Override
		public void onClick(View v) {
			String elementId = torrentEntries.get(getAdapterPosition()).getId();
			itemClickListener.onItemClickListener(elementId);
		}
	}

}
