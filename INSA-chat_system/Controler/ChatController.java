/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Controler;

import Model.*;
import View.*;

/**
 *
 * @author jason
 */
public class ChatController implements GUIListener {
    ChatGUI chatgui;
    ChatNI chatni;
    
    OutputGUI chatgui_output;
    
    public ChatController()
    {
        this.create_gui_ni();
    }
    
    private void create_gui_ni()
    {
        this.chatgui = new ChatGUI(this);
        this.chatni = new ChatNI(this);
        
        this.chatgui_output = (OutputGUI)chatgui;
    }
    
    @Override
    public void performConnect(String nickname)
    {
        this.chatni.hello(nickname, true);
    }
}
