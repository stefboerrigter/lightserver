<?php

if(!(isset($_GET['lamp'])) || (!(isset($_GET['status']))))
{
	die("Not all parameters are set (lamp=x&status=y)");
}

$lamp = intval($_GET['lamp']);
$stat = intval($_GET['status']);

$address = '192.168.188.24';
$port = 5005;
$data = sprintf("LIGHT[%04d][%04d]SWITCH\0", $lamp, $stat);

$len = strlen($data);
// Create the socket
if(($sockd = socket_create(AF_INET, SOCK_STREAM,SOL_TCP))<1){
	die("Unable to create socket 1:" . socket_strerror(socket_last_error()));
}
//Connect to Socket
if(socket_connect($sockd,$address,$port) == FALSE){
	die("Unable to connect bbb 1:" . socket_strerror(socket_last_error()));
}

if(($bytes=socket_send($sockd, $data, strlen($data), MSG_WAITALL))==false)
{
	die("Unable to send to socket 1 :" . socket_strerror(socket_last_error()));
}
else
{
	//echo " Send Sockets messages: " . $data . " <br> " ;
}

socket_close($sockd);

echo "Done";
?>

