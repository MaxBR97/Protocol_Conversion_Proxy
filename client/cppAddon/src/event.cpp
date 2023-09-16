#include "../include/event.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using json = nlohmann::json;
using namespace std;

Event::Event(std::string _team_a_name, std::string _team_b_name, std::string _name, int _time,
             std::map<std::string, std::string> _game_updates, std::map<std::string, std::string> _team_a_updates,
             std::map<std::string, std::string> _team_b_updates, std::string _description)
    :  team_a_name(_team_a_name), team_b_name(_team_b_name), name(_name),
      time(_time), game_updates(_game_updates), team_a_updates(_team_a_updates),
      team_b_updates(_team_b_updates), description(_description), sender ()
{
}

Event::~Event()
{
}

void Event::setSender(const std::string& s)
{
    this->sender = s;
}
const std::string &Event::getSender() const
{
    return this->sender;
}

const std::string &Event::get_team_a_name() const
{
    return this->team_a_name;
}

const std::string &Event::get_team_b_name() const
{
    return this->team_b_name;
}

const std::string &Event::get_name() const
{
    return this->name;
}

int Event::get_time() const
{
    return this->time;
}

const std::map<std::string, std::string> &Event::get_game_updates() const
{
    return this->game_updates;
}

const std::map<std::string, std::string> &Event::get_team_a_updates() const
{
    return this->team_a_updates;
}

const std::map<std::string, std::string> &Event::get_team_b_updates() const
{
    return this->team_b_updates;
}

const std::string &Event::get_discription() const
{
    return this->description;
}

const void Event::parseDestination(std::string destination)
{
              int start = 0;
              int middle = destination.find('_');
              int end = destination.length();
              std::string teamA = destination.substr(start, middle);
              middle = middle + 1;
              std::string teamB = destination.substr(middle,end);
              team_a_name = teamA;
              team_b_name = teamB;
}

Event::Event(const std::string &frame_body) : team_a_name(""), team_b_name(""), name(""), time(0), game_updates(), team_a_updates(), team_b_updates(), description(""), sender()
{
    std::istringstream ss(frame_body);
    std::string word;
    bool flagTeamAUpdates = false;
    bool flagTeamBUpdates = false;
    bool flagDescription = false;
    bool someOtherFlag = true;
    while(std::getline(ss,word,'\n') && someOtherFlag)
    {
       //ss>>word;
       int start = 0;
       int middle = word.find(':');
       int end = word.length();
       std::string header = word.substr(start, middle);
       middle = middle + 1;
       std::string value = word.substr(middle,end);
       if(header.compare("user") == 0)
       {
          sender = value;
       }
       else if(header.compare("team a") == 0)
       {
          team_a_name = value;
       }
       else if(header.compare("team b") == 0)
       {
          team_b_name = value;
       }
       else if(header.compare("event name") == 0)
       {
          name = value;
       }
       else if(header.compare("time") == 0)
       {
          time = stoi(value);
       }
       if(header.compare("general game updates") == 0)
       {
           someOtherFlag = false;
          std::getline(ss,word,'\n');
          while(!word.compare("team a updates:") == 0)
          {
              word = word.substr(3,word.length());
              int start = 0;
              int middle = word.find(':');
              int end = word.length();
              std::string header = word.substr(start, middle);
              middle = middle + 1;
              std::string value = word.substr(middle,end);
              game_updates[header] = value;
              std::getline(ss,word,'\n');
          }
          flagTeamAUpdates = true; 
       }

        if(flagTeamAUpdates)
       {
          std::getline(ss,word,'\n');
          while(!word.compare("team b updates:") == 0)
          {
              word = word.substr(3,word.length());
              int start = 0;
              int middle = word.find(':');
              int end = word.length();
              std::string header = word.substr(start, middle);
              middle = middle + 1;
              std::string value = word.substr(middle,end);
              team_a_updates[header] = value;
              std::getline(ss,word,'\n');
          }
          flagTeamAUpdates = false;
          flagTeamBUpdates = true;
       }
        if(flagTeamBUpdates)
       {
           std::getline(ss,word,'\n');
           while(!word.compare("description:") == 0)
           {
              word = word.substr(3,word.length());
              int start = 0;
              int middle = word.find(':');
              int end = word.length();
              std::string header = word.substr(start, middle);
              middle = middle + 1;
              std::string value = word.substr(middle,end);
              team_b_updates[header] = value;
              std::getline(ss,word,'\n');
          }
          flagTeamBUpdates = false;
          flagDescription = true;
          
       }
        if(flagDescription)
       {
          std::getline(ss,word,'\n');
          description += word;
          while(std::getline(ss,word,'\n'))
          {
              description += "\n";
              description += word;
          }
          
       }
    }
}

 const std::string Event::toString() const
 {
   std::string ans = "";
   std::map <std::string, std::string> mp;
   ans.append("user");
   ans.append(":");
   ans.append(Event::getSender());
   ans.append("\n");

   ans.append("event name");
   ans.append(":");
   ans.append(Event::get_name());
    ans.append("\n");

   ans.append("time");
   ans.append(":");
   ans.append(std::to_string(Event::get_time()));
    ans.append("\n");

   ans.append("general game updates");
   ans.append(":\n");
   mp = Event::get_game_updates();
   for (auto it = mp.begin(); it!= mp.end();it++)
   {
      ans.append("   ");
      ans.append( it->first);
      ans.append( ":");
      ans.append( it->second);
      ans.append( "\n");
   }

   ans.append("team a updates");
   ans.append(":");
   ans.append("\n");
   mp = Event::get_team_a_updates();
   for (auto it = mp.begin(); it!= mp.end();it++)
   {
      ans.append( "   ");
      ans.append( it->first);
      ans.append( ":");
      ans.append( it->second);
      ans.append( "\n");
   }

   ans.append("team b updates");
   ans.append(":");
   ans.append("\n");
   mp = Event::get_team_b_updates();
   for (auto it = mp.begin(); it!= mp.end();it++)
   {
      ans.append( "   ");
      ans.append( it->first);
      ans.append( ":");
      ans.append( it->second);
      ans.append( "\n");
   }

   ans.append("description");
   ans.append(":\n");
   ans.append(Event::get_discription());

   return ans;
 }



names_and_events parseEventsFile(std::string json_path)
{
    //f.open (json_path, std::ifstream::in);
    std::ifstream f(json_path);
    json data = json::parse(f);

    std::string team_a_name = data["team a"];
    std::string team_b_name = data["team b"];

    // run over all the events and convert them to Event objects
    std::vector<Event> events;
    for (auto &event : data["events"])
    {
        std::string name = event["event name"];
        int time = event["time"];
        std::string description = event["description"];
        std::map<std::string, std::string> game_updates;
        std::map<std::string, std::string> team_a_updates;
        std::map<std::string, std::string> team_b_updates;
        for (auto &update : event["general game updates"].items())
        {
            if (update.value().is_string())
                game_updates[update.key()] = update.value();
            else
                game_updates[update.key()] = update.value().dump();
        }

        for (auto &update : event["team a updates"].items())
        {
            if (update.value().is_string())
                team_a_updates[update.key()] = update.value();
            else
                team_a_updates[update.key()] = update.value().dump();
        }

        for (auto &update : event["team b updates"].items())
        {
            if (update.value().is_string())
                team_b_updates[update.key()] = update.value();
            else
                team_b_updates[update.key()] = update.value().dump();
        }
        
        events.push_back(Event(team_a_name, team_b_name, name, time, game_updates, team_a_updates, team_b_updates, description));
    }
    names_and_events events_and_names{team_a_name, team_b_name, events};

    return events_and_names;
}