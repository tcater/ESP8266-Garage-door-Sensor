<link rel="manifest" href="ico/manifest.json">
<meta name="msapplication-TileColor" content="#ffffff">
<meta name="msapplication-TileImage" content="ico/ms-icon-144x144.png">
<meta name="theme-color" content="#ffffff">
	<style>
		body{
			font-family: Arial, Helvetica, sans-serif;
			text-align: center;
			background-color: lightgrey;
			width:700px;
			height:700px;
		}
		.device{
			background-color: darkgrey;
			width: 200px;
			height: 200px;
			border: 2px solid black;
			border-radius: 20px;
			margin:5px;
		}
		.age{
			font-size:1em;
		}
		.title{
			font-size:2em;
		}
		
	</style>
	<script>
		if(typeof(EventSource) !== "undefined") {
			var source = new EventSource("test_sse.php");
			source.onmessage = function(event) {
				
				var data = JSON.parse(event.data);
				eventtime = new Date(data.time-1);
				document.getElementById("state").innerHTML = ("<img src=images/"+data.state+".png>");
				
				getTime();
				
			};
		} else {
			document.getElementById("result").innerHTML = "Sorry, your browser does not support server-sent events...";
		}

		function getTime(){
			var t = Math.floor((new Date()/1000-eventtime+10));
			var seconds = Math.floor( t % 60 );
			var minutes = Math.floor( (t/60) % 60 );
			var hours = Math.floor( (t/(60*60)) % 24 );
			var days = Math.floor( t/(60*60*24) );
			var s = "";
			if(days>0){
				if(days>1){s="s";}
				document.getElementById("age0").innerHTML = days + ' day'+s+' ago';
				setTimeout("getTime()", 7200000);
			}else if(hours>0){
				if(hours>1){s="s";}
				document.getElementById("age0").innerHTML = hours + ' hour'+s+' ago';
				setTimeout("getTime()", 900000);
			}else if(minutes>0){
				if(minutes>1){s="s";}
				document.getElementById("age0").innerHTML = minutes + ' minute'+s+' ago';
				setTimeout("getTime()", 30000);
			}else{
				document.getElementById("age0").innerHTML = seconds + ' seconds ago';
				setTimeout("getTime()", 1000);
			}
		}
	</script>
</head>
<body onload=getTime()>

<?php
function humanTiming ($time)
{
    $time = time() - $time; // to get the time since that moment
    $time = ($time<1)? 1 : $time;
    $tokens = array (
        31536000 => 'year',
        2592000 => 'month',
        604800 => 'week',
        86400 => 'day',
        3600 => 'hour',
        60 => 'minute',
        1 => 'second'
    );

    foreach ($tokens as $unit => $text) {
        if ($time < $unit) continue;
        $numberOfUnits = floor($time / $unit);
        return $numberOfUnits.' '.$text.(($numberOfUnits>1)?'s':'');
    }
}

$file = 'logs/GarageDoor.txt';
if (file_exists($file)) {
	echo ("<div class='device'><div class='title'>Garage Door</div>");
	$data = file($file);
	$line = $data[count($data)-1];
	$state = "";
	echo"<script>
		var eventtime2 = ".filemtime($file).";
	</script>";
	if(strpos($line, 'open') === false){
		$state = "closed";
	} else {
		$state = "open";
	}
	echo ("<div id='state'><img src=images/".$state.".png></div>");
	echo ("<div class='age' id='age0'>".humanTiming(filemtime($file))." ago.</div></div>");
}
?>

</body>
</html>