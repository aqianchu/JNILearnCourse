#include <jni.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <android/log.h>
#include <stdlib.h>
using namespace std;

#define TAG "myndk"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

////define __USE_ANDROID_LOG__ in makefile to enable android log
//#if defined(__ANDROID__) && defined(__USE_ANDROID_LOG__)
//#include <android/log.h>
//#define LOGV(...)   __android_log_print((int)ANDROID_LOG_VERBOSE, "ST_jni", __VA_ARGS__)
//#define LOGE(msg)  __android_log_print((int)ANDROID_LOG_ERROR, "ST_jni_dbg", "line:%3d %s", __LINE__, msg)
//#define LOGDBG(fmt, ...) __android_log_print((int)ANDROID_LOG_DEBUG, "ST_jni_dbg", "line:%3d " fmt, __LINE__, __VA_ARGS__)
//#else
//#define LOGV(...)
//#define LOGE(fmt)
//#define LOGDBG(fmt, ...)
//#endif

jstring charTojstring(JNIEnv* env, const char* pat) {
    //定义java String类 strClass
    jclass strClass = env->FindClass("java/lang/String");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = env->NewStringUTF("UTF-8");
    //将byte数组转换为java String,并输出
    return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
}

char* jstringToChar(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

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
        jobject tmp = env->GetObjectArrayElement(joa, i);
        env->CallVoidMethod(tmp, setAgeMid, i + 10);
    }
    return joa;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_zqc_com_example_NativeTest_jni2javaMethod5(JNIEnv *env, jobject instance) {
    jclass listCls = env->FindClass("java/util/ArrayList");//获得ArrayList类引用
    jmethodID listCon = env->GetMethodID(listCls, "<init>", "()V");
    jmethodID addMid = env->GetMethodID(listCls, "add", "(Ljava/lang/Object;)Z");

    jobject listObj = env->NewObject(listCls, listCon);
    jobject jperon = Java_zqc_com_example_NativeTest_jni2javaMethod2(env, instance);
    env->CallBooleanMethod(listObj, addMid, jperon);

    return listObj;
}
extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_jniSetStaticField(JNIEnv *env, jobject instance) {
    jclass cls = env->GetObjectClass(instance);
    if (cls == 0) {
        return;
    }
    jfieldID myStaticId = env->GetStaticFieldID(cls, "myStatic", "I");
    if (myStaticId == 0) {
        return;
    }
    env->SetStaticIntField(cls, myStaticId, 520);
}extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_jniSetField(JNIEnv *env, jobject instance) {
    jclass cls = env->GetObjectClass(instance);
    if (!cls) {
        return;
    }
    jfieldID nfieldId = env->GetFieldID(cls, "normal", "I");
    if (!nfieldId) {
        return;
    }
    env->SetIntField(instance, nfieldId, 640);
    //这里可以删除局部引用，局部引用在作用域结束后自动释放
    env->DeleteLocalRef(cls);
}

//局部引用：1.循环体内创建的局部引用，要在循环体内就直接释放了。
//2. 编写的工具函数，里面创建的局部引用，要在该工具函数里面释放了。
//3. 局部引用引用了一个大的Java对象，这时候一定一定要早点释放了。
//4. 局部引用不要缓存在native层

extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_jniLocalRef(JNIEnv *env, jobject instance) {
    //1. 局部引用不要存储在static变量中，即使存了下次也不能用
    //static jclass cls;
    //以下创建的局部引用都放入到栈中
    env->PushLocalFrame(16);
    jclass cls;
    if (!cls) {//这里就错误了，前一次方法完成后jvm会释放局部引用，这里static存的值仅第一次有效
        cls = env->GetObjectClass(instance);//这里的cls是局部引用
    }

    //删除栈里面的局部引用
    env->PopLocalFrame(NULL);
    env->EnsureLocalCapacity(20);//将本地引用的最大限制改为20
    //下面可以进行其他操作。。。
}

extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_jniGlobalRef(JNIEnv *env, jobject instance) {
    static jobject obj;
    static jclass pCls;
    if (obj) {//第二次点击时，这里就不会空
        //由于obj和personCls被保存为全局引用了，所有这里使用仍然有效
        jmethodID getId = env->GetMethodID(pCls, "getName", "()Ljava/lang/String;");
        jstring name = (jstring) env->CallObjectMethod(obj, getId);
        LOGE("obj is not null, name:%s", jstringToChar(env, name));
        return;
    }
    if (!pCls) {//为空就去新建
        jclass tmpCls = env->FindClass("zqc/com/example/Person");
        pCls = (jclass) env->NewGlobalRef(tmpCls);
        env->DeleteLocalRef(tmpCls);
    }
    jmethodID conMid = env->GetMethodID(pCls, "<init>", "()V");
    jobject tmpObj = env->NewObject(pCls, conMid);
    jmethodID setId = env->GetMethodID(pCls, "setName", "(Ljava/lang/String;)V");
    env->CallVoidMethod(tmpObj, setId, env->NewStringUTF("看看姓名"));
    obj = env->NewGlobalRef(tmpObj);
    env->DeleteLocalRef(tmpObj);
}
extern "C"
JNIEXPORT void JNICALL
Java_zqc_com_example_NativeTest_jniWeakGlobalRef(JNIEnv *env, jobject instance) {
    static jclass pCls;
    if (!pCls) {
        jclass tmpCls = env->FindClass("zqc/com/example/Person");
        pCls = (jclass) env->NewWeakGlobalRef(tmpCls);
        env->DeleteLocalRef(tmpCls);
    }
    //除了第一次需要FindClass外，在没有回收pCls之前都可以使用

    //这里使用...

    //可以手动释放
    //env->DeleteWeakGlobalRef(pCls);
}

void onLoadTest() {
    LOGE("调到我啦");
}

jstring onloadTest1(JNIEnv *env, jobject instance, jobject obj) {
    jclass pCls = env->GetObjectClass(obj);
    jfieldID nameFid = env->GetFieldID(pCls, "name", "Ljava/lang/String;");
    jstring name = (jstring) env->GetObjectField(obj, nameFid);
    char *cname = jstringToChar(env, name);
    char *tmp = new char[100];
    sprintf(tmp, "我来自Native，我叫：%s", cname);
    jstring result = charTojstring(env, tmp);
    return result;
}
//注册函数映射
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *pEnv = NULL;
    //获取环境
    jint ret = vm->GetEnv((void**) &pEnv, JNI_VERSION_1_6);
    if (ret != JNI_OK) {
        LOGE("jni_replace JVM ERROR:GetEnv");
        return -1;
    }

    JNINativeMethod g_Methods[] = {{"jniOnLoadTest", "()V", (void*) onLoadTest},
                                   {"jniOnload1", "(Lzqc/com/example/Person;)Ljava/lang/String;", (jstring*)onloadTest1}
    };
    jclass cls = pEnv->FindClass("zqc/com/example/NativeTest");
    if (cls == NULL) {
        LOGE("FindClass Error");
        return -1;
    }
    //动态注册本地方法
    ret = pEnv->RegisterNatives(cls, g_Methods,sizeof(g_Methods) / sizeof(g_Methods[0]));
    if (ret != JNI_OK) {
        LOGE("Register Error");
        return -1;
    }
    //返回java版本
    return JNI_VERSION_1_6;
}

static bool
unBindNative(JNIEnv *env) {
    jclass clazz;
    clazz = env->FindClass("zqc/com/example/NativeTest");
    if (clazz == NULL) {
        return false;
    }
    return env->UnregisterNatives(clazz) == 0;
}

//卸载函数映射
JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    bool res = unBindNative(env);
    LOGE("unbind result is %s", res ? "ok" : "error");
}
