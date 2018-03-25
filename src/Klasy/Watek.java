/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Klasy;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;

/**
 *Klasa odpowiedzialna za odbieranie danych od serwera. W tle działa wątek tworzony w 
 * klasie FXMLDocumentControler.
 * @author Anka
 */
public class Watek extends Thread {
//pola klasy
    private Polaczenie polaczenie;
    private Thread watek;
    @FXML
    private Label informacje;
    @FXML
    private ListView listaUzytkownikow;
    @FXML
    private ListView listaPokoi;
    @FXML
    private TextField nazwaPokoju;
    @FXML
    private Button dodajDoPokoju;
    @FXML
    private Button zatwierdz;
    @FXML
    private Label pokoje;
    @FXML
    private Label uzytkownicy;
    @FXML
    private Label obecnaGrupa;
    @FXML
    private TextArea oknoWiadomosci;
    @FXML
    private TextArea trescWiadomosci;
//konstruktor klasy
    public Watek(Polaczenie polaczenie, Label informacje, ListView listaUzyt, ListView listaPokoi, TextField nazwaPokoju, Button dodajDoPokoju, Button zatwierdz, Label pokoje, Label uzytkownicy, Label obecnaGrupa, TextArea oknoWiadomosci, TextArea trescWiadomosci) {
        this.polaczenie = polaczenie;
        this.informacje = informacje;
        this.listaUzytkownikow = listaUzyt;
        this.listaPokoi = listaPokoi;
        this.dodajDoPokoju = dodajDoPokoju;
        this.nazwaPokoju = nazwaPokoju;
        this.zatwierdz = zatwierdz;
        this.pokoje = pokoje;
        this.uzytkownicy = uzytkownicy;
        this.obecnaGrupa = obecnaGrupa;
        this.oknoWiadomosci = oknoWiadomosci;
        this.trescWiadomosci = trescWiadomosci;
    }
//metoda zawierająca kod wykonywany przez wątek
    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            try {
                System.out.println("Hello from thread");
                String odp = "";
                String wiadomosc = this.polaczenie.readLine();
                System.out.println("To jest wiadomosc: " + wiadomosc);
                String[] wybor = wiadomosc.split("\t");
                String x = wybor[0];
                int y = Integer.parseInt(x);
                for (int i = 0; i < wybor.length; i++) {
                    System.out.println("To jest wybor :" + i + " " + wybor[i].toString());
                }
                System.out.println("To jest wybor switch: " + y);
                switch (y) {
                    case 2:
                        //odbieranie listy użytkowników od klienta
                        ArrayList<String> uzytkownicy = new ArrayList<>(Arrays.asList(wybor));
                        uzytkownicy.remove(0);
                        Platform.runLater(() -> {
                            this.listaUzytkownikow.setItems(FXCollections.observableArrayList(uzytkownicy));
                        });
                        break;
                    case 3:
                        //odbieranie listy pokoi od klienta
                        ArrayList<String> pokoje = new ArrayList<>(Arrays.asList(wybor));
                        pokoje.remove(0);
                        Platform.runLater(() -> {
                            this.listaPokoi.setItems(FXCollections.observableArrayList(pokoje));
                        });
                        break;
                    case 4:
                        //odbieranie listy użytkowników w danym pokoju
                        ArrayList<String> ludziePokoj = new ArrayList<>(Arrays.asList(wybor));
                        ludziePokoj.remove(0);
                        Platform.runLater(() -> {
                            this.listaUzytkownikow.setItems(FXCollections.observableArrayList(ludziePokoj));
                        });
                        break;
                    case 5:
                      //odbieranie informacji czy udalo sie/nie udalo sie utwory=zyc nowego pokoju
                        x = wybor[1];
                        if (x.equals("1")) {
                          
                            Platform.runLater(() -> {
                                this.informacje.setText("Utworzono nowy pokoj");
                                this.nazwaPokoju.setVisible(false);
                                this.dodajDoPokoju.setVisible(false);
                                this.zatwierdz.setVisible(false);
                                this.pokoje.setText("Pokoje");
                                this.uzytkownicy.setText("Uzytkownicy");
                                this.listaPokoi.getItems().removeAll();
                                this.listaUzytkownikow.getItems().removeAll();
                            });
                        }
                        x = wybor[1];
                        if (!x.equals("1")) {
                            Platform.runLater(() -> {
                                this.informacje.setText("Nie udało się utworzyc nowego pokoju");
                            });
                        }

                        break;

                    case 6:
                        //odbieranie wiadomosci/ tresc rozmowy w pokoju
                        x = wybor[1];
                        //String x2 = wybor[2];
                        if (x.equals("1")) {
                            System.out.println("Ja wyslalem");
                            Platform.runLater(() -> {
                                this.oknoWiadomosci.appendText("Ja: " + this.trescWiadomosci.getText() + "\n");
                                this.informacje.setText("Udało się wysłać");
                            });

                        } else {
                            System.out.println("Ja odbieram");
                          //  if ((this.obecnaGrupa.getText() + ".txt").equals(x2)) {
                                System.out.println("To jest wybor" + wybor.toString());
                                String nick = wybor[1];
                                String tresc = wybor[3];
                                String odebranaWiadomosc = nick + " : " + tresc;
                                Platform.runLater(() -> {
                                    this.oknoWiadomosci.appendText(odebranaWiadomosc + "\n");
                                });
                            //}
                        }
                        break;
                    case 8:
                        //odbieranie informacji czy udalo sie/nie udalo sie usunac pokoj
                        x = wybor[1];
                        if (x.equals("1")) {
                            Platform.runLater(() -> {
                                //this.listaPokoi.getItems().removeAll();
                                //this.listaUzytkownikow.getItems().removeAll();
                                this.informacje.setText("Pokój został usunięty");
                            });

                        } else if (x.equals("2")) {
                            Platform.runLater(() -> {
                                //this.listaPokoi.getItems().removeAll();
                                //this.listaUzytkownikow.getItems().removeAll();
                                this.informacje.setText("Nie jesteś adminem");
                            });
                        }

                        break;
                    default:
                        Platform.runLater(() -> {
                            this.informacje.setText("Nie rozpoznano");
                        });
                        break;

                }

                Thread.sleep(3000);
            } catch (InterruptedException ex) {
            } catch (Exception e) {
                watek.interrupt();
                watek.stop();
                System.out.println("Coś poszło nie tak watek sie zakonczyl");
            }
        }
    }

    public Thread getWatek() {
        return watek;
    }

    public void setWatek(Thread watek) {
        this.watek = watek;
    }

    public Watek(Polaczenie polaczenie) {
        this.polaczenie = polaczenie;
    }

    public Polaczenie getPolaczenie() {
        return polaczenie;
    }

    public void setPolaczenie(Polaczenie polaczenie) {
        this.polaczenie = polaczenie;

    }
}
