package com.ingsw.progettolso_cliente;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {


    List<Prodotto> prodotti = new ArrayList<>();
    ProdottoAdapter adapter;
    TextView textViewTotale;
    Button buttonVaiAlleCasse;

    double totale = 0;

    int id;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Ottengo l'id del cliente
        id = SharedPreferencesID.getId(this);
        // Creo un oggetto di tipo ClientTask e lo eseguo
        ClientTask clientTask2 = new ClientTask(this, "catalogo");
        clientTask2.execute();

        // Creo una lista di prodotti
        ListView listView = findViewById(R.id.listView);
        adapter = new ProdottoAdapter(this, prodotti, id);
        adapter.setMainActivity(this);
        listView.setAdapter(adapter);

        textViewTotale = findViewById(R.id.textViewTotale);
        buttonVaiAlleCasse = findViewById(R.id.casseButton);
    }

    public void updateTotale(double prezzo){
        totale += prezzo;
        textViewTotale.setText("Totale: " + String.format("%.2f", totale) + "€");
    }

    // onClickListener per il pulsante "Vai alle casse"
    public void vaiAlleCasse(View view){
        // Avvio l'activity CodaCasseActivity
        Intent intent = new Intent(this, CodaCasseActivity.class);
        startActivity(intent);
    }
}
