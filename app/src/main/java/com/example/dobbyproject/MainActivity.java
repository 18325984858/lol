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
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.io.FileOutputStream;
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

    // 状态跟踪
    private boolean selinuxDone = false;
    private boolean inputPermDone = false;
    private boolean fontDone = false;
    private boolean launchDone = false;
    private TextView tvStatus;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        g_nativeLibPath = getApplicationContext().getApplicationInfo().nativeLibraryDir;

        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        tvStatus = findViewById(R.id.tv_status);

        CheckBox cbDumper = findViewById(R.id.cb_dumper);
        CheckBox cbHeader = findViewById(R.id.cb_header);
        CheckBox cbLog = findViewById(R.id.cb_log);

        Button btnSelinux = findViewById(R.id.btn_selinux);
        Button btnInputPerm = findViewById(R.id.btn_input_perm);
        Button btnFont = findViewById(R.id.btn_deploy_font);
        Button btnLaunch = findViewById(R.id.btn_launch);

        // ── 启动时初始化检测 ──
        updateStatus("正在检测环境...");
        new Thread(() -> {
            boolean selinuxOk = checkSelinuxPermissive();
            boolean inputOk = checkInputPermission();
            boolean fontOk = checkFileExists("/data/local/tmp/chinese.ttf");
            boolean gameInstalled = checkGameInstalled();

            runOnUiThread(() -> {
                StringBuilder sb = new StringBuilder();
                if (selinuxOk) {
                    selinuxDone = true;
                    btnSelinux.setText("✅ 宽容模式已设置");
                    btnSelinux.setEnabled(false);
                    sb.append("宽容模式 ✓  ");
                }
                if (inputOk) {
                    inputPermDone = true;
                    btnInputPerm.setText("✅ 输入权限已修改");
                    btnInputPerm.setEnabled(false);
                    sb.append("输入权限 ✓  ");
                }
                if (fontOk) {
                    fontDone = true;
                    btnFont.setText("✅ 字体已部署");
                    btnFont.setEnabled(false);
                    sb.append("字体 ✓  ");
                }
                if (!gameInstalled) {
                    btnLaunch.setEnabled(false);
                    btnLaunch.setText("⚠ 游戏未安装");
                    sb.append("游戏未安装 ✗");
                } else {
                    sb.append("游戏已安装 ✓");
                }
                updateStatus(sb.length() > 0 ? sb.toString().trim() : "就绪");
            });
        }).start();

        // ① 设置宽容模式
        btnSelinux.setOnClickListener(v -> {
            if (selinuxDone) {
                Toast.makeText(this, "已设置过宽容模式，无需重复操作", Toast.LENGTH_SHORT).show();
                return;
            }
            btnSelinux.setEnabled(false);
            setSelinuxPermissive();
            selinuxDone = true;
            btnSelinux.setText("✅ 宽容模式已设置");
            updateStatus("宽容模式 ✓");
            Toast.makeText(this, "宽容模式已设置", Toast.LENGTH_SHORT).show();
        });

        // ② 修改输入设备权限
        btnInputPerm.setOnClickListener(v -> {
            if (inputPermDone) {
                Toast.makeText(this, "输入权限已修改过，无需重复操作", Toast.LENGTH_SHORT).show();
                return;
            }
            btnInputPerm.setEnabled(false);
            fixInputPermission();
            inputPermDone = true;
            btnInputPerm.setText("✅ 输入权限已修改");
            updateStatus("输入权限 ✓");
            Toast.makeText(this, "输入设备权限已修改", Toast.LENGTH_SHORT).show();
        });

        // ③ 部署中文字体
        btnFont.setOnClickListener(v -> {
            if (fontDone) {
                Toast.makeText(this, "字体已部署过，无需重复操作", Toast.LENGTH_SHORT).show();
                return;
            }
            btnFont.setEnabled(false);
            deployChineseFont();
            fontDone = true;
            btnFont.setText("✅ 字体已部署");
            updateStatus("字体部署 ✓");
            Toast.makeText(this, "字体部署完成", Toast.LENGTH_SHORT).show();
        });

        // ④ 启动游戏
        btnLaunch.setOnClickListener(v -> {
            if (launchDone) {
                Toast.makeText(this, "游戏已启动过，请勿重复注入", Toast.LENGTH_SHORT).show();
                return;
            }
            // 初始化检测: 确保前置步骤已完成
            if (!selinuxDone) {
                Toast.makeText(this, "请先设置宽容模式", Toast.LENGTH_SHORT).show();
                return;
            }

            btnLaunch.setEnabled(false);
            launchDone = true;

            boolean enableDumper = cbDumper.isChecked();
            boolean enableHeader = cbHeader.isChecked();
            boolean enableLog = cbLog.isChecked();

            // 清除旧的完成标记
            clearDumpMarkers();

            writeFiletoTargetProgram();
            launchAndInject(enableDumper, enableHeader, enableLog);

            // 如果勾选了 dump，启动后台轮询等待完成
            if (enableDumper || enableHeader) {
                pollDumpCompletion(enableDumper, enableHeader);
            }

            String options = "";
            if (enableDumper) options += " [Dumper]";
            if (enableHeader) options += " [Header]";
            btnLaunch.setText("✅ 游戏已启动" + options);
            updateStatus("游戏启动中..." + options);
            Toast.makeText(this, "正在启动游戏并注入..." + options, Toast.LENGTH_SHORT).show();
        });
    }

    private void updateStatus(String msg) {
        if (tvStatus != null) tvStatus.setText(msg);
    }

    /**
     * 清除旧的 dump 完成标记文件
     */
    private void clearDumpMarkers() {
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("rm -f /data/local/tmp/dobby_dumper_done /data/local/tmp/dobby_header_done\n");
            os.writeBytes("exit\n");
            os.flush();
            p.waitFor();
        } catch (Exception ignored) {}
    }

    /**
     * 后台轮询 dump 完成标记文件，完成后在 UI 线程弹 Toast
     */
    private void pollDumpCompletion(boolean waitDumper, boolean waitHeader) {
        new Thread(() -> {
            boolean dumperDone = !waitDumper;
            boolean headerDone = !waitHeader;
            int maxWait = 300; // 最多等 5 分钟 (300 × 1s)

            for (int i = 0; i < maxWait && (!dumperDone || !headerDone); i++) {
                try { Thread.sleep(1000); } catch (InterruptedException ignored) { return; }

                if (!dumperDone) {
                    dumperDone = checkFileExists("/data/local/tmp/dobby_dumper_done");
                    if (dumperDone) {
                        runOnUiThread(() -> {
                            Toast.makeText(this, "✓ il2cppDumper 导出完成！", Toast.LENGTH_LONG).show();
                            updateStatus("Dumper 导出完成 ✓");
                        });
                    }
                }
                if (!headerDone) {
                    headerDone = checkFileExists("/data/local/tmp/dobby_header_done");
                    if (headerDone) {
                        runOnUiThread(() -> {
                            Toast.makeText(this, "✓ il2cppHeader 导出完成！", Toast.LENGTH_LONG).show();
                            updateStatus("Header 导出完成 ✓");
                        });
                    }
                }
            }

            if (dumperDone && headerDone) {
                runOnUiThread(() -> {
                    String msg = "全部导出完成 ✓";
                    updateStatus(msg);
                    Toast.makeText(this, msg, Toast.LENGTH_LONG).show();
                });
            }
        }).start();
    }

    private boolean checkFileExists(String path) {
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("test -f " + path + " && echo YES\n");
            os.writeBytes("exit\n");
            os.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.contains("YES")) { p.waitFor(); return true; }
            }
            p.waitFor();
        } catch (Exception ignored) {}
        return false;
    }

    /**
     * 检测 SELinux 是否已经是 Permissive 模式
     */
    private boolean checkSelinuxPermissive() {
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("getenforce\n");
            os.writeBytes("exit\n");
            os.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.trim().equalsIgnoreCase("Permissive")) { p.waitFor(); return true; }
            }
            p.waitFor();
        } catch (Exception ignored) {}
        return false;
    }

    /**
     * 检测 /dev/input/event* 是否已有 666 权限 (other 可读写)
     */
    private boolean checkInputPermission() {
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("ls -l /dev/input/event0 | grep -q 'crw-rw-rw' && echo OK\n");
            os.writeBytes("exit\n");
            os.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.contains("OK")) { p.waitFor(); return true; }
            }
            p.waitFor();
        } catch (Exception ignored) {}
        return false;
    }

    /**
     * 检测目标游戏是否已安装（通过 root 权限绕过 Android 11+ 包可见性限制）
     */
    private boolean checkGameInstalled() {
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("pm list packages " + g_packFileName + " | grep -q " + g_packFileName + " && echo INSTALLED\n");
            os.writeBytes("exit\n");
            os.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.contains("INSTALLED")) { p.waitFor(); return true; }
            }
            p.waitFor();
        } catch (Exception ignored) {}
        return false;
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

    public void launchAndInject(boolean enableDumper, boolean enableHeader, boolean enableLog) {
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

            // 写入配置文件，告知 C++ 层是否启用 Dumper/Header
            try {
                Process cfgP = Runtime.getRuntime().exec("su");
                DataOutputStream cfgOs = new DataOutputStream(cfgP.getOutputStream());
                String cfgContent = "dumper=" + (enableDumper ? "1" : "0") + "\n"
                                  + "header=" + (enableHeader ? "1" : "0") + "\n"
                                  + "log=" + (enableLog ? "1" : "0") + "\n";
                cfgOs.writeBytes("echo '" + cfgContent + "' > /data/local/tmp/dobby_config.txt\n");
                cfgOs.writeBytes("chmod 644 /data/local/tmp/dobby_config.txt\n");
                cfgOs.writeBytes("exit\n");
                cfgOs.flush();
                cfgP.waitFor();
                LogUtil.i("[注入流程] 配置文件已写入: dumper=" + enableDumper + " header=" + enableHeader);
            } catch (Exception e) {
                LogUtil.e("[注入流程] 写入配置文件异常: " + e.getMessage(), e);
            }

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

    /**
     * 从 assets 提取 chinese.ttf 到 /data/local/tmp/chinese.ttf
     */
    public void deployChineseFont() {
        String tmpFile = getCacheDir() + "/chinese.ttf";
        String dstFile = "/data/local/tmp/chinese.ttf";

        // 检查目标是否已存在（避免重复复制）
        try {
            Process chk = Runtime.getRuntime().exec("su");
            DataOutputStream chkOs = new DataOutputStream(chk.getOutputStream());
            chkOs.writeBytes("test -f " + dstFile + " && echo EXISTS\n");
            chkOs.writeBytes("exit\n");
            chkOs.flush();
            BufferedReader chkReader = new BufferedReader(new InputStreamReader(chk.getInputStream()));
            String chkLine;
            boolean exists = false;
            while ((chkLine = chkReader.readLine()) != null) {
                if (chkLine.contains("EXISTS")) exists = true;
            }
            chk.waitFor();
            if (exists) {
                LogUtil.i("[Font] chinese.ttf 已存在于 " + dstFile + "，跳过部署");
                return;
            }
        } catch (Exception ignored) {}

        // 从 assets 提取到 app cache 目录
        try {
            InputStream is = getAssets().open("chinese.ttf");
            FileOutputStream fos = new FileOutputStream(tmpFile);
            byte[] buf = new byte[8192];
            int len;
            while ((len = is.read(buf)) > 0) {
                fos.write(buf, 0, len);
            }
            fos.close();
            is.close();
            LogUtil.i("[Font] 已从 assets 提取到: " + tmpFile);
        } catch (Exception e) {
            LogUtil.e("[Font] 提取 chinese.ttf 失败: " + e.getMessage(), e);
            return;
        }

        // 用 su 复制到 /data/local/tmp/
        try {
            Process p = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(p.getOutputStream());
            os.writeBytes("cp -f " + tmpFile + " " + dstFile + "\n");
            os.writeBytes("chmod 644 " + dstFile + "\n");
            os.writeBytes("exit\n");
            os.flush();
            int exitCode = p.waitFor();
            if (exitCode == 0) {
                LogUtil.i("[Font] ✓ chinese.ttf 已部署到 " + dstFile);
            } else {
                LogUtil.e("[Font] 部署失败，错误码: " + exitCode, null);
            }
        } catch (Exception e) {
            LogUtil.e("[Font] 部署异常: " + e.getMessage(), e);
        }
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