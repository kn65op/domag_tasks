package io.github.kn65op.domag_tasks.gui

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
import io.github.kn65op.domag_tasks.R
import io.github.kn65op.domag_tasks.storage.DataStorage
import io.github.kn65op.domag_tasks.tasks.*
import io.github.kn65op.domag_tasks.tasks.Task
import io.github.kn65op.domag_tasks.tasks.filterHasDeadline
import io.github.kn65op.domag_tasks.tasks.filterNoDeadline
import io.github.kn65op.domag_tasks.utils.platform.localization.AndroidLocalization
import java.time.ZonedDateTime

abstract class TasksAdapter(
    private val taskStorage: DataStorage,
    private val refreshViewFunction: () -> Unit,
    private val activity: FragmentActivity,
    private val context: Context
) : RecyclerView.Adapter<TasksAdapter.TaskViewHolder>() {
    companion object {
        const val TAG = "TaskAdapter"
    }

    abstract fun getTasks(): List<Task>

    class TaskViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
        val textViewField: TextView = view.findViewById(R.id.taskView)
        val nextDeadlineField: TextView = view.findViewById(R.id.task_next_deadline)
        val doneCheckBox: CheckBox = view.findViewById(R.id.task_view_done)

        fun setBackgroundColor(color: Int) {
            view.setBackgroundColor(color)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): TaskViewHolder {
        Log.d(TAG, "Create view holder")
        val newView =
            LayoutInflater.from(parent.context).inflate(R.layout.task_view, parent, false) as View
        return TaskViewHolder(newView)
    }

    override fun onBindViewHolder(holder: TaskViewHolder, position: Int) {
        Log.i(TAG, "Add item $position")
        val task = getTasks()[position]
        holder.setBackgroundColor(context.getColor(getColorForTask(task)))
        holder.textViewField.text = task.summary
        holder.nextDeadlineField.text = task.nextDeadlineText(AndroidLocalization(context))
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
            Log.i(TAG, "Edit $task")
            intent.putExtra(TaskEditActivity.taskTypeIntentName, task.type)
            intent.putExtra(TaskEditActivity.taskIntentName, task)
            startActivity(context, intent, null)
        }
    }

    private fun getColorForTask(task: Task): Int {
        val today = ZonedDateTime.now().toLocalDate()
        val taskDate = task.nextDeadline?.toLocalDate()
        return when {
            taskDate == null -> R.color.taskBackground_today
            task.done -> R.color.taskBackground_done
            today > taskDate -> R.color.taskBackground_past
            today == taskDate -> R.color.taskBackground_today
            else -> R.color.default_background
        }
    }

    override fun getItemCount(): Int = getTasks().size
}

class DeadlineTasksAdapter(
    private val taskStorage: DataStorage,
    refreshViewFunction: () -> Unit,
    activity: FragmentActivity,
    context: Context
) : TasksAdapter(taskStorage, refreshViewFunction, activity, context) {

    override fun getTasks() =
        taskStorage.loadTasks().filterHasDeadline()

}

class NoDeadlineTasksAdapter(
    private val taskStorage: DataStorage,
    refreshViewFunction: () -> Unit,
    activity: FragmentActivity,
    context: Context
) : TasksAdapter(taskStorage, refreshViewFunction, activity, context) {

    override fun getTasks() =
        taskStorage.loadTasks().filterNoDeadline()

}
