<?php
	$url = "192.168.0.150";
	$user = "root";
	$pass = "ma";
	$db = "BuildOnWeb.db";
	$link = mysql_connect($url,$user,$pass) or die("Failed! mysql connect");
	$sdb = mysql_select_db($db,$link) or die("Failed! select db");
    $redirect_url= "Location: http://" . $_SERVER['SERVER_ADDR'] . "/users/guest";
    echo $redirect_url . "\n";
    echo $_SERVER['HTTP_HOST'] . "\n";
    echo $_SERVER['HTTP_X_REAL_IP'] . "\n";
//    header($redirect_url)
?>
