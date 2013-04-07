<?php
	$url = "127.0.0.1:3306";
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
	$sql = "SHOW TABLES LIKE 'user_acount'";
	$result = mysql_query($sql, $link);
	if (!$result) {
		die("Failed!".mysql_error());
	}
	$user_accout_table_exist = false;
	while ($row = mysql_fetch_row($result)) {
		$user_accout_table_exist = true;
	}
	mysql_free_result($result);
	if (!$user_accout_table_exist) {
		$sql = "CREATE TABLE user_acount
		(
			RemoteIp varchar(32),
			ID varchar(128),
			PASSWORD varchar(16),
			APP_URL varchar(256)
		)";
		$result = mysql_query($sql, $link);
		if (!$result) {
                	die("Failed!".mysql_error());
        	}
		mysql_free_result($result);
	}
	$remote_ip = $_SERVER['HTTP_X_REAL_IP'];
	if(empty($remote_ip))
	{
		$remote_ip = $_SERVER['REMOTE_ADDR'];
	}
//	echo $remote_ip . "\n";
	$sql = "SELECT * FROM user_acount WHERE RemoteIp='" . $remote_ip . "'" ;
	$result = mysql_query($sql, $link);
	if (!$result) {
		die("Failed!".mysql_error());
	}
	$row = mysql_fetch_row($result);
	if( !$row )
	{
//		echo "insert table!";
		$sql = "INSERT INTO user_acount (RemoteIp,ID,PASSWORD,APP_URL) VALUES ('". $remote_ip."','guest','','c_cxx/helloworld/main.c')";
		$result2 = mysql_query($sql, $link);
		if (!$result2) {
			die("Failed!".mysql_error());
		}
		mysql_free_result($result2);
	}
	mysql_free_result($result);
// read from db again 
        $sql = "SELECT * FROM user_acount WHERE RemoteIp='" . $remote_ip . "'" ;
        $result = mysql_query($sql, $link);
        if (!$result) {
                die("Failed!".mysql_error());
        }
        $row = mysql_fetch_row($result);
	if ( $row ) {
		$userid = $row[1];
		$last_app = $row[3];
	}
	mysql_free_result($result);
//	echo $userid;
//	echo $last_app;
	mysql_close($link);
	$redirect_url= "Location: http://" . $_SERVER['SERVER_ADDR'] . "/users/id=".$userid."/app=".$last_app;
	echo $redirect_url . "\n";
//	echo $_SERVER['HTTP_HOST'] . "\n";
//	echo $_SERVER['HTTP_X_REAL_IP'] . "\n";
	header($redirect_url)
?>
