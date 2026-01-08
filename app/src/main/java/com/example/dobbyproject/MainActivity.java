package com.example.dobbyproject;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import com.example.dobbyproject.databinding.ActivityMainBinding;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.File;
public class MainActivity extends AppCompatActivity {

    // --- 1. LogUtil 保持不变 ---
    public static class LogUtil {
        private static final boolean DEBUG = true;
        private static final String TAG = "[SFK]";

        public static void d(String msg) {
            if (DEBUG) android.util.Log.d(TAG, msg);
        }

        public static void e(String msg, Throwable tr) {
            if (DEBUG) android.util.Log.e(TAG, msg, tr);
        }

        public static void i(String msg) {
            if (DEBUG) {
                // 这里 [3] 是正确的，对应调用 i() 的位置
                StackTraceElement element = Thread.currentThread().getStackTrace()[3];
                String info = "[" + element.getFileName() + ":" + element.getLineNumber() + "] ";
                android.util.Log.i(TAG, info + msg);
            }
        }
    }

    // --- 2. 静态块加载 SO ---
    static {
        System.loadLibrary("dobbyproject");
    }

    String g_packFileName = "com.tencent.lolm";

    String g_nativeLibPath = "";

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //获取 so 路径
        g_nativeLibPath = getApplicationContext().getApplicationInfo().nativeLibraryDir;


        writeFiletoTargetProgram();

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    public native String stringFromJNI();

    public void writeFiletoTargetProgram() {
        String srcFile = g_nativeLibPath + (g_nativeLibPath.endsWith("/") ? "" : "/") + "libdobbyproject.so";
        String dstDir = "/data/data/" + g_packFileName + "/files";
        String dstFile = dstDir + "/libdobbyproject.so";

        LogUtil.i(srcFile.toString());

        LogUtil.i("KSU 尝试全局拷贝 -> " + dstFile);

        try {
            // 1. 修改为 -M 参数 (Mount Master)
            Process p = Runtime.getRuntime().exec("su -M");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());

            // 2. 写入指令
            String cmd = "mkdir -p " + dstDir + "\n" +
                    "cp -f " + srcFile + " " + dstFile + "\n" +
                    "chmod 777 " + dstFile + "\n" +
                    "sync\n" +
                    "exit\n";

            LogUtil.i(cmd.toString());

            os.writeBytes(cmd);
            os.flush();

            // 3. 读取输出以确认 ls 状态
            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                LogUtil.i("KSU Output: " + line);
            }

            int exitCode = p.waitFor();
            if (exitCode == 0) {
                LogUtil.i("命令执行完成！请再次在 Shell 确认。");
            } else {
                LogUtil.e("执行失败，错误码: " + exitCode + "。检查参数是否正确。", null);
            }

        } catch (Exception e) {
            LogUtil.e("KSU 异常: " + e.getMessage(), e);
        }
    }
}