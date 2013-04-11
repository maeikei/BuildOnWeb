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
            ID varchar(32),
			RemoteIp varchar(32),
            name varchar(128),
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
	        $userid = md5( $remote_ip . "guest" );
//        	echo $userid;
		$sql = "INSERT INTO user_acount (ID,RemoteIp,name,PASSWORD,APP_URL) VALUES ('". $userid."','" . $remote_ip."','guest','','c_cxx/helloworld/main.c')";
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
		$userid = $row[2];
		$last_app = $row[4];
	}
	mysql_free_result($result);
//	echo $userid;
//	echo $last_app;
	mysql_close($link);
	if($_SERVER['HTTP_X_REAL_IP'])
	{
		$redirect_url= "Location: http://" . $_SERVER['HTTP_HOST'] . "/users/".$userid."/".$last_app;
	}
	else
	{
		$redirect_url= "Location: http://" . $_SERVER['SERVER_ADDR'] . "/users/".$userid."/".$last_app;
	}
//	echo $redirect_url . "\n";
	header($redirect_url)
?>
