package io.github.kn65op.domag_tasks.gui.utils

import android.widget.EditText

fun EditText.replaceText(text: String) {
    this.text.replace(0, this.text.length, text)
}
