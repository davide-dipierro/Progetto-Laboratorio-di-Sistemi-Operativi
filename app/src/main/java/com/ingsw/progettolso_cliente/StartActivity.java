package com.ingsw.progettolso_cliente;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

public class StartActivity extends AppCompatActivity {

    int posizione = -1;
    int numero_chiocciola = -1;
    Button buttonEntra;
    TextView textViewCoda;
    ProgressBar progressBar;
    boolean wantToEnter = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start);

        buttonEntra = findViewById(R.id.enter_button);
        progressBar = findViewById(R.id.progressBar);
        textViewCoda = findViewById(R.id.textViewCodaIngresso);


        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true){
                    try {
                        if(wantToEnter) updateCoda();
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();


    }

    public void enter_market(View view) {
        buttonEntra.setEnabled(false);
        progressBar.setVisibility(View.VISIBLE);
        textViewCoda.setVisibility(View.VISIBLE);
        wantToEnter = true;
    }

    void updateCoda() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                wantToEnter = false;
                ClientTask clientTask = new ClientTask(StartActivity.this, "cliente:" + numero_chiocciola + ":ingresso");
                clientTask.execute();
                while(clientTask.response == null);
                textViewCoda.setText("Posizione in coda: " + posizione);
                if (posizione == 0) {
                    textViewCoda.setText("È il tuo turno!");
                    while(!checkEntra()) {
                        try {
                            Thread.sleep(4000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                    gotoMainActivity();
                    return;
                }
                wantToEnter = true;
            }
        });
    }

    boolean checkEntra() {
        ClientTask clientTask = new ClientTask(this, "cliente:-1:entra");
        clientTask.execute();
        while(clientTask.response == null);
        if (clientTask.response.contains("Non puoi entrare")) {
            Log.d("CheckEntra", "Non puoi entrare");
            return false;
        } else {
            SharedPreferencesID.saveId(this, clientTask.response.split(":")[1]);
            Log.d("CheckEntra", "Puoi entrare");
            return true;
        }
    }


    private void gotoMainActivity() {
        Intent intent = new Intent(this, MainActivity.class);
        wantToEnter = false;
        startActivity(intent);
    }

}