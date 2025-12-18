package com.mojang.minecraftpe;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.craftsman.go.databinding.ActivityMainBinding;


public class MainActivity extends AppCompatActivity {

    // Used to load the 'sircbanner' library on application startup.
    static {
        System.loadLibrary("sircbanner");
    }
    public static MainActivity mInstance;

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mInstance = this;
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText("stringFromJNI");
//        onSetVolumeSystem(this);
    }
//    public native void onSetVolumeSystem(MainActivity mainActivity);
}