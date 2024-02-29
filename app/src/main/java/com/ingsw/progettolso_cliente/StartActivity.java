package com.ingsw.progettolso_cliente;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.airbnb.lottie.LottieAnimationView;

public class StartActivity extends AppCompatActivity {

    int posizione = 100;
    int numero_chiocciola = -1;
    Button buttonEntra;
    TextView textViewCoda;
    ProgressBar progressBar;
    LottieAnimationView cartAnimation;
    boolean wantToEnter = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start);

        buttonEntra = findViewById(R.id.enter_button);
        progressBar = findViewById(R.id.progressBar);
        textViewCoda = findViewById(R.id.textViewCodaIngresso);
        cartAnimation = findViewById(R.id.cartAnimation);




    }

    @Override
    protected void onResume() {
        super.onResume();
        reset();
    }

    public void reset() {
        wantToEnter = false;
        buttonEntra.setEnabled(true);
        progressBar.setVisibility(View.INVISIBLE);
        textViewCoda.setVisibility(View.INVISIBLE);
        cartAnimation.setVisibility(View.INVISIBLE);
        posizione = 100;
        numero_chiocciola = -1;
    }

    public void enter_market(View view) {
        buttonEntra.setEnabled(false);
        progressBar.setVisibility(View.VISIBLE);
        textViewCoda.setVisibility(View.VISIBLE);
        cartAnimation.setVisibility(View.VISIBLE);
        wantToEnter = true;
        updateCoda();
    }

    void updateCoda() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (posizione > 0) {
                    try {
                        ClientTask clientTask = new ClientTask(StartActivity.this, "cliente:" + numero_chiocciola + ":ingresso");
                        clientTask.execute();
                        while (clientTask.response == null) ;
                        Log.d("UpdateCoda", clientTask.response);
                        numero_chiocciola = Integer.parseInt(clientTask.response.split(":")[1]);
                        posizione = Integer.parseInt(clientTask.response.split(":")[2]);
                        Log.d("UpdateCoda", "Numero chiocciola: " + numero_chiocciola);
                        Log.d("UpdateCoda", "Posizione: " + posizione);
                        textViewCoda.setText("Posizione in coda: " + posizione);
                        Thread.sleep(4000);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                provaAdEntrare();
            }
        }).start();
    }

    void provaAdEntrare() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                boolean entrato = false;
                do {
                    textViewCoda.setText("Attendi che ci sia spazio nel supermercato...");
                    ClientTask clientTask = new ClientTask(StartActivity.this, "cliente:-1:entra");
                    clientTask.execute();
                    while(clientTask.response == null);
                    if (clientTask.response.contains("Non puoi entrare")) {
                        Log.d("CheckEntra", "Non puoi entrare");
                        entrato = false;
                    } else {
                        textViewCoda.setText("Puoi entrare!");
                        Log.d("CheckEntra", "Puoi entrare");
                        SharedPreferencesID.saveId(StartActivity.this, clientTask.response.split(":")[1]);
                        entrato = true;
                    }
                    try {
                        Thread.sleep(4000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }while(!entrato);
                gotoMainActivity();
            }
        }).start();
    }


    private void gotoMainActivity() {
        Intent intent = new Intent(this, MainActivity.class);
        wantToEnter = false;
        startActivity(intent);
    }

}