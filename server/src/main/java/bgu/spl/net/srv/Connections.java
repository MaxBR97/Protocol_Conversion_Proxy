package bgu.spl.net.srv;

import java.io.IOException;
import java.util.HashMap;

import bgu.spl.net.api.StompMessagingProtocol;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);
    
    public void connect(ConnectionHandler<T> handler, StompMessagingProtocol stomp);

    boolean subscribe(int connectionId, String channel, int subscriptionId);

    boolean unsubscribe(int connectionId, int subscriptionId);

    int getSubscriptionID(int connectionId, String channel);

    boolean isSubscribed(int ownerClientId, String header);

    int generateMessageID();

    boolean login(int ownerClientId, String username, String pass);

    HashMap<Integer, Integer> getSubscribed(String header);

}
