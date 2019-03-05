package com.example.domag

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.domag.gui.AddTaskActivity
import com.example.domag.gui.TasksAdapter
import com.example.domag.storage.DataStorage
import com.example.domag.storage.DriveDataStorage
import com.example.domag.tasks.JsonTaskDeserializer
import com.example.domag.utils.platform.AndroidWrapper
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

        viewManager = LinearLayoutManager(this)
        storage = DriveDataStorage(applicationContext, AndroidWrapper(applicationContext ), taskDeserializer = JsonTaskDeserializer())

        prepareTaskView()

        addNewTaskButton.setOnClickListener {
            val intent = Intent(this, AddTaskActivity::class.java)
            startActivity(intent)
        }
    }

    override fun onResume() {
        super.onResume()
        updateTasksView()
    }

    private fun prepareTaskView() {
        MainTasksList.apply {
            layoutManager = viewManager
            adapter = TasksAdapter(storage)
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
                Log.i(TAG, "Removal of all tasks")
                storage.clearTasks()
                updateTasksView()
                return true
            }
            else -> super.onOptionsItemSelected(item)
        }
    }

    private fun updateTasksView() {
        Log.i(TAG, "Update task view")
        MainTasksList.adapter?.notifyDataSetChanged()
    }
}