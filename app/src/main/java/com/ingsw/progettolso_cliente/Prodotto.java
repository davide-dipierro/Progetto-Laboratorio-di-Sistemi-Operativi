package com.ingsw.progettolso_cliente;

public class Prodotto {
    protected int id;
    protected String nome;
    protected double prezzo;

    public Prodotto(int id, String nome, double prezzo) {
        this.id = id;
        this.nome = nome;
        this.prezzo = prezzo;
    }

    public int getId() {
        return id;
    }

    public String getNome() {
        return nome;
    }

    public double getPrezzo() {
        return prezzo;
    }

}
