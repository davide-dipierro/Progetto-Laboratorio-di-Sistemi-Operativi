package com.ingsw.progettolso_cliente;

import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;

import org.json.*;

import java.net.Socket;
import java.io.*;

public class ClientTask extends AsyncTask<Void, Void, Void> {
    // Dichiaro le variabili
    private Socket socket;
    private PrintWriter printWriter;
    private BufferedReader bufferedReader;
    private String message;
    private String request;
    public String response;
    private MainActivity mainActivity;

    // Costruttore
    public ClientTask(MainActivity mainActivity, String request) {
        this.mainActivity = mainActivity;
        this.request = request;
    }

    @Override
    protected Void doInBackground(Void... voids) {
        try {
            socket = new Socket("192.168.1.254", 5050);
            printWriter = new PrintWriter(socket.getOutputStream(), true);
            printWriter.println(request);
            bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            while((message = bufferedReader.readLine()) != null) {
                response+=message;
            }
            socket.close();
        } catch (Exception e) {
            Log.e("Client error", e.getMessage());
            Toast.makeText(mainActivity, "Errore di connessione", Toast.LENGTH_LONG).show();
        }
        return null;
    }

    @Override
    protected void onPostExecute(Void aVoid) {
        super.onPostExecute(aVoid);
        Log.d("Request", request);
        Log.d("Response", response);
        if (request.equals("catalogo")) processCatalogo();
        //ClientTask clientTask = new ClientTask(mainActivity, "cliente:"+ id +":aggiungi\n:" + prodotto.getId() + ":" + prodotto.getNome() + ":" + prodotto.getPrezzo());
        else if(request.contains("aggiungi")) processAcquisto();
    }
    void processCatalogo(){
        Log.d("Response", response);
        // C'è un 'null' all'inizio della stringa, quindi lo rimuovo
        response = response.substring(4);
        try{
            JSONObject jsonObject = new JSONObject(response);
            JSONArray jsonArray = jsonObject.getJSONArray("prodotti");
            for(int i = 0; i < jsonArray.length(); i++) {
                JSONObject prodotto = jsonArray.getJSONObject(i);

                mainActivity.prodotti.add(new Prodotto(prodotto.getInt("id"), prodotto.getString("nome"), prodotto.getDouble("prezzo")));
            }
        } catch (JSONException e) {
            Log.e("JSON error", e.getMessage());
        }

        // Aggiorno la lista
        mainActivity.adapter.notifyDataSetChanged();
    }

    void processAcquisto(){
        // Rimuovo il 'null' all'inizio della stringa
        response = response.substring(4);
        Log.e("Process acquisto", "Response: " + response);
        if(response.equals("ok")) {
            // Aggiorno il totale
            Log.d("Request", request);
            // cliente:0:aggiungi\n:9:Cereali:3.99
            String[] split = request.split(":");
            mainActivity.totale += Double.parseDouble(split[split.length - 1]);
            // Aggiorno il testo approssimando il totale a 2 cifre decimali
            mainActivity.textViewTotale.setText("Totale: " + String.format("%.2f", mainActivity.totale) + "€");
        }else{
            Log.e("Sessione scaduta", response);
            Toast.makeText(mainActivity, "Sessione scaduta", Toast.LENGTH_LONG).show();
            // Torna a start_activity
            mainActivity.finish();
        }
        Log.d("Response", response);
    }

}
