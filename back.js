var express = require('express')
var app = express()
var http = require('http')
var request = require('request')
var fs = require('fs')
var bodyparser = require('body-parser')

//define the base url and API key
base_url = "https://maps.googleapis.com/maps/api/directions/json?"
API_KEY = "AIzaSyBV6hBZ1SWDO8lSAUNpiltBMdrYxoikz3Q"

//will later be received by android app
origin = "Dwinelle Hall, Berkeley"
destination = "2520 College Avenue, Berkeley"
mode = "walking"

//derive the entire url
URL = base_url + "origin=" + origin.replace(" ","+") + "&destination=" + destination.replace(" ", "+") + "&mode=" + mode + "&key=" + API_KEY

//make a request to directions api
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

	}
	else{
		console.log("failed")
	}
})