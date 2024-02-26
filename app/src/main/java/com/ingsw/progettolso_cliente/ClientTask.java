package com.ingsw.progettolso_cliente;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Looper;
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
    private Activity mainActivity;

    // Costruttore
    public ClientTask(Activity mainActivity, String request) {
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
            // Invio un messaggio all'utente con un Toast
            Looper.prepare();
            Toast.makeText(mainActivity, "Errore di connessione", Toast.LENGTH_LONG).show();
            Looper.loop();
        }
        return null;
    }

    @Override
    protected void onPostExecute(Void aVoid) {
        super.onPostExecute(aVoid);
        Log.d("Request", request);
        Log.d("Response", response);
        if (request.equals("catalogo")) processCatalogo();
        else if(request.contains("aggiungi")) processAcquisto();
        else if(request.contains("coda")) processCoda();
        else if(request.contains("paga")) processPaga();
        else if(request.contains("ingresso")) processIngresso();
        else if(request.contains("entra")) processEntra();
        else if(request.contains("stampa")) processStampa();
        else if(request.contains("rimuovi")) processRimuovi();
    }

    private void processRimuovi() {
        ;
    }


    private void processStampa() {
        Log.d("ProcessStampa", response);
        ExitActivity exitActivity = (ExitActivity) this.mainActivity;
        // Rimuovo il 'null' all'inizio della stringa
        response = response.substring(4);
        try{
            // {id:nome:prezzo}{id:nome:prezzo}...
            String[] prodotti = response.split("\\}\\{");
            for(String prodotto : prodotti){
                prodotto = prodotto.replace("{", "").replace("}", "");
                String[] split = prodotto.split(":");
                exitActivity.updateTotale(Double.parseDouble(split[2]));
                exitActivity.prodotti.add(new Prodotto(Integer.parseInt(split[0]), split[1], Double.parseDouble(split[2])));
            }
        } catch (Exception e) {
            Log.e("ProcessStampa", e.getMessage());
        }

        // Aggiorno la lista
        exitActivity.adapter.notifyDataSetChanged();
    }

    private void processEntra() {
        Log.d("ProcessEntra", response);
        StartActivity startActivity = (StartActivity) this.mainActivity;
        // Rimuovo il 'null' all'inizio della stringa
        response = response.substring(4);
    }

    private void processCoda() {
        Log.d("ProcessCoda", response);
        CodaCasseActivity codaCasseActivity = (CodaCasseActivity) this.mainActivity;
        // Rimuovo il 'null' all'inizio della stringa
        response = response.substring(4);
        codaCasseActivity.posizione = Integer.parseInt(response);
        codaCasseActivity.textViewCoda.setText("Sei in coda alle casse, ci sono " + response + " persone prima di te");
        if(response.equals("0")) codaCasseActivity.textViewCoda.setText("È il tuo turno!");
        else if(response.equals("1")) codaCasseActivity.textViewCoda.setText("Sei il prossimo!");
    }

    private void processIngresso(){
        Log.d("ProcessIngresso", response);
        StartActivity startActivity = (StartActivity) this.mainActivity;
        // Rimuovo il 'null' all'inizio della stringa
        response = response.substring(4);
        startActivity.numero_chiocciola = Integer.parseInt(response.split(":")[1]);
        startActivity.posizione = Integer.parseInt(response.split(":")[2]);
    }

    private void processPaga() {
        Log.d("ProcessPaga", response);
        CodaCasseActivity codaCasseActivity = (CodaCasseActivity) this.mainActivity;
        // Rimuovo il 'null' all'inizio della stringa
        response = response.substring(4);
        if(response.equals("ok") && codaCasseActivity.wantToPay){
            Log.d("Request", request);
            codaCasseActivity.wantToPay = false;
            codaCasseActivity.textViewCoda.setText("Pagamento effettutato!");
            codaCasseActivity.gotoExitActivity();
        }else{
           codaCasseActivity.textViewCoda.setText("Attendi che il cassiere prenda il tuo carrello ed elabori il tuo ordine...");
        }
    }

    void processCatalogo(){
        Log.d("Response", response);
        MainActivity mainActivity = (MainActivity) this.mainActivity;
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
        MainActivity mainActivity = (MainActivity) this.mainActivity;
        // Rimuovo il 'null' all'inizio della stringa
        response = response.substring(4);
        Log.e("Process acquisto", "Response: " + response);
        if(response.equals("ok")) {
            Log.d("Request", request);
            String[] split = request.split(":");
            mainActivity.updateTotale(Double.parseDouble(split[split.length - 1]));
        }else{
            Log.e("Sessione scaduta", response);
            Toast.makeText(mainActivity, "Sessione scaduta", Toast.LENGTH_LONG).show();
            // Torna a StartActivity
            mainActivity.finish();
        }
        Log.d("Response", response);
    }

}
