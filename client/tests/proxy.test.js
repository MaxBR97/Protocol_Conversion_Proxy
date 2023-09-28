const {app, server, serverInitCallback} = require('../proxy_server.js')
const {exec} = require(`child_process`)
const path = require(`path`)
const axios = require(`axios`);
const { beforeAll, afterAll } = require('@jest/globals');
const { doesNotMatch } = require('node:assert');
//const { expect, jest } = require('@jest/globals')
//var waitOn = require(`wait-on`)

const stompServerIP = "192.168.56.1";
const stompServerPort = "5521";
const channelTeamA = "AA"
const channelTeamB = "BB"
let users = [] // username, passcode, clientInstance, baseURL
const amountOfUsers = 6; // must be 1
// let expressServerPort = server.address().port;
// let baseURL1 = `http://localhost:${expressServerPort}`
// let baseURL2 = `http://localhost:${expressServerPort}`

const getPort = (serverInstance) => {
    return serverInstance.address().port;
}

const delay = async (milis) => {
    await new Promise((resolve) => {
        setTimeout(resolve,milis)
    })
}
describe("test scalability", () => {

    beforeAll(async () => {
        const indexPath = path.join(path.resolve(path.join(__dirname), `../`),'proxy_server.js');
        
        //var serverProcess = require(`child_process`).spawn('node ./proxy_server.js')
        for(let i=0; i<amountOfUsers; i++) {
            let username = (Math.random() + 1).toString(36).substring(2);
            let passcode = (Math.random() + 1).toString(36).substring(2);
            let instance;
            if(i==0) {
                 instance = server;
            }
            else{
                instance = await app.listen(0, serverInitCallback);
            }
            let baseURL = `http://localhost:${getPort(instance)}`
                users.push({
                            "username": username,
                            "passcode": passcode,
                            "clientInstance" : instance,
                            "baseURL": baseURL
                            }
                          )
                          await delay(100)
        }
        let allUserProcesses = []
        for(const user of users) {
            await delay(100)
            //console.log(user)
            let pr =  new Promise(async (resolve, reject) => {
                await axios.put(user.baseURL.concat("/proxyInput"), {command:`login ${stompServerIP}:${stompServerPort} ${user.username} ${user.passcode}`})
                .then ((res) => {expect(res.status).toBe(200);})
                await axios.get(user.baseURL.concat(`/recievedFromStompServer`))
                .then ((res) => {expect(res.data.includes("CONNECTED")).toBe(true)})
                await axios.put(user.baseURL.concat("/proxyInput"), {command:`join ${channelTeamA}_${channelTeamB}`})
                .then((res) => {expect(res.status).toBe(200); resolve(true);})
                reject(true)
            })
            allUserProcesses.push(pr)
        }
        await Promise.all(allUserProcesses)
    })

    test("test 1", async () => {
        const messagesToSend = 10
        let recieved = 0;
        
        for (let i=0; i<messagesToSend; ++i) {
           
        }
    })

    afterAll(async () => {
        await users.forEach( async (user) => {
            await axios.put(user.baseURL.concat("/proxyInput"), {command:`logout`})
                    .then ((res) => {expect(res.status).toBe(200);})
            await user.clientInstance.close()
         })
    })

})