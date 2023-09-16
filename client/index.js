const client = require(`./build/Release/client`);
const express = require(`express`)
const fs = require(`fs`)
const path = require(`path`)
//const cors = require(`cors`)
const app = express()

app.get(`/proxyOutput`, (req, res) => {
    res.send("unimplemented1")
})

app.post(`/proxyInput` , (req, res) => {
    res.send("unimplemented2")              
})

app.get(`/sentToStompServer`, (req, res) => {
    res.send("unimplemented3")

})

app.get(`/recievedFromStompServer`, (req, res) => {
    res.send("unimplemented4")

})

app.get(`/getFileContent/:id`, (req,res) => {
    res.send("unimplemented5")
})

app.put(`/createFile`, (res, req) => {
    res.send("unimplemented6")
})


const PORT = 7777;
app.listen(PORT, () => console.log(`Serving at port`, PORT))