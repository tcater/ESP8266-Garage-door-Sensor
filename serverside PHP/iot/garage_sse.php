<?php
header('Content-Type: text/event-stream');
header('Cache-Control: no-cache');

$file = 'logs/GarageDoor.txt';
$state = "";
if (file_exists($file) && filemtime($file) - time() > -10) {
$data = file($file);
$line = $data[count($data)-1];

	if(strpos($line, 'open') === false){
		$state = "closed";
	} else {
		$state = "open";
	}
	echo "data: {\"state\":\"".$state."\",\"time\":\"".filemtime($file)."\"}\n\n";
	
	flush();
}

?>