package me.xcyoung.android.gpu.compute;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Test test = new Test();
        findViewById(R.id.btnCLAdd).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test.nativeCLAdd();
            }
        });

        findViewById(R.id.btnCLInfo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test.nativeCLInfo();
            }
        });
    }
}