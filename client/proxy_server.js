const client = require(`./build/Release/client`);
const express = require(`express`)
const fs = require(`fs`)
const path = require(`path`)
//const cors = require(`cors`)
const bodyParser = require(`body-parser`)
const app = express()
const session = require('express-session')


const PORT = 0
let dynamicPort = PORT;
// const SleepSeconds = 0;
// const SleepNanos = 2000000;
const server = app.listen(PORT, () =>{ 
    client.initialize(); 
    console.log(`Serving at port`, server.address().port)
    process.env.PORT=server.address().port
    dynamicPort = server.address().port
})

app.use(session({
  secret:'1nvth4u324rekijureyfdu654gvfdsz',
  resave:false,
  saveUninitialized: true
}))

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.use((req, res, next) => {
    const indexPath = path.join(__dirname, 'front', 'dist', 'index.html');
  
    fs.readFile(indexPath, 'utf8', (err, data) => {
      if (err) {
        console.error('Error reading index.html:', err);
        return res.status(500).send('Internal Server Error');
      }
 
      const newDynamicPort = dynamicPort; 
  
      if (data.includes('<script> window.dynamicPort =')) {
        const modifiedHTML = data.replace(
          /<script> window\.dynamicPort = .*<\/script>/,
          `<script> window.dynamicPort = ${newDynamicPort}</script>`
        );
  
        fs.writeFile(indexPath, modifiedHTML, 'utf8', (err) => {
          if (err) {
            console.error('Error writing index.html:', err);
            return res.status(500).send('Internal Server Error');
          }
  
          // Continue with the next middleware
          next();
        });
      } else {
        // The script doesn't exist, add it
        const modifiedHTML = data.replace(
          '</head>',
          `<script> window.dynamicPort = ${newDynamicPort}</script></head>`
        );
  
        // Write the modification back to the file
        fs.writeFile(indexPath, modifiedHTML, 'utf8', (err) => {
          if (err) {
            console.error('Error writing index.html:', err);
            return res.status(500).send('Internal Server Error');
          }
  
          // Continue with the next middleware
          next();
        });
      }
    });
  });

  const generateUniqueID = () => {
    return Math.random().toString(36).substr(2,9)
  }

app.use(express.static(path.join(__dirname,'front','dist')));

app.get(`/proxyOutput`, async (req, res) => {
        const result = await client.getOutput((data) => {
            res.send(data)
        })
})

app.put(`/proxyInput` , async (req, res) => {
    if(req.body.command.includes("report")) {
      let arr = req.body["command"].split(" ")
      //report existing (file)
      if(arr[0] == "report" && arr[1] == "existing") {
        let fileName = arr[2]
        new Promise( async (resolve) => {
          await client.setInput(`report cppAddon/data/${fileName}`)
          resolve()
        }).then( (ans) => res.end()) .catch((err) => {res.send("no connection to server"); console.log(err)})
      }
      //report (teamA)_(teamB)
      else if(arr[0] == "report") {
        let fileName = arr[1].concat(".json")
        const indexPath = path.join(__dirname, 'cppAddon', 'data',fileName);
        fs.writeFile(indexPath,JSON.stringify(req.body["json"], null, 2) ,`utf8`, (err) => {
          let com = `report cppAddon/data/${fileName}`
          new Promise ( async (resolve) => {
     
              await client.setInput(com)
          
            resolve()        
            })
            .then( (ans) => res.end()) .catch(err => res.send("no connection to server"))
        })
      }
    }
    // summary {teamA}_{teamB} {user} ({file})
    else if(req.body.command.includes("summary")) {
      let fullCommand = req.body.command.split(" ")
      let teams = fullCommand[1].split("_")
      let summaryToFile = "SUMMARY_" + fullCommand[1]
      client.setInput(req.body.command+" "+ summaryToFile)
      const indexPath = path.join(__dirname, summaryToFile);
      await waitForFileToCreate(indexPath, 3000)
      fs.readFile(indexPath,'utf-8', (err,data) => {
        
        if(err){
          console.log("error: ",err)
          res.send("error occured while reading summary file")
        }
        else{
          if(fullCommand[2] == "@all"){
            res.send(teams[0] +" vs " +teams[1] +" summary \n" + 
                    "-------------------------- \n" + data +
                    "-------------------------- \n");
          }
          
          else {
            res.send(teams[0] +" vs " +teams[1] +" summary from "+fullCommand[2] +"\n" + 
            "-------------------------- \n" + data +
            "-------------------------- \n");
          }
        }
      })
    }
    else {
      new Promise ( (resolve) => {
          client.setInput(req.body.command)
        
      resolve()        
      })
      .then( (ans) => res.end()).catch(err => res.send("no connection to server")) 
    }
})

app.get(`/sentToStompServer`, async (req, res) => {
        const result = await client.getFrameOut((data) => {
            res.send(data);
        })
})

app.get(`/recievedFromStompServer`, async (req, res) => {
    const result = await client.getFrameIn( (data) => {
        res.send(data)
    })
})

const waitForFileToCreate = async (path, timeoutMili) => {
  if(fs.existsSync(path) || timeoutMili <= 0)
    return;
  await new Promise( (resolve) => {
    setTimeout(() => resolve(), 200)
  })
  return waitForFileToCreate(path, timeoutMili-200)
}