<html>
<body>
<?php
	//var_dump($_POST);
	$user=$_POST["user"];
	$password=md5($_POST["password"]);
//	echo $user;
//	echo $password;
	
	$url = "192.168.0.160:3306";
	$dbuser = "root";
	$pass = "ma";
	$db = "BuildOnWeb_db";
	$link = mysql_connect($url,$dbuser,$pass);
	if (!$link) {
		 die("Failed!".mysql_error());
	}
	$sdb = mysql_select_db($db,$link);
	if(!$sdb ) {
		 die("Failed!".mysql_error());
	}
	$sql = "SELECT * FROM user_acount WHERE name='" . $user . "'" ;
	$result = mysql_query($sql, $link);
	if (!$result) {
		error_return(__LINE__);
	}
	else
	{
		$row = mysql_fetch_row($result);
		if($row)
		{
			var_dump($row);
			var_dump($password);
			if($password == $row[3])
			{
				$redirect_to = "/users/" . $row[2] . "/" . $row[4];
				sucess_return($redirect_to);
			}
			else
			{
				error_return(__LINE__);
			}
		}
		else
		{
			error_return(__LINE__);
		}
	}
	mysql_close($link);

function error_return($line){
	echo "Error!!at " . $line;
	header("Location: /login.php");
}
function sucess_return($url){
        echo "sucess!! go to " . $url;
	header("Location: " . $url);
}
?>
</body>
</html>
