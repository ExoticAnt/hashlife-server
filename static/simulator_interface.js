"use strict";

var timeout;

var simulation = null;

var grid_color = "#CCCCCC"; 
var live_color = "#3333EE";
var dead_color = "#EEEE00";

function new_simulation() {
    var xhttp = new XMLHttpRequest();
    
    if(timeout) clearTimeout(timeout); 

    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //document.getElementById("marc").innerHTML = xhttp.responseText;
			simulation = JSON.parse(xhttp.responseText);
			display_universe(simulation, "display_canvas");
        }
    };
    xhttp.open("GET", "/simulation/new", true);
    xhttp.send();
}

function run_simulation() {
    var xhttp = new XMLHttpRequest();
	
	if(simulation == null) return;
    
    if(timeout) clearTimeout(timeout); 

    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //document.getElementById("marc").innerHTML = xhttp.responseText;
			simulation = JSON.parse(xhttp.responseText);
			display_universe(simulation, "display_canvas");
        }
    };
	xhttp.open("GET", "/simulation/run/" + simulation["simulation_id"], true);
    xhttp.send();
}

function run_continuous_simulation() {
	setInterval(run_simulation, 400);
}

function display_universe(simulation, container_id) {
	const canvas = document.getElementById("display_canvas");
	const ctx = canvas.getContext("2d");
	
	const width = canvas.width;
	const height = canvas.height;
	
	var cell_x = width/simulation["cols"];
	var cell_y = width/simulation["rows"];
	
	ctx.fillStyle = grid_color;
	
	ctx.fillRect(0,0,width,height);
	
	let data = simulation["data"];
	var i, j=0;
	for(i=0; i<simulation["rows"]; i++) {
		for(j=0; j<simulation["cols"]; j++) {
			if(data[i][j] > 0) {
				ctx.fillStyle = live_color;
			} else {
				ctx.fillStyle = dead_color;
			}
			ctx.fillRect(j*cell_x, i*cell_y, cell_x, cell_y);
		}
	}
	
}