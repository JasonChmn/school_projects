/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package View;

import Controler.ChatController;
import Controler.ChatNI;
import Controler.GUIListener;

/**
 *
 * @author jason
 */
public class ChatGUI implements OutputGUI {
   GUIListener chatctrl_listener;

    public ChatGUI(GUIListener chatctrl_listener) {
        this.chatctrl_listener = chatctrl_listener;
    }

    @Override
    public void connectOK() {
        System.out.println("========= Connection faite ! ======");
    }
    
    
    
}
