/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Controler;

import Model.Hello_UDP;

/**
 *
 * @author jason
 */
public class ChatNI {
    ChatController chatctrl;

    public ChatNI(ChatController chatctrl) {
        this.chatctrl = chatctrl;
        
    }
    
    // Envoyer hello en broadcast
    public void hello(String nickname, Boolean reply)
    {
        Hello_UDP my_hello = new Hello_UDP(nickname, reply);
        
    }
    
    
    private send_hello(Hello_UDP)
    {
        
    }
    
    
}
