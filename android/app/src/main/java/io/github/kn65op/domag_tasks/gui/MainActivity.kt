package io.github.kn65op.domag_tasks.gui

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import io.github.kn65op.domag_tasks.R
import io.github.kn65op.domag_tasks.notifications.Alarm
import io.github.kn65op.domag_tasks.notifications.Notifications
import io.github.kn65op.domag_tasks.notifications.TodayAndPastTasksNotification
import io.github.kn65op.domag_tasks.storage.DataStorage
import io.github.kn65op.domag_tasks.storage.DataStorageFactory
import io.github.kn65op.domag_tasks.tasks.NoDeadlineTask
import com.google.android.material.tabs.TabLayout
import io.github.kn65op.domag_tasks.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    companion object {
        const val TAG = "DomagMainActivity"
        const val deadline_tasks = 0
        const val no_deadline_tasks = 1
        const val tab_bundle_tag = "TAB_SELECTED"
    }

    private var tabSelected: TabLayout.Tab? = null
    private lateinit var storage: DataStorage
    private lateinit var viewManager: RecyclerView.LayoutManager
    private lateinit var binding: ActivityMainBinding


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        Log.i(TAG, "Create")
        setContentView(binding.root)
        setSupportActionBar(binding.toolbar)

        Alarm().start(applicationContext)
        Notifications().registerChannels(applicationContext)

        viewManager = LinearLayoutManager(this)
        storage = DataStorageFactory().createDriveDataStorageFactory(applicationContext)

        if (savedInstanceState == null) {
            Log.i(TAG, "No state to restore")
        }

        val lastSelectedTabPosition = savedInstanceState?.getInt(tab_bundle_tag)
        if (lastSelectedTabPosition != null) {
            Log.i(TAG, "Selected tab is $lastSelectedTabPosition")
            tabSelected = binding.tasksTab.getTabAt(lastSelectedTabPosition)
        }

        binding.tasksTab.addOnTabSelectedListener(
            object : TabLayout.OnTabSelectedListener {
                override fun onTabSelected(tab: TabLayout.Tab?) {
                    tabSelected = tab
                    prepareTaskView(tab?.position)
                }

                override fun onTabReselected(tab: TabLayout.Tab?) {
                    prepareTaskView(tab?.position)
                }

                override fun onTabUnselected(tab: TabLayout.Tab?) {
                    Log.d(TAG, "Unselected ${tab?.position}")
                }
            }
        )

        Log.i(TAG, "Set litener for: ${binding.addNewTaskButton}");
        binding.addNewTaskButton.setOnClickListener {
            Log.d(TAG, "listener called")
            val intent = Intent(this, TaskEditActivity::class.java)
            if (binding.tasksTab.selectedTabPosition == no_deadline_tasks)
                intent.putExtra(TaskEditActivity.taskTypeIntentName, NoDeadlineTask.type)
            startActivity(intent)
        }
    }

    override fun onResume() {
        super.onResume()
        Log.i(TAG, "Resume")
        loadTabSelected()
        updateTasksView()
    }

    private fun loadTabSelected() {
        if (tabSelected != null) {
            Log.i(TAG, "Selecting tab")
            binding.tasksTab.selectTab(tabSelected)
        }
        Log.i(TAG, "No tab selected")
    }

    override fun onStop() {
        super.onStop()
        notifyTasks()
    }

    private fun prepareTaskView(position: Int?) {
        when (position) {
            deadline_tasks -> prepareDeadlineTaskView()
            no_deadline_tasks -> prepareNoDeadlineTaskView()
            else -> Log.e(TAG, "Not supported task view: $position")
        }
    }

    private fun notifyTasks() {
        TodayAndPastTasksNotification().notifyTasks(applicationContext)
    }

    private fun prepareDeadlineTaskView() {
        binding.MainTasksList.apply {
            layoutManager = viewManager
            adapter = DeadlineTasksAdapter(
                storage,
                { binding.MainTasksList.adapter?.notifyDataSetChanged() },
                this@MainActivity,
                context
            )
        }
    }

    private fun prepareNoDeadlineTaskView() {
        binding.MainTasksList.apply {
            layoutManager = viewManager
            adapter = NoDeadlineTasksAdapter(
                storage,
                { binding.MainTasksList.adapter?.notifyDataSetChanged() },
                this@MainActivity,
                context
            )
        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.action_settings -> true
            R.id.remove_tasks_menu_item -> {
                removeAllTasks()
                return true
            }
            R.id.remove_completed_task_button -> {
                removeDoneTasks()
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }

    private fun removeDoneTasks() {
        storage.removeDoneTasks()
        updateTasksView()
    }

    private fun removeAllTasks() {
        Log.i(TAG, "Removal of all tasks")
        storage.clearTasks()
        updateTasksView()
    }

    private fun updateTasksView() {
        Log.i(TAG, "Update task view")
        prepareTaskView(binding.tasksTab.selectedTabPosition)
        binding.MainTasksList.adapter?.notifyDataSetChanged()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        Log.i(TAG, "Storing state")
        outState.putInt(tab_bundle_tag, binding.tasksTab.selectedTabPosition)
        super.onSaveInstanceState(outState)
    }
}
