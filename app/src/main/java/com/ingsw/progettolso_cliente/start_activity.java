package com.ingsw.progettolso_cliente;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class start_activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start);
    }

    //enter_market è il metodo che viene chiamato quando si clicca sul bottone "Entra nel mercato"
    public void enter_market(View view) {
        Intent intent = new Intent(this, MainActivity.class);
        ClientTask clientTask = new ClientTask(null, "cliente:-1:entra");
        clientTask.execute();
        while(clientTask.response == null);
        intent.putExtra("id", clientTask.response.split(":")[1]); //clientTask.response è una stringa del tipo "id:<id_cliente
        startActivity(intent);
    }

}