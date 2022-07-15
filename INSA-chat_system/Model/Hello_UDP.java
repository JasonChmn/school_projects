/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Model;

/**
 *
 * @author jason
 */
public class Hello_UDP {
    String message = "hello";
    String nickname = "";
    Boolean reply = true;
    
    public Hello_UDP(String nickname, Boolean reply)
    {
        this.nickname = nickname;
    }
}
