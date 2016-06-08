#!/usr/local/bin/node

// module info
var http 		= require('http');
var express 	= require('express');
var cookieParser = require('cookie-parser');
var mongo		= require('mongodb').MongoClient;
var sys			= require('sys');
var app 		= express();


// database info
var db_name = 'CAPSTONEDESIGN1';
var db_coll = 'DATA_HISTORY';
var db_conn;

app.use(cookieParser());

function getFormattedTime()
{
	// Requested Information
	var today   = new Date();

	var year    = '' + today.getFullYear();
	var month   = '' + (today.getMonth() + 1);
	var day     = '' + today.getDate();
    
    var hour    = '' + today.getHours();
    var min     = '' + today.getMinutes();
    var sec     = '' + today.getSeconds();
	
	if(month.length < 2)    month = '0' + month;
	if(day.length   < 2)    day   = '0' + day;

	if(hour.length  < 2)    hour  = '0' + hour;
	if(min.length   < 2)    min   = '0' + min;
	if(sec.length   < 2)    sec   = '0' + sec;

	return [year, month, day].join('-') + ' ' + [hour, min, sec].join(':');
}

app.get('/update', function(req, res) {

	var humidity        = req.query['humvalue'];
	var temperature     = req.query['tempvalue'];
	var lightIntensity  = req.query['lightvalue'];

    console.log(humidity, temperature, lightIntensity);

    datestr = getFormattedTime();

    db_conn.collection(db_coll, function(err, history) {
        history.insert({
            date:               datestr,
            humidity:           humidity,
            temperature:        temperature,
            lightIntensitiy:    lightIntensity
        });

    });
});
								
http.createServer(app).listen(8080, function() {
	console.log('[' + getFormattedTime() + ']' +  ' Server Running at http://localhost:8080');
    mongo.connect("mongodb://localhost/", function(err, db) {
        console.log('Database "' + db_name + '" connected!');
        db_conn = db.db(db_name);
    });
});
