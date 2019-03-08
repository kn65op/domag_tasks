package com.example.domag.gui.utils

import android.widget.EditText

fun EditText.replaceText(text : String) : Unit {
    this.text.replace(0, this.text.length, text)
}
