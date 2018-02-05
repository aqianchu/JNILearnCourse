package zqc.com.example;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Created by zhangqianchu on 2018/2/5.
 */

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.button).setOnClickListener(this);
        findViewById(R.id.button2).setOnClickListener(this);
        findViewById(R.id.button3).setOnClickListener(this);
        findViewById(R.id.button4).setOnClickListener(this);
        findViewById(R.id.button5).setOnClickListener(this);
        findViewById(R.id.button6).setOnClickListener(this);
        findViewById(R.id.button7).setOnClickListener(this);
        findViewById(R.id.button8).setOnClickListener(this);
        findViewById(R.id.button9).setOnClickListener(this);
        findViewById(R.id.button10).setOnClickListener(this);
        findViewById(R.id.button11).setOnClickListener(this);
        findViewById(R.id.button12).setOnClickListener(this);
        findViewById(R.id.button13).setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        NativeTest test = new NativeTest();
        switch (view.getId()) {
            case R.id.button:
                test.java2jniMethod1(true, 20, 34f, "天宇");
                break;
            case R.id.button2:
                int a[] = new int[3];
                for (int i = 0; i < a.length; i++) {
                    a[i] = i + 10;
                }
                String[] strs = new String[4];
                for (int i = 0; i < strs.length; i++) {
                    strs[i] = "我的值：" + i;
                }
                test.java2jniMethod2(a, strs);
                break;
            case R.id.button3:
                Person person = new Person();
                test.java2jniMethod3(person);
                Toast.makeText(this, person.toString(), Toast.LENGTH_SHORT).show();
                break;
            case R.id.button4:
                List<Person> people = new ArrayList<>();
                for (int i = 0; i < 3; i++) {
                    Person person1 = new Person();
                    person1.setName("我是Java层");
                    people.add(person1);
                }
                test.java2jniMethod4(people);
                Iterator<Person> iterator = people.iterator();
                while (iterator.hasNext()) {
                    Person person1 = iterator.next();
                    Log.e("myndk", person1.getName() + "\n");
                }
                break;
            case R.id.button5:
                String jni = test.jni2javaMethod1();
                Log.e("myndk", jni);
                break;
            case R.id.button6:
                Log.e("myndk", test.jni2javaMethod2().toString());
                break;
            case R.id.button7:
                int array[] = test.jni2javaMethod3();
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < array.length; i++) {
                    sb.append("第").append(i).append("个：").append(array[i]);
                }
                Log.e("myndk", sb.toString());
                break;
            case R.id.button8:
                Person[] people1 = test.jni2javaMethod4();
                StringBuilder sb1 = new StringBuilder();
                for (int i = 0; i < people1.length; i++) {
                    sb1.append("第").append(i).append("个：").append(people1[i]);
                }
                Log.e("myndk", sb1.toString());
                break;
            case R.id.button9:
                List<Person> people2 = test.jni2javaMethod5();
                StringBuilder sb2 = new StringBuilder();
                for (int i = 0; i < people2.size(); i++) {
                    sb2.append("第").append(i).append("个：").append(people2.get(i));
                }
                Log.e("myndk", sb2.toString());
                break;
            case R.id.button10:
                StringBuilder sb3 = new StringBuilder();
                sb3.append("修改前：").append(NativeTest.myStatic);
                test.jniSetStaticField();
                sb3.append("修改后：").append(NativeTest.myStatic);
                Log.e("myndk", sb3.toString());
                break;
            case R.id.button11:
                StringBuilder sb4 = new StringBuilder();
                sb4.append("修改前：").append(test.normal);
                test.jniSetField();
                sb4.append("修改后：").append(test.normal);
                Log.e("myndk", sb4.toString());
                break;
            case R.id.button12:
                test.jniLocalRef();
                break;
            case R.id.button13:
                test.jniGlobalRef();
                break;
        }
    }
}
