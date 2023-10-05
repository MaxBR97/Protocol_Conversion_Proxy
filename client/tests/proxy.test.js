const {app, server, testServerInitCallback} = require('../proxy_server.js')
const {exec} = require(`child_process`)
const path = require(`path`)
const axios = require(`axios`);
const { beforeAll, afterAll } = require('@jest/globals');
const { doesNotMatch } = require('node:assert');
const { off } = require('process');
//const { expect, jest } = require('@jest/globals')
//var waitOn = require(`wait-on`)

const stompServerIP = "192.168.56.1";
const stompServerPort = "5521";
const channelTeamA = "AA"
const channelTeamB = "BB"
let users = [] // username, passcode, clientInstance, baseURL
const amountOfUsers = 1; // must be 1
const messagesToSendEach = (process.env.MESSAGES_EACH || 10);
let messagesToRecieve = (process.env.CLIENTS || 1) * messagesToSendEach;
const delaying = 0;

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
        let i=0;
        while(users.length<amountOfUsers) {
            let username = (Math.random() + 1).toString(36).substring(2);
            let passcode = (Math.random() + 1).toString(36).substring(2);
            let instance;
            if(i==0) {
                 instance = server;
            }
            else{
                instance = await app.listen(0, testServerInitCallback);
            }
            let baseURL = `http://localhost:${getPort(instance)}`
            await delay(delaying*3)
            await new Promise(async (resolve, reject) => {
                await axios.put(baseURL.concat("/proxyInput"), {command:`login ${stompServerIP}:${stompServerPort} ${username} ${passcode}`}, )
                .then ((res) => {})
                .catch((err) => reject(true))
                await delay(delaying)
                await axios.get(baseURL.concat(`/recievedFromStompServer`), )
                .then ((res) => {res.data.includes("CONNECTED") ? 0 : reject(true)})
                .catch((err) => reject(true))
                await delay(delaying)
                await axios.put(baseURL.concat("/proxyInput"), {command:`join ${channelTeamA}_${channelTeamB}`}, )
                .then((res) => {})
                .catch((err) => reject(true))
                await delay(delaying)
                await axios.get(baseURL.concat(`/recievedFromStompServer`),)
                .then ((res) => {res.data.includes("RECEIPT") ? resolve(true) : reject(true);})
                .catch((err) => reject(true))
                await delay(delaying)
                reject(true)
            }). then( (result) => {users.push({
                "username": username,
                "passcode": passcode,
                "clientInstance" : instance,
                "baseURL": baseURL
                }
              )
            }) . catch((err) => {})

            i++;
            await delay(delaying*3)
                        
        }
        //console.log("FINISHED INITIALIZING",users.length)
        await delay(delaying)
    },10000)

    test("test 1", async () => {
        await delay(3000+delaying + (process.env.CLIENTS*0.1))
        let allListeners = [];
        for(const user of users) {
            let pr = new Promise(async resolve => {
                let recieved = 0;
               
                while ( recieved < (messagesToRecieve)) {
                    await axios.get(user.baseURL.concat(`/recievedFromStompServer`), {timeout:1000})
                    .then ((res) => {
                        if(res.data.includes("MESSAGE")){
                            recieved ++; 
                            console.log("recieved ",recieved);
                        } else {
                            console.log("error 2")
                            throw new Error("didn't recieve");
                        }
                    })
                    .catch((err) => {})
                }
                resolve()
            })
            allListeners.push(pr)
        }
       
            for (let i=0; i<messagesToSendEach; ++i) {
                for(const user of users) {
                    await axios.put(user.baseURL.concat(`/proxyInput`), {"command" : "report existing testReport.json"})
                    await delay(delaying)
                }
            }

        await Promise.all(allListeners);
    }, 10000 + (messagesToRecieve*10) + process.env.CLIENTS*50)

    afterAll(async () => {
        await users.forEach( async (user) => {
            await axios.put(user.baseURL.concat("/proxyInput"), {command:`logout`})
                    .then ((res) => {expect(res.status).toBe(200);})
            await user.clientInstance.close()
         })
    })

})