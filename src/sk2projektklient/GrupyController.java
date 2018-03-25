/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

import Klasy.Polaczenie;
import java.io.File;
import java.io.FileNotFoundException;
import java.net.InetAddress;
import java.net.URL;
import java.util.Arrays;
import java.util.List;
import java.util.ResourceBundle;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;

/**
 * FXML Controller class
 * KLASA KONCOWO NIE UZYWANA
 * @author Anka
 */
public class GrupyController implements Initializable {
     @FXML
    private TextField nickAdministratora;
    @FXML
    private Button dodajDoPokoju;
    @FXML
    private ListView uzytkownicyNalezacy;
    @FXML
    private Button zatwierdz;
    @FXML
    private TextField nazwaPokoju;
    @FXML
    private ListView uzytkownicy;
    @FXML
    private TableView<?> tabelaUzyt;
    @FXML
    void handleDodaj(ActionEvent event) {
        uzytkownicyNalezacy.getItems().add(uzytkownicy.getSelectionModel().getSelectedItem());
        uzytkownicy.getItems().remove(uzytkownicy.getSelectionModel().getSelectedItem());   
    }
    @FXML 
    void handleZatwierdz (ActionEvent event){
         ObservableList lista=uzytkownicyNalezacy.getItems();
         String listaUzyt;
         listaUzyt=lista.toString();
         String replace = listaUzyt.replace("[","");
         replace = replace.replace("]","");
         replace = replace.replace(",","");
         replace = replace.replace(" ", "\t");
         replace=replace+"\n";
        System.out.println(replace);
         
        Polaczenie polaczenie = null;
     try {
                polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                polaczenie.println("5\n");
                String odbior=polaczenie.readLine();
                if (odbior.equals("11")) {
                            polaczenie.println("\""+this.nazwaPokoju.getText()+".txt\""+"\n");
                            odbior=polaczenie.readLine();
                                if(odbior.equals("13")){
                                    polaczenie.println(this.nickAdministratora.getText()+"\n");
                                    odbior=polaczenie.readLine();
                                        if(odbior.equals("14")){
                                            polaczenie.println(replace);
                                            odbior=polaczenie.readLine();
                                                if(odbior.equals("15")){
                                                    System.out.println("Udało sie utworzyc pokoj");
                                                }else { System.out.println("Nie udało się utworzyć pokoju");} 
                                        }
                                }
                        }
              polaczenie.close();
                }
         catch (Exception ex) {
             polaczenie.close();
        }
    }
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        Polaczenie polaczenie = null;
        try {
            System.out.println("wlazlem");
            
            File file = new File("C:\\Users\\Anka\\Desktop\\nick.txt");
            Scanner in;
            in = new Scanner(file);  
            String nick = in.nextLine();
            nickAdministratora.setText(nick);  
        } catch (FileNotFoundException ex) {
            System.out.println("Nie znaleziono pliku");
        }
      
        try {
                polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                polaczenie.println("2\n");
                String odbior=polaczenie.readLine();
                if (odbior.equals("11")) {
                    String uzyt = "";
                        uzyt=polaczenie.readLine();
                        String uzyt2=  uzyt.replace(" ","");
                        String [] uzyt3=uzyt2.split("\t");
                        List<String> uzyt4=Arrays.asList(uzyt3);
                        uzytkownicy.setItems(FXCollections.observableArrayList(uzyt4));
                        }
              polaczenie.close();
                }
         catch (Exception ex) {
             polaczenie.close();
        }  
    }       
}
