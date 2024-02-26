package com.ingsw.progettolso_cliente;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

public class CodaCasseActivity extends AppCompatActivity {

    int id;
    TextView textViewCoda;
    int posizione = -1;

    Thread threadCoda;
    boolean wantToPay = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_coda_casse);
        id = SharedPreferencesID.getId(this);
        textViewCoda = findViewById(R.id.textViewCoda);

        // Eseguo il metodo updateCoda ogni 5 secondi per aggiornare la coda

        threadCoda = new Thread(new Runnable() {
            @Override
            public void run() {
                while(true){
                    try {
                        if (wantToPay) updateCoda();
                        Thread.sleep(5000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        threadCoda.start();
    }

    // Metodo per aggiornare la coda
    public void updateCoda(){
        // Aggiorno la coda
        if(posizione != 0) {
            ClientTask clientTask = new ClientTask(this, "cliente:" + id + ":coda");
            clientTask.execute();
            while(clientTask.response == null) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        } else {
            ClientTask clientTask = new ClientTask(this, "cliente:" + id + ":paga");
            clientTask.execute();
            while(clientTask.response == null) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }


    public void gotoExitActivity() {
        Intent intent = new Intent(this, ExitActivity.class);
        startActivity(intent);
    }
}