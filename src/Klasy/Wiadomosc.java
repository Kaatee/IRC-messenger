/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Klasy;

import Klasy.Uzytkownik;

/**
 *KLASA KOŃCOWO NIE UŻYWANA 
 * @author Anka
 */
public class Wiadomosc {
    private String tresc;
    private Uzytkownik odbiorca;
    private Uzytkownik nadawca;

    public String getTresc() {
        return tresc;
    }

    public void setTresc(String tresc) {
        this.tresc = tresc;
    }

    public Uzytkownik getOdbiorca() {
        return odbiorca;
    }

    public void setOdbiorca(Uzytkownik odbiorca) {
        this.odbiorca = odbiorca;
    }

    public Uzytkownik getNadawca() {
        return nadawca;
    }

    public void setNadawca(Uzytkownik nadawca) {
        this.nadawca = nadawca;
    }

    public Wiadomosc(String tresc, Uzytkownik odbiorca, Uzytkownik nadawca) {
        this.tresc = tresc;
        this.odbiorca = odbiorca;
        this.nadawca = nadawca;
    }
    
}
