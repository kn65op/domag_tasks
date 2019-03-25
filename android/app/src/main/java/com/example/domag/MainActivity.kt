package com.example.domag

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.domag.gui.SimpleTaskEditActivity
import com.example.domag.gui.TasksAdapter
import com.example.domag.notifications.Alarm
import com.example.domag.notifications.Notifications
import com.example.domag.notifications.TodayAndPastTasksNotification
import com.example.domag.storage.DataStorage
import com.example.domag.storage.DataStorageFactory
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    companion object {
        const val TAG = "MainActivity"
    }

    private lateinit var storage: DataStorage
    private lateinit var viewManager: RecyclerView.LayoutManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)

        Alarm().start(applicationContext)
        Notifications().registerChannels(applicationContext)

        viewManager = LinearLayoutManager(this)
        storage = DataStorageFactory().createDriveDataStorageFactory(applicationContext)

        prepareTaskView()

        addNewTaskButton.setOnClickListener {
            val intent = Intent(this, SimpleTaskEditActivity::class.java)
            startActivity(intent)
        }
    }

    override fun onResume() {
        super.onResume()
        updateTasksView()
        TodayAndPastTasksNotification().notifyTasks(applicationContext)
    }

    private fun prepareTaskView() {
        MainTasksList.apply {
            layoutManager = viewManager
            adapter = TasksAdapter(
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
            R.id.remove_complteted_task_button -> {
                removeDoneTasks()
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }

    private fun removeDoneTasks() {
        storage.removeDoneTasks()
    }

    private fun removeAllTasks() {
        Log.i(TAG, "Removal of all tasks")
        storage.clearTasks()
        updateTasksView()
    }

    private fun updateTasksView() {
        Log.i(TAG, "Update task view")
        MainTasksList.adapter?.notifyDataSetChanged()
    }
}
