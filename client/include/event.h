#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>

class Event
{
public:
    Event( std::string team_A_name, std::string team_B_name, std::string _name, int _time, std::map<std::string, std::string> game_up, std::map<std::string, std::string> team_a_up, std::map<std::string, std::string> team_b_up, std::string desc);
    Event(const std::string & frame_body);
    virtual ~Event();
    const std::string &getSender() const;
    const std::string &get_team_a_name() const;
    const std::string &get_team_b_name() const;
    const std::string &get_name() const;
    int get_time() const;
    const std::map<std::string, std::string> &get_game_updates() const;
    const std::map<std::string, std::string> &get_team_a_updates() const;
    const std::map<std::string, std::string> &get_team_b_updates() const;
    const std::string &get_discription() const;
    void setSender(const std::string& user);
    const std::string toString() const;
    const void parseDestination(std::string destination);

private:
    std::string sender;
    // name of team a
    std::string team_a_name;
    // name of team b
    std::string team_b_name;
    // name of the event
    std::string name;
    // time of the event in seconds
    int time;
    // map of all the general game updates
    std::map<std::string, std::string> game_updates;
    // map of all team a updates the second type can be a string bool or int
    std::map<std::string, std::string> team_a_updates;
    // map of all team b updates
    std::map<std::string, std::string> team_b_updates;
    // description of the event
    std::string description;

};

// an object that holds the names of the teams and a vector of events, to be returned by the parseEventsFile function
struct names_and_events {
    std::string team_a_name;
    std::string team_b_name;
    std::vector<Event> events;
};

// function that parses the json file and returns a names_and_events object
names_and_events parseEventsFile(std::string json_path);

