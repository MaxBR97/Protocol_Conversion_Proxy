import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import axios from 'axios'
const baseURL = "http://192.168.56.102:7777"

const delay = (mili) => {
  return new Promise( (resolve) => setTimeout(resolve, mili))
}

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
    console.log("command: ",text)
    commandInput.value = "";
    setUserPanel(userPanel + "\n" + text)
    axios.put(baseURL.concat(`/proxyInput`), {command : text})
  }
  
 useEffect(async () => {
    if(listenToProxy) {
      new Promise( async (resolve) => {
        while(listenToProxy) {
          await axios.get(baseURL.concat('/proxyOutput')).then((ans) => setProxyToUser(proxyToUser + '\n' + ans))
          await delay(2000)
        }
        resolve()
      })

    }
 }, [listenToProxy])

  useEffect( async () => {
    if(listenToServer) {
      new Promise( async (resolve) => {
        while(listenToServer) {
          await axios.get(baseURL.concat('/sentToStompServer')).then((ans) => setProxyToServer(proxyToServer + '\n' + ans))
          await delay(2000)
        }
        resolve()
      })

      new Promise( async (resolve) => {
        while(listenToProxy) {
          await axios.get(baseURL.concat('/recievedFromStompServer')).then((ans) => setProxyToUser(proxyToServer + '\n' + ans))
          await delay(2000)
        }
        resolve()
      })

    }
   
  }, [listenToServer])

  return (
    <>
    <form> 
      <label>command line:</label>
      <input className="commandText" type="text" ></input>
      <button onClick={sendRequest}>Send Command</button>
    </form>
    <div>
      <Panel panelName={"User Panel"} text= {userPanel}/>
      <Panel panelName={"Proxy-Client Panel"} text={proxyToUser}/>
      <Panel panelName={"Proxy-Server Panel"} text={proxyToServer}/>
    </div>
    </>
  )
}

export default App
