package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
    

        if(args.length == 2){
            int port = Integer.parseInt(args[0]);

            if(args[1].equals("tpc")){
            Server.threadPerClient(port,
            new ConnectionsImpl<String>(), //protocol factory
            ()-> new StompMessagingProtocolImpl(),
            ()-> new LineMessageEncoderDecoder()    
            ).serve();
            }

            else if (args[1].equals("reactor")){
            Server.reactor(1, port, new ConnectionsImpl<String>(), 
            ()-> new StompMessagingProtocolImpl(),
            ()-> new LineMessageEncoderDecoder()).serve();
            }

            System.out.println("program finished");
        }
        else {
            System.out.println("No arguments found! Please enter port and tpc/reactor");
        }
            

    }
}
