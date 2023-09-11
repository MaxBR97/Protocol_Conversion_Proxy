package bgu.spl.net.impl.stomp;

import java.util.EnumMap;

public enum StompHeader {

    ACCEPT_VERSION("accept-version", "String"),
   // CONTENT_LENGTH("content-length"),
   // CONTENT_TYPE("content-type"),
    DESTINATION("destination", "String"),
    HOST("host", "String"),
    ID("id", "Integer"),
    MESSAGE_ID("message-id", "Integer"),
    RECEIPT_ID("receipt-id", "Integer"),
    RECEIPT("receipt", "Integer"),
    SUBSCRIPTION("subscription", "Integer"),
    VERSION("version", "String"),
    MESSAGE("message", "String"),
    LOGIN("login", "String"),
    PASSCODE("passcode", "String");


    private final String value;
    private String valueType;

    public static StompHeader fromValue(String value) throws Exception {
        for (StompHeader c : StompHeader.values()) {
            if (c.value.equals(value)) {
                return c;
            }
        }
        throw new Exception("no such header: " + value);
    }

    private StompHeader(String value, String type) {
        this.value = value;
        try{
        this.valueType = type;
        } catch (Exception ignore){}
    }

    public String getValue() {
        return this.value;
    }
    public String getType(){
        return this.valueType;
    }

}
