# Details

Date : 2023-09-13 11:11:36

Directory /home/spl211/Downloads/SPL_hw3

Total : 74 files,  20848 codes, 4536 comments, 3511 blanks, all 28895 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [README.md](/README.md) | Markdown | 2 | 0 | 1 | 3 |
| [client/data/events1.json](/client/data/events1.json) | JSON | 96 | 0 | 2 | 98 |
| [client/data/events1_partial.json](/client/data/events1_partial.json) | JSON | 29 | 0 | 0 | 29 |
| [client/data/events2.json](/client/data/events2.json) | JSON | 96 | 0 | 2 | 98 |
| [client/data/events3.json](/client/data/events3.json) | JSON | 0 | 0 | 1 | 1 |
| [client/include/ConnectionHandler.h](/client/include/ConnectionHandler.h) | C++ | 27 | 14 | 19 | 60 |
| [client/include/InputProtocol.h](/client/include/InputProtocol.h) | C++ | 21 | 0 | 8 | 29 |
| [client/include/StompFrame.h](/client/include/StompFrame.h) | C++ | 28 | 1 | 5 | 34 |
| [client/include/StompProtocol.h](/client/include/StompProtocol.h) | C++ | 44 | 1 | 4 | 49 |
| [client/include/event.h](/client/include/event.h) | C++ | 40 | 10 | 8 | 58 |
| [client/include/json.hpp](/client/include/json.hpp) | C++ | 17,345 | 4,321 | 2,975 | 24,641 |
| [client/makefile](/client/makefile) | Makefile | 20 | 0 | 11 | 31 |
| [client/src/ConnectionHandler.cpp](/client/src/ConnectionHandler.cpp) | C++ | 110 | 5 | 18 | 133 |
| [client/src/InputProtocol.cpp](/client/src/InputProtocol.cpp) | C++ | 286 | 1 | 18 | 305 |
| [client/src/StompClient.cpp](/client/src/StompClient.cpp) | C++ | 50 | 15 | 9 | 74 |
| [client/src/StompFrame.cpp](/client/src/StompFrame.cpp) | C++ | 179 | 0 | 17 | 196 |
| [client/src/StompProtocol.cpp](/client/src/StompProtocol.cpp) | C++ | 169 | 11 | 24 | 204 |
| [client/src/event.cpp](/client/src/event.cpp) | C++ | 271 | 3 | 34 | 308 |
| [client/trash/echoClient.cpp](/client/trash/echoClient.cpp) | C++ | 40 | 13 | 7 | 60 |
| [server/pom.xml](/server/pom.xml) | XML | 15 | 0 | 0 | 15 |
| [server/server.iml](/server/server.iml) | XML | 15 | 0 | 0 | 15 |
| [server/src/main/java/bgu/spl/net/api/MessageEncoderDecoder.java](/server/src/main/java/bgu/spl/net/api/MessageEncoderDecoder.java) | Java | 5 | 13 | 5 | 23 |
| [server/src/main/java/bgu/spl/net/api/MessagingProtocol.java](/server/src/main/java/bgu/spl/net/api/MessagingProtocol.java) | Java | 5 | 8 | 4 | 17 |
| [server/src/main/java/bgu/spl/net/api/StompMessagingProtocol.java](/server/src/main/java/bgu/spl/net/api/StompMessagingProtocol.java) | Java | 9 | 6 | 5 | 20 |
| [server/src/main/java/bgu/spl/net/impl/rci/Command.java](/server/src/main/java/bgu/spl/net/impl/rci/Command.java) | Java | 5 | 0 | 4 | 9 |
| [server/src/main/java/bgu/spl/net/impl/rci/ObjectEncoderDecoder.java](/server/src/main/java/bgu/spl/net/impl/rci/ObjectEncoderDecoder.java) | Java | 63 | 2 | 15 | 80 |
| [server/src/main/java/bgu/spl/net/impl/rci/RCIClient.java](/server/src/main/java/bgu/spl/net/impl/rci/RCIClient.java) | Java | 39 | 0 | 10 | 49 |
| [server/src/main/java/bgu/spl/net/impl/rci/RemoteCommandInvocationProtocol.java](/server/src/main/java/bgu/spl/net/impl/rci/RemoteCommandInvocationProtocol.java) | Java | 17 | 0 | 8 | 25 |
| [server/src/main/java/bgu/spl/net/impl/stomp/ConnectionsImpl.java](/server/src/main/java/bgu/spl/net/impl/stomp/ConnectionsImpl.java) | Java | 201 | 1 | 18 | 220 |
| [server/src/main/java/bgu/spl/net/impl/stomp/LineMessageEncoderDecoder.java](/server/src/main/java/bgu/spl/net/impl/stomp/LineMessageEncoderDecoder.java) | Java | 31 | 4 | 10 | 45 |
| [server/src/main/java/bgu/spl/net/impl/stomp/StompCommand.java](/server/src/main/java/bgu/spl/net/impl/stomp/StompCommand.java) | Java | 27 | 0 | 8 | 35 |
| [server/src/main/java/bgu/spl/net/impl/stomp/StompFrame.java](/server/src/main/java/bgu/spl/net/impl/stomp/StompFrame.java) | Java | 119 | 29 | 25 | 173 |
| [server/src/main/java/bgu/spl/net/impl/stomp/StompHeader.java](/server/src/main/java/bgu/spl/net/impl/stomp/StompHeader.java) | Java | 38 | 2 | 10 | 50 |
| [server/src/main/java/bgu/spl/net/impl/stomp/StompMessagingProtocolImpl.java](/server/src/main/java/bgu/spl/net/impl/stomp/StompMessagingProtocolImpl.java) | Java | 117 | 1 | 24 | 142 |
| [server/src/main/java/bgu/spl/net/impl/stomp/StompServer.java](/server/src/main/java/bgu/spl/net/impl/stomp/StompServer.java) | Java | 28 | 0 | 11 | 39 |
| [server/src/main/java/bgu/spl/net/srv/ActorThreadPool.java](/server/src/main/java/bgu/spl/net/srv/ActorThreadPool.java) | Java | 66 | 0 | 13 | 79 |
| [server/src/main/java/bgu/spl/net/srv/BaseServer.java](/server/src/main/java/bgu/spl/net/srv/BaseServer.java) | Java | 54 | 0 | 19 | 73 |
| [server/src/main/java/bgu/spl/net/srv/BlockingConnectionHandler.java](/server/src/main/java/bgu/spl/net/srv/BlockingConnectionHandler.java) | Java | 50 | 0 | 13 | 63 |
| [server/src/main/java/bgu/spl/net/srv/ConnectionHandler.java](/server/src/main/java/bgu/spl/net/srv/ConnectionHandler.java) | Java | 6 | 13 | 6 | 25 |
| [server/src/main/java/bgu/spl/net/srv/Connections.java](/server/src/main/java/bgu/spl/net/srv/Connections.java) | Java | 17 | 0 | 16 | 33 |
| [server/src/main/java/bgu/spl/net/srv/NonBlockingConnectionHandler.java](/server/src/main/java/bgu/spl/net/srv/NonBlockingConnectionHandler.java) | Java | 109 | 4 | 19 | 132 |
| [server/src/main/java/bgu/spl/net/srv/Reactor.java](/server/src/main/java/bgu/spl/net/srv/Reactor.java) | Java | 118 | 2 | 31 | 151 |
| [server/src/main/java/bgu/spl/net/srv/Server.java](/server/src/main/java/bgu/spl/net/srv/Server.java) | Java | 30 | 37 | 10 | 77 |
| [server/target/classes/bgu/spl/net/api/MessageEncoderDecoder.class](/server/target/classes/bgu/spl/net/api/MessageEncoderDecoder.class) | Java | 4 | 0 | 0 | 4 |
| [server/target/classes/bgu/spl/net/api/MessagingProtocol.class](/server/target/classes/bgu/spl/net/api/MessagingProtocol.class) | Java | 3 | 0 | 1 | 4 |
| [server/target/classes/bgu/spl/net/api/StompMessagingProtocol.class](/server/target/classes/bgu/spl/net/api/StompMessagingProtocol.class) | Java | 4 | 0 | 0 | 4 |
| [server/target/classes/bgu/spl/net/impl/rci/Command.class](/server/target/classes/bgu/spl/net/impl/rci/Command.class) | Java | 3 | 0 | 1 | 4 |
| [server/target/classes/bgu/spl/net/impl/rci/ObjectEncoderDecoder.class](/server/target/classes/bgu/spl/net/impl/rci/ObjectEncoderDecoder.class) | Java | 40 | 0 | 0 | 40 |
| [server/target/classes/bgu/spl/net/impl/rci/RCIClient.class](/server/target/classes/bgu/spl/net/impl/rci/RCIClient.class) | Java | 27 | 0 | 0 | 27 |
| [server/target/classes/bgu/spl/net/impl/rci/RemoteCommandInvocationProtocol.class](/server/target/classes/bgu/spl/net/impl/rci/RemoteCommandInvocationProtocol.class) | Java | 13 | 0 | 0 | 13 |
| [server/target/classes/bgu/spl/net/impl/stomp/ConnectionsImpl.class](/server/target/classes/bgu/spl/net/impl/stomp/ConnectionsImpl.class) | Java | 44 | 0 | 0 | 44 |
| [server/target/classes/bgu/spl/net/impl/stomp/LineMessageEncoderDecoder.class](/server/target/classes/bgu/spl/net/impl/stomp/LineMessageEncoderDecoder.class) | Java | 23 | 3 | 0 | 26 |
| [server/target/classes/bgu/spl/net/impl/stomp/StompCommand.class](/server/target/classes/bgu/spl/net/impl/stomp/StompCommand.class) | Java | 27 | 0 | 0 | 27 |
| [server/target/classes/bgu/spl/net/impl/stomp/StompFrame.class](/server/target/classes/bgu/spl/net/impl/stomp/StompFrame.class) | Java | 50 | 0 | 0 | 50 |
| [server/target/classes/bgu/spl/net/impl/stomp/StompHeader.class](/server/target/classes/bgu/spl/net/impl/stomp/StompHeader.class) | Java | 31 | 0 | 0 | 31 |
| [server/target/classes/bgu/spl/net/impl/stomp/StompMessagingProtocolImpl.class](/server/target/classes/bgu/spl/net/impl/stomp/StompMessagingProtocolImpl.class) | Java | 70 | 0 | 0 | 70 |
| [server/target/classes/bgu/spl/net/impl/stomp/StompServer.class](/server/target/classes/bgu/spl/net/impl/stomp/StompServer.class) | Java | 31 | 0 | 0 | 31 |
| [server/target/classes/bgu/spl/net/srv/ActorThreadPool.class](/server/target/classes/bgu/spl/net/srv/ActorThreadPool.class) | Java | 30 | 0 | 0 | 30 |
| [server/target/classes/bgu/spl/net/srv/BaseServer.class](/server/target/classes/bgu/spl/net/srv/BaseServer.class) | Java | 39 | 0 | 0 | 39 |
| [server/target/classes/bgu/spl/net/srv/BlockingConnectionHandler.class](/server/target/classes/bgu/spl/net/srv/BlockingConnectionHandler.class) | Java | 28 | 0 | 0 | 28 |
| [server/target/classes/bgu/spl/net/srv/ConnectionHandler.class](/server/target/classes/bgu/spl/net/srv/ConnectionHandler.class) | Java | 5 | 0 | 0 | 5 |
| [server/target/classes/bgu/spl/net/srv/Connections.class](/server/target/classes/bgu/spl/net/srv/Connections.class) | Java | 6 | 0 | 0 | 6 |
| [server/target/classes/bgu/spl/net/srv/NonBlockingConnectionHandler.class](/server/target/classes/bgu/spl/net/srv/NonBlockingConnectionHandler.class) | Java | 49 | 0 | 0 | 49 |
| [server/target/classes/bgu/spl/net/srv/Reactor.class](/server/target/classes/bgu/spl/net/srv/Reactor.class) | Java | 89 | 0 | 0 | 89 |
| [server/target/classes/bgu/spl/net/srv/Server$1.class](/server/target/classes/bgu/spl/net/srv/Server$1.class) | Java | 20 | 0 | 0 | 20 |
| [server/target/classes/bgu/spl/net/srv/Server.class](/server/target/classes/bgu/spl/net/srv/Server.class) | Java | 10 | 0 | 0 | 10 |
| [server/trash/echo/EchoClient.java](/server/trash/echo/EchoClient.java) | Java | 29 | 1 | 9 | 39 |
| [server/trash/echo/EchoProtocol.java](/server/trash/echo/EchoProtocol.java) | Java | 20 | 0 | 7 | 27 |
| [server/trash/echo/EchoServer.java](/server/trash/echo/EchoServer.java) | Java | 11 | 7 | 6 | 24 |
| [server/trash/newsfeed/FetchNewsCommand.java](/server/trash/newsfeed/FetchNewsCommand.java) | Java | 13 | 0 | 7 | 20 |
| [server/trash/newsfeed/NewsFeed.java](/server/trash/newsfeed/NewsFeed.java) | Java | 22 | 0 | 7 | 29 |
| [server/trash/newsfeed/NewsFeedClientMain.java](/server/trash/newsfeed/NewsFeedClientMain.java) | Java | 40 | 1 | 14 | 55 |
| [server/trash/newsfeed/NewsFeedServerMain.java](/server/trash/newsfeed/NewsFeedServerMain.java) | Java | 14 | 7 | 6 | 27 |
| [server/trash/newsfeed/PublishNewsCommand.java](/server/trash/newsfeed/PublishNewsCommand.java) | Java | 16 | 0 | 6 | 22 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)