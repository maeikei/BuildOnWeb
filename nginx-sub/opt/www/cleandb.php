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
	$sql = "DROP TABLE user_acount";
	$result = mysql_query($sql, $link);
	if (!$result) {
		die("Failed!".mysql_error());
	}
	mysql_free_result($result);
        $sql = "SHOW TABLES LIKE 'user_acount'";
        $result = mysql_query($sql, $link);
        if (!$result) {
                die("Failed!".mysql_error());
        }
	print $result;
        mysql_free_result($result);
	echo "succes delete table user_acount";
	mysql_close($link);
?>
