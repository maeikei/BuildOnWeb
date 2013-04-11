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
	$sql = "CREATE TABLE manual_git_repos
	(
		name varchar(256),
		repo varchar(256),
		url  varchar(256)
	)";
	$result = mysql_query($sql, $link);
	if (!$result) {
		die("Failed!".mysql_error());
	}
	mysql_free_result($result);

	$man3_htmls = shell_exec('cd /opt/www/man/ && ls *.html');
	$arr_man3s = explode("\n", $man3_htmls);
	foreach ($arr_man3s as &$value) {
		$man_name =  basename($value,".html");
		// echo $man_name;
		$sql = "INSERT INTO manual_git_repos (name,repo,url) 
				VALUES ('".$man_name."','" . $man_name."','".$value."')";
		$result = mysql_query($sql, $link);
		if (!$result) {
			die("Failed!".mysql_error());
		}
		//
		// create git repositories
		$git_homw ='/opt/BuildOnWeb/repositories/'.$man_name.'.git';
		$cmd = 'mkdir -p '.$git_homw.' && cd '.$git_homw.'&& git init --bare --share=true';
		shell_exec($cmd);
//		mysql_free_result($result);
	}
	$sql = "SELECT * FROM manual_git_repos " ;
 	if (!$result) {
                die("Failed!".mysql_error());
        }
 	mysql_free_result($result);
	mysql_close($link);
?>
