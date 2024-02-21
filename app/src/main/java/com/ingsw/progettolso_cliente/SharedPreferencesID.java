package com.ingsw.progettolso_cliente;

import android.content.Context;
import android.content.SharedPreferences;

public class SharedPreferencesID {
public static final String PREF_NAME = "MyAppPrefs";
    public static final String ID_KEY = "Id";

    public SharedPreferencesID() {
    }

    public static int getId(Context context) {
        SharedPreferences sharedPreferences = context.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        return sharedPreferences.getInt(ID_KEY, -1);
    }

    public static void saveId(Context context, String id) {
        SharedPreferences sharedPreferences = context.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putInt(ID_KEY, Integer.parseInt(id));
        editor.apply();
    }

}
