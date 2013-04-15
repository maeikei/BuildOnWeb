<?php
	$man3_repos = shell_exec('find /opt/BuildOnWeb/repositories -type d -name *.git');
	$arr_man3s = explode("\n", $man3_repos);
$Makefile=
"HOME  := /opt/llvm/bin
CLANG := $(HOME)/clang
LLI   := $(HOME)/lli

CLANGFLAGS += -pthread


.PHONY: build clean exe 
build:bow_output
	$(CLANG) -c -emit-llvm  $(CLANGFLAGS) main.c -o.bow_output/main.bc
clean:
	rm -rf bow_output/
exe:build
	$(LLI) .bow_output/main.bc < ./stdin.txt | tee .bow_output/output.log

bow_output:
	mkdir -p .bow_output/";
$Source =
"int main(int argc ,char* argv[])
{
	return 0;
}";

	foreach ($arr_man3s as &$value) {
		if( !$value ){
			break;
		}
		$cmd = 'git clone ' . $value . ' temp_dir';
		$result = shell_exec($cmd);
		echo $result;
		$fp = fopen('temp_dir/Makefile', 'w');
		fwrite($fp, $Makefile);
		fclose($fp);
                $fp = fopen('temp_dir/main.c', 'w');
                fwrite($fp, $Source);
                fclose($fp);

		$cmd = 'cd temp_dir && touch stdin.txt';
		$result = shell_exec($cmd);
		echo $result;
		$cmd = 'cd temp_dir && git add stdin.txt';
		$result = shell_exec($cmd);
		echo $result;
		$cmd = 'cd temp_dir && git add Makefile';
		$result = shell_exec($cmd);
		echo $result;
                $cmd = 'cd temp_dir && git add main.c';
                $result = shell_exec($cmd);
                echo $result;

		$cmd = 'cd temp_dir && git commit -am "first"';
		$result = shell_exec($cmd);
		echo $result;
		$cmd = 'cd temp_dir && git push origin master';
		$result = shell_exec($cmd);
		echo $result;
		$cmd = 'rm -rf temp_dir';
		$result = shell_exec($cmd);
		echo $result;
	}
?>
