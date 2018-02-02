#include <jni.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <android/log.h>

using namespace std;

#define TAG "myndk"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

extern "C"
JNIEXPORT jstring

JNICALL
Java_zqc_com_example_NativeTest_stringFromJNI(
        JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_java2jniMethod1(JNIEnv *env, jobject instance, jboolean b, jint i,
                                                jfloat f, jstring s_) {
    //在native层会把string转换成c/c++都特别熟悉的char*，由char*可以转string,wstring等等。
    //在Java层String是对象，这里讲char*指针指向了该对象，在方法结束的时候记得要是否该指针引用
    if (b == JNI_TRUE) {
        LOGE("b is true");
    } else {
        LOGE("b is false");
    }
    float nativi = i + f;
    LOGE("native i: %f", nativi);
    const char *s = env->GetStringUTFChars(s_, 0);
    LOGE("native string: %s", s);
    env->ReleaseStringUTFChars(s_, s);
}
extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_java2jniMethod2(JNIEnv *env, jobject instance, jintArray as_,
                                                jobjectArray strs) {
    //获取数组里面内容
    jint *as = env->GetIntArrayElements(as_, NULL);
    int result = 0, len = env->GetArrayLength(as_);
    for (int i = 0; i < len; ++i) {
        result += as[i];
    }
    LOGE("intarray sum is %d", result);
    //方法native的引用
    env->ReleaseIntArrayElements(as_, as, 0);
    //这里可以看出String[]对应的是jobjectArray
    len = env->GetArrayLength(strs);
    for (int i = 0; i < len; ++i) {
        jstring temp = (jstring) env->GetObjectArrayElement(strs, i);
        const char *ctemp = env->GetStringUTFChars(temp, JNI_FALSE);
        LOGE("第%d个：%s", i, ctemp);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_java2jniMethod3(JNIEnv *env, jobject instance, jobject person) {
    jclass cls = env->FindClass("zqc/com/example/Person");
    if (cls == 0) {
        LOGE("find class fail");
        return;
    }
    jmethodID mid_ID = env->GetMethodID(cls, "setId", "()V");
    jmethodID mid_Name = env->GetMethodID(cls, "setName", "(Ljava/lang/String;)V");
    jmethodID mid_Age = env->GetMethodID(cls, "setAge", "(I)V");
    if (mid_ID && mid_Name && mid_Age) {
        //调用该类的相关方法env->CallXXXMethod,其中静态方法应该使用:env->CallStaticXXXMethod
        env->CallVoidMethod(person, mid_ID, 100L);
        jstring name = env->NewStringUTF("Tianyu");
        env->CallVoidMethod(person, mid_Name, name);
        env->CallVoidMethod(person, mid_Age, 18);
        return;
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_java2jniMethod4(JNIEnv *env, jobject instance, jobject people) {
    //下面所有操作都得先判断是否为空。。。
    jclass cls = env->GetObjectClass(people);
    jclass pcls = env->FindClass("zqc/com/example/Person");
    //jmethodID getNameMid = env->GetMethodID(pcls, "getName", "()Ljava/lang/String;");
    jmethodID setNameMid = env->GetMethodID(pcls, "setName", "(Ljava/lang/String;)V");
    //获取List的get方法id
    jmethodID getMid = env->GetMethodID(cls, "get", "(I)Ljava/lang/Object;");
    //获取list的长度
    jmethodID sizeMid = env->GetMethodID(cls, "size", "()I");
    int len = env->CallIntMethod(people, sizeMid);
    for (int i = 0; i < len; ++i) {
        //获取第i个元素
        jobject data = env->CallObjectMethod(people, getMid, i);
        env->CallVoidMethod(data, setNameMid, env->NewStringUTF("全部随我native"));
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_zqc_com_example_NativeTest_jni2javaMethod1(JNIEnv *env, jobject instance) {
    //jstring NewStringUTF(const char* bytes),jstring NewString(const jchar* unicodeChars, jsize len)
    char *returnValue = "你在native做你的操作后，生成char*后，通过env->NewStringUTF即可返回Java的String类型";
    return env->NewStringUTF(returnValue);
}
extern "C"
JNIEXPORT jobject JNICALL
Java_zqc_com_example_NativeTest_jni2javaMethod2(JNIEnv *env, jobject instance) {
    jclass cls = env->FindClass("zqc/com/example/Person");
    jmethodID cmid = env->GetMethodID(cls, "<init>", "()V");
    jmethodID setNameMid = env->GetMethodID(cls, "setName", "(Ljava/lang/String;)V");
    jobject obj = env->NewObject(cls, cmid);
    env->CallVoidMethod(obj, setNameMid, env->NewStringUTF("天宇"));
    return obj;
}
extern "C"
JNIEXPORT jintArray JNICALL
Java_zqc_com_example_NativeTest_jni2javaMethod3(JNIEnv *env, jobject instance) {
    int nat[] = {2, 1, 4, 3, 5};
    jintArray jnat = env->NewIntArray(5);
    env->SetIntArrayRegion(jnat, 0, 5, nat);
    return jnat;
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_zqc_com_example_NativeTest_jni2javaMethod4(JNIEnv *env, jobject instance) {
    jclass cls = env->FindClass("zqc/com/example/Person");
    jmethodID cMid = env->GetMethodID(cls, "<init>", "()V");
    jclass pcls = env->FindClass("zqc/com/example/Person");
    jmethodID cmid = env->GetMethodID(pcls, "<init>", "()V");
    jmethodID setNameMid = env->GetMethodID(pcls, "setName", "(Ljava/lang/String;)V");
    jmethodID setAgeMid = env->GetMethodID(pcls, "setAge", "(I)V");
    jobject obj = env->NewObject(pcls, cmid);
    env->CallVoidMethod(obj, setNameMid, env->NewStringUTF("天宇"));

    int len = 3;
    jobjectArray joa = env->NewObjectArray(len, cls, obj);
    for (int i = 0; i < len; ++i) {
        jobject tmp = env->GetObjectArrayElement(joa,i);
        env->CallVoidMethod(tmp, setAgeMid, i + 10);
    }
    return joa;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_zqc_com_example_NativeTest_jni2javaMethod5(JNIEnv *env, jobject instance) {
    jclass listCls = env->FindClass("java/util/ArrayList");//获得ArrayList类引用
    jmethodID  listCon = env->GetMethodID(listCls, "<init>", "()V");
    jmethodID addMid = env->GetMethodID(listCls,"add","(Ljava/lang/Object;)Z");

    jobject listObj = env->NewObject(listCls, listCon);
    jobject jperon = Java_zqc_com_example_NativeTest_jni2javaMethod2(env, instance);
    env->CallBooleanMethod(listObj, addMid, jperon);

    return listObj;
}