package bgu.spl.net.impl.stomp;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.lang.*;

public class StompFrame implements Serializable{
    private StompCommand command;
    private HashMap<StompHeader,String> headers;
    private String body;
    private boolean requiresReciept;

    private String message;

    public StompFrame(String msg) throws Exception{ // build stomp object with a string representation
        headers = new HashMap<StompHeader,String>();
        body = "";
        requiresReciept = false;
        this.message = msg;
        Iterator<java.lang.String> lines = getStringLinesIterator(msg);

        if(lines.hasNext()){ //set command
            if(!parseCommand(lines.next()))
              throw new Exception("Illegal Frame! (command)");
        }
           
        while(lines.hasNext()){ // set headers
            String line = lines.next();
            if(line.equals(""))
               break;
            else{
                if(!parseHeader(line))
                   throw new Exception("Illegal Frame! (header):" + line);
            }
        }

        while(lines.hasNext())
        {
            if(body.equals(""))
               body += lines.next();
            else
               body += "\n" + lines.next();
        }

    }

    public StompFrame(StompCommand com, HashMap<StompHeader, String> headers, String body, boolean requiresReciept){
        this.command = com;
        this.headers = headers;
        this.body = body;
        this.requiresReciept = requiresReciept;
        this.message = toString(com, headers, body);
    }

    public String toString(StompCommand com, HashMap<StompHeader, String> hdrs, String bod){
        String message = "";

        message += this.command.toString()+ "\n";

        for (StompHeader head : this.headers.keySet()) {
            message += head.getValue() + ":" + this.headers.get(head) + "\n";
        }

        if(body!=null && body.length() != 0){
            message += "\n" + body;
        }
        message+="\n" /*+'\u0000'*/;
       
        return message;
    }

    @Override
    public String toString(){
       return this.message;
    }

    private boolean parseCommand(String line){
        try{
        this.command = StompCommand.fromValue(line);
        return true;
        } catch(Exception e) { return false;}
    }

    private boolean parseHeader(String line){
        String[] strings = line.split(":");
        if(strings.length != 2 )
           return false;
        
        StompHeader head;
        try{
        head = StompHeader.fromValue(strings[0]);
        } catch(Exception e) {return false;}
        String value = strings[1];
        if(head.getType().equals("Integer")){
            try{
            Integer.parseInt(value);
            } catch (Exception e) { return false;}
        }

        if(head == StompHeader.RECEIPT)
           this.requiresReciept = true;

        this.headers.put(head, value);
        return true;
    }

    private boolean parseBody(String body){
       this.body = body;
       return true;
    }

    public boolean getRequiresReciept(){
        return this.requiresReciept;
    }
    public StompCommand getCommand(){
        return this.command;
    }
    public HashMap<StompHeader,String> getHeaders(){
        return this.headers;
    }
    public String getFrameBody(){
        return this.body;
    }
    public String getHeader(StompHeader h){
       if(this.headers.containsKey(h))
           return this.headers.get(h);
        else
           return null;
    }
    private Iterator<String> getStringLinesIterator(String m)
    {
        List<String> ans = new LinkedList<String>();
        String[] lines = m.split("\n");
        for(String line : lines)
            ans.add(line);

        return ans.iterator();
    }
}


/*if(!lines.hasNext())
           throw new Exception("Illegal Frame! (no null character)");
        String frameBody = "";
        boolean done = false;
        boolean firstLine = true;
        while(lines.hasNext()){
            String line = lines.next();
            for(int i=0; i<line.length(); i++){
                if(line.charAt(i) == '\u0000'){

                    if(lines.hasNext() || i!= line.length()-1)
                       throw new Exception("Illegal Frame! (null character is not last)");
                    else if(i!=0){
                        line = line.substring(0, i);
                    }
                    else
                       done = true;
                }
            }
            if(firstLine && !done){
               frameBody = line;
               firstLine = false;
            }
            else if(!done)
               frameBody = frameBody + "\n" + line;
        }
        this.body = frameBody;
        if(this.body.charAt(body.length()-1) != '\u0000')
           throw new Exception("Illegal Frame! (no null character at the end)");*/