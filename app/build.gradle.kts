plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.example.dobbyproject"
    // 注意：建议确认 SDK 36 是否已正式发布，通常当前稳定版为 34 或 35
    compileSdk = 36

    defaultConfig {
        applicationId = "com.example.dobbyproject"
        minSdk = 24
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        // 支持 ARM + x86 模拟器架构
        externalNativeBuild {
            cmake {
                abiFilters.addAll(listOf("arm64-v8a", "armeabi-v7a", "x86", "x86_64"))
            }
        }
    }

    // --- 添加以下这段配置 ---
    packaging {
        jniLibs {
            // 强制将 SO 文件解压缩到 lib 目录，而不是留在 APK 中
            useLegacyPackaging = true
        }
    }
    // -----------------------

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    buildFeatures {
        viewBinding = true
    }
}

dependencies {
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
}