package bgu.spl.net.impl.stomp;

public enum StompCommand {
    
    CONNECT("CONNECT"),
    CONNECTED("CONNECTED"),
    DISCONNECT("DISCONNECT"),
    ERROR("ERROR"),
    MESSAGE("MESSAGE"),
    RECEIPT("RECEIPT"),
    SEND("SEND"),
    SUBSCRIBE("SUBSCRIBE"),
    UNSUBSCRIBE("UNSUBSCRIBE");

    private final String value;

    public static StompCommand fromValue(String value) throws Exception {
        for (StompCommand c : StompCommand.values()) {
            if (c.value.equals(value)) {
                return c;
            }
        }
        throw new Exception("No such command: " + value);
    }

    private StompCommand(String value) {
        this.value = value;
    }

    public String getValue() {
        return this.value;
    }

}
