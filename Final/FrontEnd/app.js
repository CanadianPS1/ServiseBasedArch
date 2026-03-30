const express = require('express');
const app = express();
const PORT = 9001;
app.set("view engine", "ejs");
app.use(express.static("public"));
app.use(express.json());
app.use(express.urlencoded());
app.get("/",  async (req, res) => {
    res.render("home"); 
})