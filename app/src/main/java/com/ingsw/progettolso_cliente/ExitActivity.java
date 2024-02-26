package com.ingsw.progettolso_cliente;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class ExitActivity extends AppCompatActivity {

    int id;
    List<Prodotto> prodotti = new ArrayList<>();
    double totale = 0;

    TextView textViewTotale;

    ScontrinoAdapter adapter;
    Button buttonEsci;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_exit);
        // Set toolbar title
        getSupportActionBar().setTitle("Scontrino");
        // Ottengo l'id del cliente
        id = SharedPreferencesID.getId(this);
        // Creo un oggetto di tipo ClientTask e lo eseguo
        ClientTask clientTask2 = new ClientTask(this, "cliente:" + id + ":stampa");
        clientTask2.execute();

        ListView listView = findViewById(R.id.listViewScontrino);
        adapter = new ScontrinoAdapter(this, prodotti, id);
        adapter.setMainActivity(this);
        listView.setAdapter(adapter);

        textViewTotale = findViewById(R.id.textViewTotale);
        buttonEsci = findViewById(R.id.esciButton);
    }

    public void updateTotale(double prezzo){
        totale += prezzo;
        textViewTotale.setText("Totale: " + String.format("%.2f", totale) + "€");
    }

    public void esciDalSupermercato(View view){
        ClientTask clientTask = new ClientTask(this, "cliente:" + id + ":esce");
        clientTask.execute();
        while (clientTask.response == null) ;
        Intent intent = new Intent(this, StartActivity.class);
        startActivity(intent);
    }
}