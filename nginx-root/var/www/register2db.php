<html>
<body>
<?php
	//var_dump($_POST);
	$reguser=$_POST["regname"];
	$regemail=$_POST["regemail"];
	$regpass1=md5($_POST["regpass1"]);
	$regpass2=md5($_POST["regpass2"]);
	//echo $reguser;
	//echo $regemail;
	//echo $regpass1;
	//echo $regpass2;
	if ($regpass1 != $regpass2)
	{
		error_return(__LINE__);
	}
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
	$sql = "SELECT * FROM user_acount WHERE name='" . $reguser . "'" ;
	$result = mysql_query($sql, $link);
	$user_not_exist = false;
	if ($result) {
		if( ! ($row = mysql_fetch_row($result))) {
			$user_not_exist = true;
		}
		else
		{
			 error_return(__LINE__);
		}
	}
//	var_dump( $user_not_exist);
	if($user_not_exist)
	{
		$remote_ip = $_SERVER['HTTP_X_REAL_IP'];
		if(empty($remote_ip))
		{
			$remote_ip = $_SERVER['REMOTE_ADDR'];
		}
		//		echo "insert table!";
	        $userid = md5( $reguser );
//        	echo $userid;
		$sql = "INSERT INTO user_acount (ID,RemoteIp,name,PASSWORD,APP_URL) 
				VALUES ('". $userid."','" . $remote_ip."','" .$reguser . "','" . $regpass1 . "','c_cxx/helloworld/main.c')";
		$result2 = mysql_query($sql, $link);
		if (!$result2) {
			die("Failed!".mysql_error());
		}
		mysql_free_result($result2);
		sucess_return();

	}
	mysql_close($link);

function error_return($line){
	echo "Error!!". $line;
	header("Location: /register.php");
}
function sucess_return(){
}
?>
<p>Sucess</p>
</body>
</html>
