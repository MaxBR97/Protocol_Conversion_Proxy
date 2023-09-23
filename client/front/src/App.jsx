import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import axios from 'axios'
const PORT = window.dynamicPort
const baseURL = `http://192.168.56.102:${PORT}`

const delay = (mili) => {
  return new Promise( (resolve) => setTimeout(resolve, mili))
}

const Panel = ({panelName,text}) => {
  return (
    <div className={`${panelName}`}>
      <label className="panelName">{text}</label>
      <div className="content">  
      </div>
    </div>)
}

const Stat = () => {
  return(<div className="stat">
      <input className="stat_name" type="text"></input>
      <div>:</div>
      <input className="stat_value" type="text"></input>
    </div>)
}

const Event = () => {
  const [teamAStats, setTeamAStats] = useState([])
  const [teamBStats, setTeamBStats] = useState([])

  const addTeamAStat = (event) => {
    event.preventDefault()
    setTeamAStats([...teamAStats, teamAStats.length])
  }
  const addTeamBStat = (event) => {
    event.preventDefault()
    setTeamBStats([...teamBStats, teamBStats.length])
  }
  const removeTeamAStat = (event) => {
    event.preventDefault()
    let copy = [...teamAStats];
    copy.pop()
    setTeamAStats(copy)
  }
  const removeTeamBStat = (event) => {
    event.preventDefault()
    let copy = [...teamBStats];
    copy.pop()
    setTeamBStats(copy)
  }

    return(<>
      <label >Event Name: </label>
      <input className="event_name" type="text"></input>
      <label>Time: </label>
      <input className="time" type="number"></input>
      <label>General Updates: </label>
      <label className="wrapping_label">Active <input className="active" type="checkbox" text="Active" value="Active" name="Active"></input> </label>
      <label className="wrapping_label">Before Halftime<input className="before_halftime" type="checkbox" text="Before Halftime"></input> </label>
      <label>Team A Stats: </label>
      <ul className="team_a_stats">
        {teamAStats.map((item,index)=> <Stat key={item}/>)}
        <button className="add_stat" onClick={addTeamAStat}>Add Stat</button>
        <button className="remove_stat" onClick={removeTeamAStat}>Remove Stat</button>
      </ul>
      <label>Team B Stats: </label>
      <ul className="team_b_stats">
      {teamBStats.map((item,index)=> <Stat key={item}/>)}
         <button className="add_stat" onClick={addTeamBStat}>Add Stat</button>
         <button className="remove_stat" onClick={removeTeamBStat}>Remove Stat</button>
      </ul>
      <label >Description: </label>
      <input className="description" type="text"></input>
    </>)
}

const ReportComponent = () => {
  const [events, setEvents] = useState([])

  const onAddEvent = (event) => {
    event.preventDefault()
    setEvents([...events, events.length])
  }
  const onRemoveEvent =(ev) => {
    ev.preventDefault()
    let n = [...events]
    n.pop()
    setEvents(n)
  }
  
  return( <div className="report_component">
  <label>Team A:</label>
  <input className="team_a" type="text"></input>
  <label>Team B:</label>
  <input className="team_b" type="text"></input>
  <ul className="events">
    {events.map((item, index) => 
      (<li className="event"><Event key={item}/></li>)
    )}
  </ul>
  <button className="add_event_button" onClick={onAddEvent}>Add Event</button>
  <button className="remove_event_button" onClick={onRemoveEvent}>Remove Event</button>

  </div>)
}

const ShowHelpComp = () => {
  return (
    <>
    <p className="help_paragraph">
      Commands:
      <br/> Login to Stomp server: "login (StompServerIP):(StompServerPort) (username) (passcode)"
      <br/> Join to a game reporting channel: "join (teamA)_(teamB)"
      <br/> Report new event to channel: 
      <br/> ------1. "report"      
      <br/> ------2. Fill the event details and click 'Send Report'
      <br/> Report existing file in proxy from previous sessions: "report existing (fileName)"
      <br/> Summarize game reports from particular user: "summary (teamA)_(teamB) (userToSummarizeFrom)"
      <br/> Summarize game reports from all users: "summary (teamA)_(teamB) @all"   
      <br/> Exit game channel: "exit (teamA)_(teamB)"
      <br/> Logout from Stomp server: "logout"

    </p>
    </>
  )
}

function App() {
  const [listenToProxy, setListenToProxy] = useState(true);
  const [listenToServer, setListenToServer] = useState(true)
  const [reportComp, setReportComp] = useState(false)
  const [showHelp, setShowHelp] = useState(false)
  let userPanel = "User Panel"
  let proxyPanel = "Proxy Panel"
  let serverPanel = "Server Panel"

  const sendRequest = (event) => {
    event.preventDefault()
    const commandInput = event.target.parentNode.querySelector(".commandText")
    const text = commandInput.value
    console.log("command: ",text)
    commandInput.value = "";
    appendToPanel(userPanel, text)
    if(!handleLocally(text)) {
        axios.put(baseURL.concat(`/proxyInput`), {command : text})
    }
  }
  
  const onShowHelp = (event)=> {
    event.preventDefault()
    setShowHelp(!showHelp)
  }

  const handleLocally = (command) => {
    if(command == "report")
    {
      setReportComp(true)
      return true;
    }
    else if(command.includes("report")) {
      axios.put(baseURL.concat(`/proxyInput`), {command}).then( (ans) => {
        appendToPanel(proxyPanel, ans.data)
      })
      return true;
    }
    else if(command.includes("summary")) {
      axios.put(baseURL.concat(`/proxyInput`), {command}).then( (res) => {
        appendToPanel(proxyPanel,res.data)
      })
      return true;
    }
    return false;
  }

  const onSendReport = (event) => {
    event.preventDefault()
    let report = {}
    let teamA = document.querySelector(".team_a").value;
    let teamB = document.querySelector(".team_b").value;
    let events = []
    document.querySelector(".events").childNodes.forEach( (val,key) => {
      console.log(val)
      let eventName = val.querySelector(".event_name").value
      let time = val.querySelector(".time").value
      let active = val.querySelector(".active").checked
      let beforeHalftime = val.querySelector(".before_halftime").checked
      let team_a_stats = {}
      val.querySelectorAll(".team_a_stats .stat").forEach( (val) => {
        console.log("---",val)
        let statName = val.querySelector(".stat_name").value
        console.log("------", statName)
        let statVal = val.querySelector(".stat_value").value
        team_a_stats[statName] = statVal
      })
      let team_b_stats = {}
      val.querySelectorAll(".team_b_stats .stat").forEach( (val) => {
        let statName = val.querySelector(".stat_name").value
        let statVal = val.querySelector(".stat_value").value
        team_b_stats[statName] = statVal
      })
      let description = val.querySelector(".description").value

      let event = {}
      event["event name"] = eventName
      event["time"] = parseInt(time)
      let generalUpdates = {"active": active, "before halftime": beforeHalftime}
      event["general game updates"] = generalUpdates
      event["team a updates"] = team_a_stats
      event["team b updates"] = team_b_stats
      event["description"] = description
      events.push(event)
    })

    report["team a"] = teamA
    report["team b"] = teamB
    report["events"] = events
    let toSend = {"command": `report ${teamA}_${teamB}`, "json" : report}
    axios.put(baseURL.concat('/proxyInput'), toSend).then((ans) => {appendToPanel(proxyPanel,ans.data);  setReportComp(false)})
  
  }

  const appendToPanel = (panelName, text) => {
    if(text.length !=0) {
      let newDiv = document.createElement("div");
      if(text.includes("MESSAGE"))
        newDiv.classList.add("event_from_channel")
      if(text.includes(" vs "))
        newDiv.classList.add("summary_from_channel")
      newDiv.classList.add("message")
      let textToAppend = document.createTextNode(text)
      newDiv.append(textToAppend)
      let arr = panelName.split(" ")
      const classes = arr.reduce((acc,cur,index,arr)=> acc+"."+cur, "")
      document.querySelector(`${classes} div.content`).append(newDiv)
    }
    return;
  }
  
 useEffect(async () => {
    if(listenToProxy) {
      new Promise( async (resolve) => {
        while(listenToProxy) {
          await axios.get(baseURL.concat('/proxyOutput')).then((ans) => appendToPanel(proxyPanel,ans.data))
          //await delay(2000)
        }
        resolve()
      })

    }
 }, [listenToProxy])

  useEffect( async () => {
    if(listenToServer) {
      // new Promise( async (resolve) => {
      //   while(listenToServer) {
      //     await axios.get(baseURL.concat('/sentToStompServer')).then((ans) => appendToPanel())
      //     await delay(2000)
      //   }
      //   resolve()
      // })

      new Promise( async (resolve) => {
        while(listenToServer) {
          await axios.get(baseURL.concat('/recievedFromStompServer')).then((ans) => appendToPanel(serverPanel,ans.data))
          //await delay(2000)
        }
        resolve()
      })

    }
   
  }, [listenToServer])

  return (
    <>
    
    <form className="report_form">
      {reportComp ? (<><ReportComponent/> 
        <button className="send_event_button" onClick={onSendReport}>Send Report</button></>)
      : <></>}
    </form>
    <form> 
      <label>command line:    </label>
      <input className="commandText" type="text" ></input>
      <button className="send_command_button" onClick={sendRequest}>Send Command</button>
    </form>
    <div className="allPanels">
      <Panel panelName={userPanel} text={"User Commands Panel"} />
      <Panel panelName={proxyPanel} text={"Responses From Proxy"}/>
      <Panel panelName={serverPanel} text={"Responses From Server"}/>
    </div>
    <button className="show_help_button" onClick={onShowHelp}>Help</button>
      {showHelp ? (<ShowHelpComp/>) : <></>}
    </>
  )
}

export default App
