<?php
	$url = "192.168.0.160:3306";
	$user = "root";
	$pass = "ma";
	$db = "BuildOnWeb_db";
	$link = mysql_connect($url,$user,$pass);
	if (!$link) {
		 die("Failed!".mysql_error());
	}
	$sdb = mysql_select_db($db,$link);
	if(!$sdb ) {
		 die("Failed!".mysql_error());
	}
	$sql = "DROP TABLE manual_git_repos";
	$result = mysql_query($sql, $link);
	if (!$result) {
		die("Failed!".mysql_error());
	}
	mysql_close($link);
?>
