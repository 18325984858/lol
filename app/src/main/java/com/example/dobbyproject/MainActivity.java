package com.example.dobbyproject;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.dobbyproject.databinding.ActivityMainBinding;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;


public class MainActivity extends AppCompatActivity {

    String soFileName = "C:\\Users\\Song\\Desktop\\1\\libdobbyproject.so";
     String soTmpFileName = "/data/local/tmp/";
    // Used to load the 'dobbyproject' library on application startup.
    static {
        System.loadLibrary("dobbyproject");
    }

    String packFileName = "com.DefaultCompany.Demo1";


    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }




    /**
     * A native method that is implemented by the 'dobbyproject' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}