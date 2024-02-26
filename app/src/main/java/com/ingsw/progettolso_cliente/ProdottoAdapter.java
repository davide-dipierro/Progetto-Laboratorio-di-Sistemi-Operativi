package com.ingsw.progettolso_cliente;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class ProdottoAdapter extends ArrayAdapter<Prodotto> {

    int id;
    MainActivity mainActivity;
    public ProdottoAdapter(Context context, List<Prodotto> prodotti, int id) {
        super(context, 0, prodotti);
        this.id = id;
    }

    void setMainActivity(MainActivity mainActivity){
        this.mainActivity = mainActivity;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // Ottieni l'oggetto Prodotto per questa posizione
        Prodotto prodotto = getItem(position);

        // Controlla se una vista riutilizzata è disponibile, altrimenti infla una nuova vista
        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.list_item_layout, parent, false);
        }

        // Trova i TextView e il pulsante nella vista
        TextView textViewId = convertView.findViewById(R.id.textViewId);
        TextView textViewNome = convertView.findViewById(R.id.textViewNome);
        TextView textViewPrezzo = convertView.findViewById(R.id.textViewPrezzo);
        ImageButton buttonAggiungi = convertView.findViewById(R.id.buttonAggiungi);
        ImageButton buttonRimuovi = convertView.findViewById(R.id.buttonRimuovi);

        // Imposta i valori degli attributi del prodotto nei TextView
        textViewId.setText("ID: \n" + prodotto.getId());
        textViewNome.setText("Nome: \n" + prodotto.getNome());
        textViewPrezzo.setText("Prezzo: \n" + prodotto.getPrezzo());

        // Gestisci il clic sul pulsante (puoi aggiungere qui la logica desiderata)
        buttonAggiungi.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ClientTask clientTask = new ClientTask(mainActivity, "cliente:"+ id +":aggiungi\n:" + prodotto.getId() + ":" + prodotto.getNome() + ":" + prodotto.getPrezzo());
                clientTask.execute();
            }
        });

        buttonRimuovi.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ClientTask clientTask = new ClientTask(mainActivity, "cliente:"+ id +":rimuovi\n:" + prodotto.getId());
                clientTask.execute();
                while(clientTask.response == null) {
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                if(clientTask.response.contains("ok")) {
                    mainActivity.updateTotale(-prodotto.getPrezzo());
                }
                else {
                    mainActivity.updateTotale(0);
                }
            }
        });

        return convertView;
    }
}
