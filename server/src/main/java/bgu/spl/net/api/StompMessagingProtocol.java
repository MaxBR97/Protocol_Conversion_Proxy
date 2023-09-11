package bgu.spl.net.api;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.*;

public interface StompMessagingProtocol  {
	/**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
    public void start(int connectionId, Connections connections);
    
    public void process(String nextMessage);
	
	/**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
}
