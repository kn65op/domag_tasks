package com.example.domag.gui

import android.content.Context
import android.content.Intent
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.CheckBox
import android.widget.TextView
import androidx.core.content.ContextCompat.startActivity
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.RecyclerView
import com.example.domag.R
import com.example.domag.storage.DataStorage
import java.time.format.DateTimeFormatter

class TasksAdapter(
    private val taskStorage: DataStorage,
    private val refreshViewFunction: () -> Unit,
    private val activity: FragmentActivity,
    private val context: Context,
    private val timeFormatter: DateTimeFormatter = DateTimeFormatter.ofPattern("ccc dd-MMMM-yyyy")
) : RecyclerView.Adapter<TasksAdapter.TaskViewHolder>() {
    companion object {
        const val TAG = "TaskAdapter"
    }

    class TaskViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
        val textViewField: TextView = view.findViewById(R.id.taskView)
        val nextDeadlineField: TextView = view.findViewById(R.id.task_next_deadline)
        val doneCheckBox: CheckBox = view.findViewById(R.id.task_view_done)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): TaskViewHolder {
        Log.d(TAG, "Create view holder")
        val newView = LayoutInflater.from(parent.context).inflate(R.layout.task_view, parent, false) as View
        return TaskViewHolder(newView)
    }

    override fun onBindViewHolder(holder: TaskViewHolder, position: Int) {
        Log.i(TAG, "Add item $position")
        val task = taskStorage.loadTasks().tasks[position]
        holder.textViewField.text = task.summary
        holder.nextDeadlineField.text = task.nextDeadline.format(timeFormatter)
        holder.doneCheckBox.isChecked = task.done
        holder.doneCheckBox.setOnClickListener { view ->
            if (view is CheckBox) {
                task.done = view.isChecked
                taskStorage.store(task)
                refreshViewFunction()
            } else {
                Log.e(TAG, "OnClickListener for checkbox got other View then Checkbox")
            }
        }
        holder.view.setOnClickListener {
            val intent = Intent(activity, TaskEditActivity::class.java)
            intent.putExtra(task.type, task)
            startActivity(context, intent, null)
        }
    }

    override fun getItemCount(): Int = taskStorage.loadTasks().tasks.size
}
