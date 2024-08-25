# Educational Project - Instant Group Messaging App For Soccer
## What is this project?
This project originally started as a university project to make a client and a server from scratch using native libraries only to form an instant messaging app for soccer game updates.
* Each client can subscribe to specific game channels (i.e France vs Argentina)
* Each client can post a report to a channel regarding live updates/thoughts.
* Each client can request a summary of a game statistics by compiling all of the statistics gathered from all reports from all users in the channel.
  
### Implementation Specification:
* Client is written in C++, Server is written in Java.
* Server can run in two configurations: "Thread-Per-Client" and "Reactor"
* Server and Client communicate through STOMP protocol (application layer protocol)

***Thread-Per-Client** mode is when the server dedicates a port to every client that connects. It is good for when there is a small amount of clients (not more than about 2^16) to serve each client quickly and efficiently.

***Reactor** mode is when the server runs with a fixed number of threads that are responsible to serve all clients, regardless of how many clients there are. It is good for limiting the resources dedicated by the server and to serve a much higher amount of clients simultaneously, although the response time might be slower.

### What Makes This Project Significant?
* 
