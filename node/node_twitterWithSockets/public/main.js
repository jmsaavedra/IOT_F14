
$(document).ready(function() {
  //anything that needs to load AFTER the page has loaded...
  console.log("loaded main.js + socket.io");

  // Open a prompt to enter a search term after page loads:
  searchTermPrompt();
});

//initialize socket.io, open socket with server.
var socket= io();


// Whenever the server passes us a 'tweet', show the message
socket.on('tweet', function (data) {
  console.log("received tweet from server: "+data);
  $( '#data' ).append("<p>• "+ data +"</p>"); //add a paragraph for each tweet
  var len = $( "p" ).length;  //number of paragraphs
  if (len > 25) { //if we're showing more than 25 tweets, empty the list
    $("#data").empty();
  }
});


// Whenever you click on the #restart div, open the search prompt
$("#restart").click(function(event){
  // Open a prompt to enter a search term after page loads:
  searchTermPrompt();
});


// Whenever you click on the <img>, send out a 'stop' with value of 1
$("img").click(function(event){
  // Open a prompt to enter a search term after page loads:
  socket.emit('stop', 1);
});


//code for the search term prompt!
var searchTermPrompt = function(){
  socket.emit('stop', 1); //stop the twitterStream command (in case it's running already)
  $( "#restart").css('background-color', '#FF0000');

  var searchTerm = prompt("Enter a search term", "Search term"); //show prompt
  if(searchTerm != null){ //if what we entered isn't empty
    socket.emit('search', searchTerm); //emit a search command with the term
    $( "#restart").css('background-color', '#00FFAA');
    $("#data").empty(); // empty any currently displaying tweets
  }
}
