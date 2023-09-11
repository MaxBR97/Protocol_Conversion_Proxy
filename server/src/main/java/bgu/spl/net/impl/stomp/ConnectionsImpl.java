package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class ConnectionsImpl<T> implements Connections<T>{

    private HashMap < String, String> userNameToPass;
    private HashMap< Integer, ConnectionHandler<T> > clientIdToConnection;
    private HashMap< Integer, String> clientIdToUsername;
    private HashMap< Integer, HashMap<Integer,String> > clientIdToSubToChannel;
    private static int messageID;
    private static int clientID;


    public ConnectionsImpl (){
        userNameToPass = new HashMap<String, String>();
        clientIdToConnection = new HashMap < Integer, ConnectionHandler<T> >();
        clientIdToUsername = new HashMap <Integer, String>();
        clientIdToSubToChannel = new HashMap <Integer, HashMap<Integer,String>>();
        ConnectionsImpl.messageID = 1;
        ConnectionsImpl.clientID = 1;
    }
    @Override
    public void disconnect(int connectionId) {
        synchronized(clientIdToConnection){
        synchronized(clientIdToUsername){
        unsubscribe(connectionId);
        clientIdToUsername.remove(connectionId);
        try{
        //clientIdToConnection.get(connectionId).close(); // might need to be removed
        }catch(Exception e){ e.printStackTrace();}
        clientIdToConnection.remove((Integer)connectionId);
    }}
    }

    @Override
    public boolean send(int connectionId, T msg) {
        synchronized(clientIdToConnection){
        try{
          if(clientIdToConnection.containsKey(connectionId)){
           clientIdToConnection.get(connectionId).send(msg);
           return true;
          }
        return false;
        } catch(Exception e){ e.printStackTrace(); return false;}
    }
    }

    @Override
    public void send(String channel, T msg) {
        synchronized(clientIdToConnection){
            synchronized(clientIdToUsername){
                synchronized(clientIdToSubToChannel){
        for(Integer clientId : clientIdToSubToChannel.keySet()){
            for( String ch : clientIdToSubToChannel.get(clientId).values()){
                if(ch.hashCode() == channel.hashCode()){
                    if(clientIdToConnection.containsKey(clientId) && clientIdToUsername.containsKey(clientId) && clientIdToUsername.get(clientId)!=null){
                       try{
                       clientIdToConnection.get(clientId).send(msg);
                       }catch( Exception e) { e.printStackTrace();}
                   }
                }
            }
        }
     }}}
    }

    public boolean subscribe(int connectionId, String channel, int subscriptionId){
        synchronized (clientIdToUsername){
            synchronized(clientIdToSubToChannel){
        if(!clientIdToUsername.containsKey(connectionId))
           return false; // client not logged in
        if(clientIdToSubToChannel.get(connectionId).containsKey(subscriptionId))
           return false; // already subscribed to that subscriptionID
        clientIdToSubToChannel.get(connectionId).put(subscriptionId,channel);
            }
        }
        return true;
    }

    public boolean unsubscribe(int connectionId, int subscriptionId){
        synchronized (clientIdToUsername){
            synchronized(clientIdToSubToChannel){
        if(!clientIdToUsername.containsKey(connectionId))
           return false; // client not logged in
        clientIdToSubToChannel.get(connectionId).remove(subscriptionId);
            }
        }
        return true;
    }

    public boolean unsubscribe(int connectionId){ // removes all subscriptions
        synchronized (clientIdToUsername){
            synchronized(clientIdToSubToChannel){
                
        if(!clientIdToUsername.containsKey(connectionId))
           return false; // client not logged in
        synchronized(clientIdToSubToChannel.get(connectionId).keySet()){
        List<Integer> subsToDelete = new LinkedList<Integer>();
        for ( int subscription : clientIdToSubToChannel.get(connectionId).keySet())
        {
            subsToDelete.add(subscription);
            
        }
        for(Integer sub: subsToDelete)
        {
            unsubscribe(connectionId, sub);
        }
    }
            }
        }
        return true;
    }

    public void connect(ConnectionHandler <T> handler, StompMessagingProtocol stomp){
        synchronized(clientIdToConnection){
            synchronized(clientIdToSubToChannel){
        int id = generateClientID();
        stomp.start(id, this);
        clientIdToConnection.put(id, handler);
        if(!clientIdToSubToChannel.containsKey(id))
           clientIdToSubToChannel.put(id, new HashMap<Integer,String>());
           }
        }
    }
    public boolean isSubscribed(int connectionId, String channel) {
        synchronized(clientIdToSubToChannel){
            synchronized(clientIdToSubToChannel.get(connectionId).keySet()){
        for(String ch : clientIdToSubToChannel.get(connectionId).values()){
            if(ch.equals(channel))
               return true;
        }
        }
        }
        return false;
    }
    public int getSubscriptionID(int connectionId, String channel) {
        synchronized(clientIdToSubToChannel){
            synchronized(clientIdToSubToChannel.get(connectionId).keySet()){
        for(Integer subId: clientIdToSubToChannel.get(connectionId).keySet()){
            if(clientIdToSubToChannel.get(connectionId).get(subId).equals(channel))
               return subId;
        }
    }
    }
        return -1;
    }
    public boolean login(int connectionId, String username, String password){
        synchronized(userNameToPass){
            synchronized(clientIdToUsername){
        for(String user : userNameToPass.keySet()){
            if(user.hashCode() == username.hashCode()){
               if(userNameToPass.get(user).equals(password)){
                  if(!isUserLoggedIn(user)){
                     clientIdToUsername.put(connectionId,username);
                     return true;
                     }
                   else
                      return false; //RIGHT USERNAME & PASS, BUT USER LOGGED IN
               }
               else{
                return false; //RIGHT USERNAME, WRONG PASSWORD
               }
            }

        }
        userNameToPass.put(username,password); // NO SUCH USER, USER CREATED
        clientIdToUsername.put(connectionId,username);
        return true;
    }
   }
    }
    private boolean isUserLoggedIn(String username){
        synchronized(clientIdToUsername){
       for(String user : clientIdToUsername.values()){
           if(user.hashCode() == username.hashCode())
              return true;
       }
       return false;
    }
    }
    public int generateMessageID() {
        synchronized(this){
        messageID++;
        return messageID;
        }
    }
    private int generateClientID() {
        synchronized(this){
        clientID++;
        return clientID;
        }
    }
	@Override
	public HashMap<Integer,Integer> getSubscribed(String channel) { // hashmap is <connectionId, subId>\
        
        HashMap<Integer,Integer> ans = new HashMap<Integer,Integer>();
        synchronized(clientIdToSubToChannel){
            synchronized(clientIdToSubToChannel.keySet()){
		    for(Integer clientId : clientIdToSubToChannel.keySet())
            {
                int id = getSubscriptionID(clientId, channel);
                if(id != -1)
                {
                   ans.put(clientId,id);
                }
            }
        }

        }
        return ans;
	}
    
}