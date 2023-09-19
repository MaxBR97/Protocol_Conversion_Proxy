import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
const axios = require(`axios`)

const listenToServer = ()
const Panel = ({panelName, text}) => {
  return (
    <div className="Panel">
      <label>{panelName}</label>
      <div>
          {text}
      </div>
    </div>)
}

function App() {

  const [userPanel, setUserPanel] = useState("");
  const [proxyToUser, setProxyToUser] = useState("");
  const [proxyToServer, setProxyToServer] = useState("")
  const [listenToProxy, setListenToProxy] = useState(true);
  const [listenToServer, setListenToServer] = useState(true)

  const sendRequest = (event) => {
    event.preventDefault()
    const commandInput = event.target.parentNode.querySelector(".commandText")
    const text = commandInput.value
    commandInput.value = "";
  }
  
  useEffect(() => {
    new Promise(resolve, () => {
      setInterval()
    })
  }, [])

  return (
    <>
    <form> 
      <label>command line:</label>
      <input className="commandText" type="text" ></input>
      <button onClick={sendRequest}></button>
    </form>
    <Panel panelName={"User Panel"} text= {userPanel}/>
    <Panel panelName={"Proxy-Client Panel"} text={proxyToUser}/>
    <Panel panelName={"Proxy-Server Panel"} text={proxyToServer}/>
    </>
  )
}

export default App
