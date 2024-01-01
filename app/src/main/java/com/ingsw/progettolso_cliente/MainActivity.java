package com.ingsw.progettolso_cliente;

import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
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

    double totale = 0;

    int id;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Ottengo l'id del cliente
        id = Integer.parseInt(getIntent().getStringExtra("id").trim());
        // Creo un oggetto di tipo ClientTask e lo eseguo
        ClientTask clientTask = new ClientTask(this, "catalogo");
        clientTask.execute();

        // Creo una lista di prodotti
        ListView listView = findViewById(R.id.listView);
        adapter = new ProdottoAdapter(this, prodotti, id);
        adapter.setMainActivity(this);
        listView.setAdapter(adapter);

        textViewTotale = findViewById(R.id.textViewTotale);
    }
}
