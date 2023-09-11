package bgu.spl.net.impl.stomp;

import java.util.HashMap;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import java.util.*;
import java.lang.String;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {

    private boolean shouldTerminate;
    private Connections<String> connections;
    private int ownerClientId;
    public static final String VERSION = "1.2";

    @Override
    public void start(int connectionId, Connections connections) {
        this.connections = connections;
        this.ownerClientId = connectionId;
        this.shouldTerminate = false;
    }

    @Override
    public void process(String message) {
        try{
        StompFrame recieved = new StompFrame(message);
        if(recieved.getCommand() == StompCommand.CONNECT){
           if(authenticate(recieved.getHeader(StompHeader.LOGIN), recieved.getHeader(StompHeader.PASSCODE)) && recieved.getHeader(StompHeader.ACCEPT_VERSION).equals(StompMessagingProtocolImpl.VERSION)){
              sendConnected(recieved.getHeader(StompHeader.ACCEPT_VERSION), this.ownerClientId);
           }
           else{
            Integer receipt = null;
            if(recieved.getHeader(StompHeader.RECEIPT) != null)
               receipt = Integer.parseInt(recieved.getHeader(StompHeader.RECEIPT));
            sendError("couldn't connect", receipt, "couldnt authenticate or version doesnt match or user is already logged in", ownerClientId);
            connections.disconnect(ownerClientId);
            shouldTerminate = true;
           }
        }
        else if(recieved.getCommand() == StompCommand.SUBSCRIBE){
            if(!connections.subscribe(this.ownerClientId, recieved.getHeader(StompHeader.DESTINATION), Integer.parseInt(recieved.getHeader(StompHeader.ID)))){
                sendError("couldn't subscribe", Integer.parseInt(recieved.getHeader(StompHeader.RECEIPT)), "couldnt subscribe, you are being disconnected", ownerClientId);
                connections.disconnect(ownerClientId);
                shouldTerminate = true;
            }
            if(recieved.getRequiresReciept())
              sendReciept(Integer.parseInt(recieved.getHeader(StompHeader.RECEIPT)), ownerClientId);
        }
        else if(recieved.getCommand() == StompCommand.UNSUBSCRIBE){
            if(!connections.unsubscribe(this.ownerClientId, Integer.parseInt(recieved.getHeader(StompHeader.ID)))){
                sendError("couldn't unsubscribe", Integer.parseInt(recieved.getHeader(StompHeader.RECEIPT)), "couldnt unsubscribe, you are being disconnected", ownerClientId);
                connections.disconnect(ownerClientId);
                shouldTerminate = true;
            }
            if(recieved.getRequiresReciept())
              sendReciept(Integer.parseInt(recieved.getHeader(StompHeader.RECEIPT)), ownerClientId);
        }
        else if(recieved.getCommand() == StompCommand.DISCONNECT){
            sendReciept(Integer.parseInt(recieved.getHeader(StompHeader.RECEIPT)), ownerClientId);
            connections.disconnect(ownerClientId);
            shouldTerminate = true;
        }
        else if(recieved.getCommand() == StompCommand.SEND){
            if(recieved.getHeader(StompHeader.DESTINATION) != null && connections.isSubscribed(ownerClientId,recieved.getHeader(StompHeader.DESTINATION))){
                for(Map.Entry<Integer,Integer> clientIdToSubId : connections.getSubscribed(recieved.getHeader(StompHeader.DESTINATION)).entrySet()){
                    //this.sendMessage(connections.getSubscriptionID(ownerClientId, recieved.getHeader(StompHeader.DESTINATION)), connections.generateMessageID(), recieved.getHeader(StompHeader.DESTINATION), recieved.getFrameBody());
                    this.sendMessage(clientIdToSubId.getKey(), clientIdToSubId.getValue(), connections.generateMessageID(), recieved.getHeader(StompHeader.DESTINATION), recieved.getFrameBody());
                }
            }
            else{
                sendError("couldn't send", Integer.parseInt(recieved.getHeader(StompHeader.RECEIPT)), "couldn't send, you are being disconnected", ownerClientId);
                connections.disconnect(ownerClientId);
                shouldTerminate = true;
            }
        }
        else {
            throw new Exception( " didnt recognize stomp command! ");
        }
        
        } catch (Exception e) { 
            e.printStackTrace();
            System.out.println("Exception Caught while processing: " +  e.getMessage() + " \n : ------------\n" + message +"\n------------, sending ERROR frame!");
            sendError("failed processing frame!", null, "recieved message was: \n ----------------------\n" + message, ownerClientId);
            connections.disconnect(ownerClientId);
            shouldTerminate = true;
        }
        
    }

    public void sendConnected(String version, int connectionId){
        HashMap<StompHeader, String> versionHeader = new HashMap<StompHeader, String>();
        versionHeader.put(StompHeader.VERSION, version);

        StompFrame connectedFrame = new StompFrame(StompCommand.CONNECTED, versionHeader, "", false);
        
        this.connections.send(connectionId, connectedFrame.toString());
    }

    public void sendReciept(int recieptId, int connectionId){
        HashMap<StompHeader, String> recieptHeader = new HashMap<StompHeader, String>();
        recieptHeader.put(StompHeader.RECEIPT_ID, Integer.toString(recieptId));

        StompFrame reciept = new StompFrame(StompCommand.RECEIPT, recieptHeader, "", false);
        
        this.connections.send(this.ownerClientId, reciept.toString());
    }

    public void sendMessage(int clientId ,int subscription, int messageId, String dest, String bod){
        HashMap<StompHeader, String> messageHeaders = new HashMap<StompHeader, String>();
        messageHeaders.put(StompHeader.SUBSCRIPTION, Integer.toString(subscription));
        messageHeaders.put(StompHeader.MESSAGE_ID, Integer.toString(messageId));
        messageHeaders.put(StompHeader.DESTINATION, dest);

        StompFrame messageFrame = new StompFrame(StompCommand.MESSAGE, messageHeaders, bod, false);
        
        this.connections.send(clientId, messageFrame.toString());
    }

    public void sendError(String messageHeader, Integer recieptId, String errorDetails, int connectionId ){
        HashMap<StompHeader, String> headerz = new HashMap<StompHeader, String>();
        if(recieptId != null)
           headerz.put(StompHeader.RECEIPT_ID, Integer.toString(recieptId));
        if(!messageHeader.equals(""))
           headerz.put(StompHeader.MESSAGE, messageHeader);

        StompFrame messageFrame = new StompFrame(StompCommand.ERROR, headerz, errorDetails, false);
        
        this.connections.send(connectionId, messageFrame.toString());
    }


    public boolean authenticate(String username, String pass){
        return connections.login(this.ownerClientId, username, pass);
    }


    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}