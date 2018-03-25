/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Klasy;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

/**
 * Klasa odpowiedzialna za nawiązywanie połączenia
 * @author Anka
 */

public class Polaczenie {
    private Socket socket;
/**
 * Funkcja odpowiedzialna za tworzenie gniazda - socket
 * @param ip
 * @param port 
 */
    public Polaczenie(InetAddress ip, int port) {
        try {
            this.socket = new Socket(ip, port);
        } catch (IOException e) {
         System.out.println("Nie udalo sie utworzyc gniazda");
        }
    }  
    /**
     * Funkcja odpowiedzialna za wysyłanie wiadomosci
     * @param message 
     */
    public void println(String message) {
        //PrintWriter writer;
        try {
           // writer = new PrintWriter(new OutputStreamWriter( socket.getOutputStream()), true);
          //  writer.println(message);
          //writer.print(message);  
          //writer.flush();
            DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());

            byte[] bytes = message.getBytes();
            dataOutputStream.write(bytes);
            dataOutputStream.flush();
       
        } catch (IOException e){ 
            System.out.println("Nie udało sie wyslac");
        }
    }
/**
 * Funkcja odpowiedzialna za odbieranie wiadomosci
 * @return String wiadomosc od serwera
 */
    public String readLine() {
        BufferedReader reader;
        try {
            reader = new BufferedReader(new InputStreamReader(
                                        socket.getInputStream()));
            
            return reader.readLine();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            System.out.print("Nie udalo sie odberac");
            return "";
        }
    }     
    /**
     * Funkcja zamykajaca gniazdo
     */
    public void close() {
        try {
            if (socket != null && !socket.isClosed())
                socket.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
           // e.printStackTrace();
        }
    }
    public Socket getSocket() {
        return socket;
    }

    public void setSocket(Socket socket) {
        this.socket = socket;
    }
    
}
