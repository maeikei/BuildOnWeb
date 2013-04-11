<?php
	$man3_htmls = shell_exec('cd /opt/www/man/ && ls *.html');
	echo '<pre>' . $man3_htmls . '</pre>';
	$arr_man3s = explode("\n", $man3_htmls);
	foreach ($arr_man3s as &$value) {
		$man_name =  basename($value,".html");
		echo $man_name;
	}
?>
