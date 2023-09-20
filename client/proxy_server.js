const client = require(`./build/Release/client`);
const express = require(`express`)
const fs = require(`fs`)
const path = require(`path`)
//const cors = require(`cors`)
const bodyParser = require(`body-parser`)
const app = express()
app.use(express.static(__dirname+`/front/dist`))
app.use(express.json());
app.use(express.urlencoded({ extended: true }));


app.get(`/proxyOutput`, async (req, res) => {
        console.log("/proxyOutput");
        const result = client.getOutput((data) => {
            res.send(data)
        })
})

app.put(`/proxyInput` , async (req, res) => {
    new Promise ( (resolve) => {
    console.log("/proxyInput: ", req.body)
    client.setInput(req.body)
    resolve()        
    })
    .then( (ans) => res.end()) 
})

app.get(`/sentToStompServer`, async (req, res) => {
        console.log("/sentToStompServer");
        const result = await client.getFrameOut((data) => {
            res.send(data);
        })
})

app.get(`/recievedFromStompServer`, async (req, res) => {
    console.log("/recievedFromStompServer")
    const result = await client.getFrameIn( (data) => {
        res.send(data)
    })
})

app.get(`/getFileContent/:id`, (req,res) => {
    res.send("unimplemented5")
})

app.put(`/createFile`, (res, req) => {
    res.send("unimplemented6")
})

const PORT = 0;
const SleepSeconds = 3;
const SleepNanos = 0;
const server = app.listen(PORT, () =>{ 
    client.initialize(); 
    client.setSleepTimes(SleepSeconds,SleepNanos);
    console.log(`Serving at port`, server.address().port)
})