/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

import Klasy.Funkcje;
import Klasy.Polaczenie;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

/**
 * FXML Controller class
 *KLASA KONCOOW NIE UZYWANA
 * @author Anka
 */
public class LogowanieController implements Initializable {
    @FXML
    private Button zalogujSie2;
    @FXML
    private TextField nickDoLogowania;
    @FXML
    private Label info;
    private Funkcje funkcje;
    
    @FXML
    void handleZalogujSie2(ActionEvent event) throws InterruptedException {
        
        Polaczenie polaczenie=null;
              // InetAddress addr = null;//=InetAddress.getByAddress(new byte[]{(byte)192), (byte)168,(byte)0,(byte)15);
        try {
             System.out.println("polaczono");
            //Polaczenie polaczenie;
                   polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                   System.out.println("polaczono");
            polaczenie.println("1\n");
            String czyOdbior= polaczenie.readLine();
            System.out.println(czyOdbior);
            if (czyOdbior.equals("11")) {
                System.out.println("czkema");
                polaczenie.println("\""+nickDoLogowania.getText()+"\""+"\n");
                //polaczenie.println("nick\n");
                String odbior=polaczenie.readLine();
                System.out.println( odbior);
                if(odbior.equals("100")){
                    info.setText("Nie udało się zalogować. Taki nick juz istnieje");
                }else if(odbior.equals("101")) {
                    info.setText("Udało się zalogować poprawnie");
                    
                }
                }
            polaczenie.close();
        } catch (Exception ex) {
            polaczenie.close();
            //Logger.getLogger(Sk2ProjektKlient.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
        //Polaczenie polaczenie;
    }    
    
}
