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
// read from db again 
        $sql = "SELECT * FROM user_acount" ;
        $result = mysql_query($sql, $link);
        if (!$result) {
                die("Failed!".mysql_error());
        }
        while ($row = mysql_fetch_row($result)) {
		echo "<p>";
//		echo 'id=##' ;
//		echo  $row[0];
//		echo '##';
		echo 'ip=##';
		echo  $row[1];
		echo  '##';
		echo 'name=##';
		echo  $row[2];
		echo  '##';
		echo "</p>";
	}
	mysql_free_result($result);
	mysql_close($link);
?>

