package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.*;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Connections<T> connections;
    private final Supplier<StompMessagingProtocol> stompProtocolFactory;
    private final Supplier<MessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket sock;

    public BaseServer(
            int port,
            Connections<T> connections,
            Supplier<StompMessagingProtocol> stompProtocolFactory,
            Supplier<MessageEncoderDecoder<T>> encdecFactory
            ) {

        this.port = port;
        this.connections = connections;
        this.stompProtocolFactory = stompProtocolFactory;
        this.encdecFactory = encdecFactory;
		this.sock = null;
    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
            
			System.out.println("BaseServer started listening at address: " + serverSock.getInetAddress() + " and at port: " + serverSock.getLocalPort());

            this.sock = serverSock; //just to be able to close

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();
                System.out.println("Client accepted: " + clientSock.getInetAddress() + " port: " + clientSock.getPort());
                StompMessagingProtocol stomp = stompProtocolFactory.get();

                BlockingConnectionHandler<T> handler = new BlockingConnectionHandler<>(
                        clientSock,
                        encdecFactory.get(),
                        stomp);
                
                connections.connect(handler, stomp);
                execute(handler);
            }
        } catch (IOException ex) { ex.printStackTrace();
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

}
