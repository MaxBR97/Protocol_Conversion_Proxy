#include "../include/InputProtocol.h"
#include <sstream>
#include <fstream>
#include <vector>

InputProtocol::InputProtocol () : command() , umap(), myUsername(){}

bool InputProtocol::process(std::string message)
{
    std::istringstream ss(message);
    string word;
    ss>>word;
    
    
    if( /*word == NULL ||*/ !InputProtocol::setCommand(word))
     {
        std::cout << " invalid command" << message <<" " << std::endl;
        return false;
     }
    
    if(ss && command == InputProtocol::Command::LOGIN)
    {
        ss>>word;
        int start = 0;
        int middle = word.find(':');
        int end = word.length();
        if( middle ==-1)
        {
           std::cout<< " invalid host:port format"<< std::endl;
           return false; 
        }
        string host = word.substr(start, middle);
        middle = middle + 1;
        string port = word.substr(middle,end);
        umap[InputProtocol::Header::HOST] = host;
        umap[InputProtocol::Header::PORT] = port;
        
        if(ss)
        {
           ss>>word;
           umap[InputProtocol::Header::USERNAME] = word;
        }
        else 
        {
            std::cout<< " no username"<< std::endl;
            return false;
        }

        if(ss)
        {
           ss>>word;
           umap[InputProtocol::Header::PASSWORD] = word;
        }
        else 
        {
            std::cout<< " no password"<< std::endl;
            return false;
        }
        return true;
    }
    else if(ss && command == InputProtocol::Command::JOIN)
    {
       ss>>word;
       umap[InputProtocol::Header::GAME_NAME] = word;
       return true;
    }
    else if(ss && command == InputProtocol::Command::EXIT)
    {
       ss>>word;
       umap[InputProtocol::Header::GAME_NAME] = word;
       return true;
    }
    else if(ss && command == InputProtocol::Command::REPORT)
    {
       ss>>word;
       umap[InputProtocol::Header::FILE] = word;
       return true;
    }
    else if(ss && command == InputProtocol::Command::SUMMARY)
    {
       ss>>word;
       umap[InputProtocol::Header::GAME_NAME] = word;

       if(ss)
       {
        ss>>word;
        umap[InputProtocol::Header::USERNAME] = word;
       }
       else
       {
          std::cout<< " no username entered "<< std::endl;
          return false;
       }

       if(ss)
       {
        ss>>word;
        umap[InputProtocol::Header::FILE] = word;
       }
       else
       {
          std::cout<< " no file entered "<< std::endl;
          return false;
       }
       return true;
    }
    else if (command == InputProtocol::Command::LOGOUT)
       return true;
    else
    {
        std::cout << "command without headers" << std::endl;
        return false;
    }
}

bool InputProtocol::setCommand(std::string word)
{
    if(word.compare("login") == 0)
       command = InputProtocol::Command::LOGIN;
    else if(word.compare("join") == 0)
       command = InputProtocol::Command::JOIN;
    else if(word.compare("exit") == 0)
       command = InputProtocol::Command::EXIT;
    else if(word.compare("report") == 0)
       command = InputProtocol::Command::REPORT;
    else if(word.compare("summary") == 0)
       command = InputProtocol::Command::SUMMARY;
    else if(word.compare("logout") == 0)
       command = InputProtocol::Command::LOGOUT;
    else 
       return false;
    return true;
}

void InputProtocol::SetUsername(std::string s)
{
   myUsername.clear();
   myUsername.append(s);
}

void InputProtocol::execute(StompProtocol& stomp)
{
     
        if(command  == InputProtocol::Command::LOGIN)
        {
         if(stomp.isLoggedIn())
            cout<<"The client is already logged in, log out before trying again."<<endl;
         else{
           std::string host = umap[InputProtocol::Header::HOST];
           std::string user = umap[InputProtocol::Header::USERNAME];
           stomp.setPort(stoi(umap[InputProtocol::Header::PORT]));
           std::string version = "1.2";
           std::string pass = umap[InputProtocol::Header::PASSWORD];
           SetUsername(user);
           stomp.sendConnectMessage(version,host,user,pass);
           }
        }
        else if(command  == InputProtocol::Command::JOIN)
        { 
            std::string destination = umap[InputProtocol::Header::GAME_NAME];
            stomp.sendSubscribeMessage(destination,true);
        }
        else if(command == InputProtocol::Command::EXIT)
        {
           std::string destination = umap[InputProtocol::Header::GAME_NAME];
           stomp.sendUnsubscribeMessage(destination, true); 
        }
        else if(command  == InputProtocol::Command::REPORT)
        {
           names_and_events evs = ::parseEventsFile(umap[InputProtocol::Header::FILE]);
           for (auto & element : evs.events)
           {
              element.setSender(myUsername);
              std::string body = "";
              body.append(element.toString());
              body.append("\n");
              std::string destination = "";
              destination.append(evs.team_a_name);
              destination.append("_");
              destination.append(evs.team_b_name);
              stomp.sendSendMessage(destination,body, false); 
           }
        }
        else if(command  == InputProtocol::Command::SUMMARY)
        {
            std::string file = umap[InputProtocol::Header::FILE];
            std::string destination = umap[InputProtocol::Header::GAME_NAME];
            std::string user = umap[InputProtocol::Header::USERNAME];
            std::vector<Event> eventsReported = stomp.getGameUpdatesFrom(user, destination);
            std::vector<int> times;
            std::vector<Event> sortedEvents;
            for(auto & t : eventsReported)
            {
               times.push_back(t.get_time());
            }
            std::sort(times.begin(),times.end());

            for(auto & t : times)
            {
               for(auto & ev : eventsReported)
               {
                  if(ev.get_time() == t)
                  {
                     sortedEvents.push_back(ev);
                  }
               }
            }
            std::string reported[100];
            int index = 0;
           // cout<<"vector SIZE: " << reported.size() << endl;
            ofstream myfile;
            myfile.open(file);
            
            int start = 0;
            int middle = destination.find('_');
            int end = destination.length();
            std::string teamAName = destination.substr(start, middle);
            middle = middle + 1;
            std::string teamBName = destination.substr(middle,end); 
            myfile << teamAName << " vs " << teamBName << endl;

            myfile << "\nGame stats:\n" << endl;
            myfile << "GeneralStats:\n" << endl;
            for (auto ev = sortedEvents.rbegin(); ev != sortedEvents.rend(); ++ev)
            {
               bool flag = true;
               for(auto pair = ev->get_game_updates().begin(); pair != ev->get_game_updates().end(); pair++ )
               {
               for(auto q = 0; q != 100; q++)
               {
                  if((pair->first).compare(reported[q]) == 0 && (pair->first).length() > 1)
                  {
                     flag = false;
                  }
               }
               if(flag)
               {
                  reported[index] = pair->first;
                  index++;
                  myfile << pair->first << ":"<< pair->second << endl;
               }
               }
            }
            std::fill(reported, reported+100, "");
            myfile << "\nteam_a_stats:" << endl;
            for (auto ev = sortedEvents.rbegin(); ev != sortedEvents.rend(); ++ev)
            {
               bool flag = true;
               for(auto pair = ev->get_team_a_updates().begin(); pair != ev->get_team_a_updates().end(); pair++ )
               {
               for(auto q = 0; q != 100; q++)
               {
                  if((pair->first).compare(reported[q]) == 0 && (pair->first).length() > 1)
                  {
                     flag = false;
                  }
               }
               if(flag)
               {
                  reported[index] = pair->first;
                  index++;
                  myfile << pair->first << ":"<< pair->second << endl;
               }
               }
            }
            myfile << "\nteam_b_stats:" << endl;
            std::fill(reported, reported+100, "");
            for (auto ev = sortedEvents.rbegin(); ev != sortedEvents.rend(); ++ev)
            {
               bool flag = true;
               for(auto pair = ev->get_team_b_updates().begin(); pair != ev->get_team_b_updates().end(); pair++ )
               {
               for(auto q = 0; q != 100; q++)
               {
                  if(pair->first.compare(reported[q]) == 0)
                  {
                     flag = false;
                  }
               }
               if(flag)
               {
                  reported[index] = pair->first;
                  index++;
                  myfile << pair->first << ":"<< pair->second << endl;
               }
               }
            }
            
            myfile << "Game event reports:\n" << endl;
            std::fill(reported, reported+100, "");
            try{
            for (auto  ev = sortedEvents.begin() ; ev != sortedEvents.end(); ev++)
            {
               myfile << ev->get_time() << " - " << ev->get_name() << "\n\n"<<
               ev->get_discription() << "\n\n"<< endl;
            }
            } catch(std::exception &e){cout<<"Exception caught 5"<< endl;}

            myfile.close();
        }
        else if( command  == InputProtocol::Command::LOGOUT)
        {
           stomp.sendDisconnectMessage(true);
        }
}