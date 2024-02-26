package com.ingsw.progettolso_cliente;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;

import java.util.List;

public class ScontrinoAdapter extends ArrayAdapter<Prodotto> {

    int id;
    ExitActivity mainActivity;
    public ScontrinoAdapter(Context context, List<Prodotto> prodotti, int id) {
        super(context, 0, prodotti);
        this.id = id;
    }

    void setMainActivity(ExitActivity mainActivity){
        this.mainActivity = mainActivity;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // Ottieni l'oggetto Prodotto per questa posizione
        Prodotto prodotto = getItem(position);

        // Controlla se una vista riutilizzata è disponibile, altrimenti infla una nuova vista
        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.scontrino_item_layout, parent, false);
        }

        // Trova i TextView e il pulsante nella vista
        TextView textViewId = convertView.findViewById(R.id.textViewId);
        TextView textViewNome = convertView.findViewById(R.id.textViewNome);
        TextView textViewPrezzo = convertView.findViewById(R.id.textViewPrezzo);

        // Imposta i valori degli attributi del prodotto nei TextView
        textViewId.setText("ID: " + prodotto.getId());
        textViewNome.setText("Nome: " + prodotto.getNome());
        textViewPrezzo.setText("Prezzo: " + String.format("%.2f", prodotto.getPrezzo()) + "€");

        return convertView;
    }
}