var express = require('express')
var app = express()
var http = require('http')
var request = require('request')
var fs = require('fs')
var bodyparser = require('body-parser')

//define the base url and API key
base_url = "https://maps.googleapis.com/maps/api/directions/json?"
API_KEY = "AIzaSyBV6hBZ1SWDO8lSAUNpiltBMdrYxoikz3Q"

app.use(bodyparser.json({ type: 'application/json' }));

//make a request to directions api
var make_request = function(origin, destination, mode, res){
	URL = base_url + "origin=" + origin.replace(" ","+") + "&destination=" + destination.replace(" ", "+") + "&mode=" + mode + "&key=" + API_KEY
	request(URL, function(error, response, body){
		if(!error){
			body = JSON.parse(body);
			steps = body["routes"][0]["legs"][0]["steps"]
			num_steps = steps.length
			turn_directions = []
			distance_to_travel = []
			duration_to_travel = []
			end_loc_latitude = []
			end_loc_longitude = []
			var destination_loc = ""
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
				if(i==(num_steps-1)){
					preString = "Destination"
  					preIndex = steps[i]["html_instructions"].indexOf(preString)
  					preString = steps[i]["html_instructions"].substring(preIndex)
  					if(preString.indexOf("left") > -1){
  						destination_loc = "left"
  					}
  					else if (preString.indexOf("right") > -1){
  						destination_loc = "right"
  					}
				}
				distance_to_travel[i] = steps[i]["distance"]["value"]
				duration_to_travel[i] = steps[i]["duration"]["value"]
				end_loc_latitude[i] = steps[i]["end_location"]["lat"]
				end_loc_longitude[i] = steps[i]["end_location"]["lng"]	
			}
			console.log(turn_directions)
			console.log(distance_to_travel)
			console.log(duration_to_travel)
			console.log(destination_loc)
			var turn_directions_string = ""
			var distance_string = ""
			var duration_string = ""
			var end_loc_string = ""

			for(var j=0; j < turn_directions.length; j++){
				turn_directions_string += turn_directions[j] + ","
				distance_string += distance_to_travel[j] + ","
				duration_string += duration_to_travel[j] + ","
				end_loc_string += end_loc_latitude[j] + ";" + end_loc_longitude + ","
			}
			var masterString = turn_directions_string + "|" + distance_string + "|" + duration_string + "|" + end_loc_string + "|" + destination_loc
			res.end(masterString)
		}
		else{
			res.send(500, "Couldn't complete request")
		}
	})
}

// Our handler function is passed a request and response object
app.post('/', function(req, res) {
  if(req.body.walkity_key == "walkity786"){
  	setTimeout(function(){
  		make_request(req.body.origin, req.body.destination, req.body.mode, res)
  	},10)
  }
  else{
  	res.send(500,"Failed Request")
  }
});

app.listen(process.env.PORT || 8000)
console.log("App listening on port 8000")