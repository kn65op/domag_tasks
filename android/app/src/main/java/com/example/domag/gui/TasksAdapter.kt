package com.example.domag.gui

import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.example.domag.R
import com.example.domag.storage.DataStorage
import java.time.format.DateTimeFormatter

class TasksAdapter(
    private val taskStorage: DataStorage,
    val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy")
) : RecyclerView.Adapter<TasksAdapter.TaskViewHolder>() {
    companion object {
        const val TAG = "TaskAdapter"
    }

    class TaskViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val textViewField: TextView = view.findViewById(R.id.taskView)
        val nextDeadlineField: TextView = view.findViewById(R.id.task_next_deadline)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): TaskViewHolder {
        Log.i(TAG, "Create view holder")
        val newView = LayoutInflater.from(parent.context).inflate(R.layout.task_view, parent, false) as View
        return TaskViewHolder(newView)
    }

    override fun onBindViewHolder(holder: TaskViewHolder, position: Int) {
        Log.i(TAG, "Add item $position")
        val task = taskStorage.loadTasks()[position]
        holder.textViewField.text = task.name
        holder.nextDeadlineField.text = task.nextDeadline.format(timeFormatter)
    }

    override fun getItemCount(): Int = taskStorage.loadTasks().size
}
