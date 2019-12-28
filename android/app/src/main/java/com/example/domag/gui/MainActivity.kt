package com.example.domag.gui

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.domag.R
import com.example.domag.notifications.Alarm
import com.example.domag.notifications.Notifications
import com.example.domag.notifications.TodayAndPastTasksNotification
import com.example.domag.storage.DataStorage
import com.example.domag.storage.DataStorageFactory
import com.example.domag.tasks.NoDeadlineTask
import com.google.android.material.tabs.TabLayout
import kotlinx.android.synthetic.main.activity_main.*

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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.i(TAG, "Create")
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)

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
            tabSelected = tasks_tab.getTabAt(lastSelectedTabPosition)
        }

        tasks_tab.addOnTabSelectedListener(
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

        addNewTaskButton.setOnClickListener {
            val intent = Intent(this, TaskEditActivity::class.java)
            if (tasks_tab.selectedTabPosition == no_deadline_tasks)
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
            tasks_tab.selectTab(tabSelected)
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
        MainTasksList.apply {
            layoutManager = viewManager
            adapter = DeadlineTasksAdapter(
                storage,
                { MainTasksList.adapter?.notifyDataSetChanged() },
                this@MainActivity,
                context
            )
        }
    }

    private fun prepareNoDeadlineTaskView() {
        MainTasksList.apply {
            layoutManager = viewManager
            adapter = NoDeadlineTasksAdapter(
                storage,
                { MainTasksList.adapter?.notifyDataSetChanged() },
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
        prepareTaskView(tasks_tab.selectedTabPosition)
        MainTasksList.adapter?.notifyDataSetChanged()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        Log.i(TAG, "Storing state")
        outState.putInt(tab_bundle_tag, tasks_tab.selectedTabPosition)
        super.onSaveInstanceState(outState)
    }
}
