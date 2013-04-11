<?php
	$man3_tars = shell_exec('find /usr/share/man/man3/ -type f -name *.3.gz');
	$arr_man3s = explode("\n", $man3_tars);
	foreach ($arr_man3s as &$value) {
	//	echo "<p></p>";
	//	echo $value;
		$html_name = "/opt/www/man/" . basename($value,".3.gz") . ".html";
	//	echo $html_name;
		$cmd = 'zcat ' . $value . '| groff -mandoc -Thtml';
		$out_html = shell_exec($cmd);
	//	echo $out;
	//	echo $html_name;
		$fp = fopen($html_name, 'w');
		fwrite($fp, $out_html);
		fclose($fp);
	}
	$man3_htmls = shell_exec('find /opt/www/man/ -type f -name *.html');
	echo '<pre>' . $man3_htmls . '</pre>'
?>
