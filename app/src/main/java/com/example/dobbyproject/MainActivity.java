/*
 * ═══════════════════════════════════════════════════════════════════════
 *  DobbyProject - 注入流程说明
 * ═══════════════════════════════════════════════════════════════════════
 *
 *  onCreate() 启动后依次执行:
 *
 *  1. setSelinuxPermissive()
 *     └─ su → setenforce 0  (设置 SELinux 为宽容模式)
 *
 *  2. fixInputPermission()
 *     └─ su → chmod 666 /dev/input/event*  (开放触摸输入设备权限)
 *
 *  3. writeFiletoTargetProgram()
 *     └─ su -M → cp libdobbyproject.so → /data/data/com.tencent.lolm/files/
 *     └─ chmod 777  (拷贝 SO 到目标应用目录)
 *
 *  4. launchAndInject()  [子线程]
 *     ├─ ls -la 检查 SO 文件是否就位
 *     ├─ su → cp libinjector.so → /data/local/tmp/injector + chmod 755
 *     ├─ su → am start 启动 LoL 进程 (备用: monkey)
 *     ├─ sleep 15s  (等待目标进程初始化)
 *     └─ su → /data/local/tmp/injector com.tencent.lolm <soPath>
 *            ├─ findPidByName() → pidof 查找目标 PID
 *            ├─ ptrace(ATTACH) → 附加到目标进程 (root)
 *            ├─ 远程 mmap → 在目标进程分配内存
 *            ├─ 远程 dlopen → 加载 libdobbyproject.so
 *            ├─ 远程 dlsym → 查找 _Z12MyStartPointPvS_S_S_S_
 *            ├─ 读取 libil2cpp.so 偏移处的 IL2CPP 元数据指针:
 *            │   +0xF45D838 → pCodeRegistration
 *            │   +0xF45D840 → pMetadataRegistration
 *            │   +0xF45D858 → pGlobalMetadataHeader
 *            │   +0x1D21140 → pMetadataImagesTable
 *            ├─ 远程调用 MyStartPoint(base, codeReg, metaReg, globalMeta, metaImages)
 *            ├─ 远程 munmap → 释放临时内存
 *            └─ ptrace(DETACH) → 恢复寄存器并分离
 *
 * ═══════════════════════════════════════════════════════════════════════
 */
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

        //设置宽容模式
        setSelinuxPermissive();

        //修改输入设备权限
        fixInputPermission();

        //全局拷贝 so 到目标程序
        writeFiletoTargetProgram();

        //启动目标进程并注入 SO
        launchAndInject();

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    public native String stringFromJNI();
    public native int injectSoToTarget(String packageName, String soPath);

    public void setSelinuxPermissive() {
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("setenforce 0\n");
            os.writeBytes("exit\n");
            os.flush();

            int exitCode = p.waitFor();
            if (exitCode == 0) {
                LogUtil.i("SELinux 已设置为宽容模式 (Permissive)");
            } else {
                LogUtil.e("设置 SELinux 宽容模式失败，错误码: " + exitCode, null);
            }
        } catch (Exception e) {
            LogUtil.e("设置 SELinux 异常: " + e.getMessage(), e);
        }
    }

    public void fixInputPermission() {
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("chmod 666 /dev/input/event*\n");
            os.writeBytes("exit\n");
            os.flush();

            int exitCode = p.waitFor();
            if (exitCode == 0) {
                LogUtil.i("输入设备权限修改成功");
            } else {
                LogUtil.e("输入设备权限修改失败，错误码: " + exitCode, null);
            }
        } catch (Exception e) {
            LogUtil.e("修改输入设备权限异常: " + e.getMessage(), e);
        }
    }

    public void launchAndInject() {
        String soPath = "/data/data/" + g_packFileName + "/files/libdobbyproject.so";
        String injectorSrc = g_nativeLibPath + (g_nativeLibPath.endsWith("/") ? "" : "/") + "libinjector.so";
        String injectorDst = "/data/local/tmp/injector";

        new Thread(() -> {
            // 检查 SO 文件是否存在
            LogUtil.i("[注入流程] 检查 SO 文件: " + soPath);
            try {
                Process chk = Runtime.getRuntime().exec("su");
                DataOutputStream chkOs = new DataOutputStream(chk.getOutputStream());
                chkOs.writeBytes("ls -la " + soPath + "\n");
                chkOs.writeBytes("exit\n");
                chkOs.flush();
                BufferedReader chkReader = new BufferedReader(new InputStreamReader(chk.getInputStream()));
                String chkLine;
                while ((chkLine = chkReader.readLine()) != null) {
                    LogUtil.i("[注入流程] SO 文件信息: " + chkLine);
                }
                chk.waitFor();
            } catch (Exception e) {
                LogUtil.e("[注入流程] 检查 SO 文件异常: " + e.getMessage(), e);
            }

            // 部署 injector 可执行文件
            LogUtil.i("[注入流程] 部署 injector: " + injectorSrc + " -> " + injectorDst);
            try {
                Process dep = Runtime.getRuntime().exec("su");
                DataOutputStream depOs = new DataOutputStream(dep.getOutputStream());
                depOs.writeBytes("cp -f " + injectorSrc + " " + injectorDst + "\n");
                depOs.writeBytes("chmod 755 " + injectorDst + "\n");
                depOs.writeBytes("exit\n");
                depOs.flush();
                dep.waitFor();
                LogUtil.i("[注入流程] injector 部署完成");
            } catch (Exception e) {
                LogUtil.e("[注入流程] 部署 injector 异常: " + e.getMessage(), e);
            }

            // 启动目标应用
            LogUtil.i("[注入流程] 正在启动目标应用: " + g_packFileName);
            try {
                Process p = Runtime.getRuntime().exec("su");
                DataOutputStream os = new DataOutputStream(p.getOutputStream());
                os.writeBytes("am start -n " + g_packFileName + "/com.riotgames.league.RiotNativeActivity\n");
                os.writeBytes("exit\n");
                os.flush();
                int exitCode = p.waitFor();
                if (exitCode == 0) {
                    LogUtil.i("[注入流程] 目标应用启动成功");
                } else {
                    LogUtil.i("[注入流程] am start 失败, 尝试 monkey 启动...");
                    Process p2 = Runtime.getRuntime().exec("su");
                    DataOutputStream os2 = new DataOutputStream(p2.getOutputStream());
                    os2.writeBytes("monkey -p " + g_packFileName + " -c android.intent.category.LAUNCHER 1\n");
                    os2.writeBytes("exit\n");
                    os2.flush();
                    p2.waitFor();
                    LogUtil.i("[注入流程] monkey 启动完成");
                }
            } catch (Exception e) {
                LogUtil.e("[注入流程] 启动目标应用异常: " + e.getMessage(), e);
            }

            // 等待 15 秒让目标进程完成初始化
            LogUtil.i("[注入流程] 等待 15 秒让目标进程初始化...");
            try { Thread.sleep(15000); } catch (InterruptedException ignored) {}

            // 通过 su 执行 injector (root 权限, 可以 ptrace)
            LogUtil.i("[注入流程] 以 root 身份执行 injector...");
            long startTime = System.currentTimeMillis();
            try {
                Process p = Runtime.getRuntime().exec("su");
                DataOutputStream os = new DataOutputStream(p.getOutputStream());
                os.writeBytes(injectorDst + " " + g_packFileName + " " + soPath + "\n");
                os.writeBytes("exit\n");
                os.flush();

                BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
                BufferedReader errReader = new BufferedReader(new InputStreamReader(p.getErrorStream()));
                String line;
                while ((line = reader.readLine()) != null) {
                    LogUtil.i("[注入流程] " + line);
                }
                while ((line = errReader.readLine()) != null) {
                    LogUtil.e("[注入流程] stderr: " + line, null);
                }

                int exitCode = p.waitFor();
                long elapsed = System.currentTimeMillis() - startTime;

                if (exitCode == 0) {
                    LogUtil.i("[注入流程] ✓ 注入成功! 耗时: " + elapsed + "ms");
                } else {
                    LogUtil.e("[注入流程] ✗ 注入失败, 退出码: " + exitCode + " 耗时: " + elapsed + "ms", null);
                }
            } catch (Exception e) {
                LogUtil.e("[注入流程] 执行 injector 异常: " + e.getMessage(), e);
            }
        }).start();
    }

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