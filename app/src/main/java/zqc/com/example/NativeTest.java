package zqc.com.example;

import java.util.List;

/**
 * Created by zhangqianchu on 2018/2/5.
 */

public class NativeTest {

    public static int myStatic = 10;
    public int normal = 39;

    static {
        System.loadLibrary("native-lib");
    }

    //定义一个native方法，然后传入基本数据类型和String型
    public native void java2jniMethod1(boolean b, int i, float f, String s);
    //向native传递数组类型
    public native void java2jniMethod2(int[] as, String[] strs);
    //Java向native传自定义类对象
    public native void java2jniMethod3(Person person);
    //Java向native传List对象
    public native void java2jniMethod4(List<Person> people);

    //native向java返回字符串类型
    public native String jni2javaMethod1();
    //native向java返回java对象
    public native Person jni2javaMethod2();
    //native向java返回基本数组类型
    public native int[] jni2javaMethod3();
    //native向java返回对象数组类型
    public native Person[] jni2javaMethod4();
    //native向Java返回List对象
    public native List<Person> jni2javaMethod5();

    //对类的静态变量进行操作
    public native void jniSetStaticField();
    //对类的实例变量进行操作
    public native void jniSetField();

    //局部引用，全局引用，弱全局引用
    //千万不要缓存局部引用，局部引用在方法返回后，jvm就会释放该局部引用，缓存的地址指向的内容已经无效了。
    public native void jniLocalRef();
    //全局引用
    public native void jniGlobalRef();
    //弱全局引用
    public native void jniWeakGlobalRef();
}
