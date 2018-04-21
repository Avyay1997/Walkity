var express = require('express')
var app = express()
var http = require('http')
var request = require('request')
var fs = require('fs')
var bodyparser = require('body-parser')

//define the base url and API key
base_url = "https://maps.googleapis.com/maps/api/directions/json?"
API_KEY = "AIzaSyBV6hBZ1SWDO8lSAUNpiltBMdrYxoikz3Q"

//make a request to directions api
var make_request = function(origin, destination, mode){
	URL = base_url + "origin=" + origin.replace(" ","+") + "&destination=" + destination.replace(" ", "+") + "&mode=" + mode + "&key=" + API_KEY
	request(URL, function(error, response, body){
		if(!error){
			body = JSON.parse(body);
			steps = body["routes"][0]["legs"][0]["steps"]
			num_steps = steps.length
			turn_directions = []
			distance_to_travel = []
			duration_to_travel = []
			console.log(steps)
			for(var i=0; i < num_steps; i++){
				if (i==0){
					if (steps[i]["html_instructions"].indexOf("east") > -1){
						turn_directions[i] = "east"
					}
					else if (steps[i]["html_instructions"].indexOf("west") > -1){
						turn_directions[i] = "west"
					}
					else if (steps[i]["html_instructions"].indexOf("north") > -1){
						turn_directions[i] = "north"
					}
					else if (steps[i]["html_instructions"].indexOf("south") > -1){
						turn_directions[i] = "south"
					}
				}
				else{
					turn_directions[i] = steps[i]["maneuver"]
				}
				distance_to_travel[i] = steps[i]["distance"]["value"]
				duration_to_travel[i] = steps[i]["duration"]["value"]
			}
			console.log(turn_directions)
			console.log(distance_to_travel)
			console.log(duration_to_travel)
			var turn_directions_string = ""
			var distance_string = ""
			var duration_string = ""

			for(var j=0; j < turn_directions.length; j++){
				turn_directions_string += turn_directions[j] + ","
				distance_string += distance_to_travel[j] + ","
				duration_string += duration_to_travel[j] + ","
			}
			var masterString = turn_directions_string + "|" + distance_string + "|" + duration_string
			console.log(masterString)
		}
		else{
			console.log("failed")
		}
	})
}

// Our handler function is passed a request and response object
app.get('/', function(req, res) {
  console.log(req)
  res.end();
});

app.listen(8000)