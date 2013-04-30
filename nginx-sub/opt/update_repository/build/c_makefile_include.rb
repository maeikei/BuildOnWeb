require_relative '../git.conf'
$DELETE_BRANCH = 'guest_from_192_168_0_2'
$BUILD_TYPE='c'
$MAKEFILE="include ./build/#{$BUILD_TYPE}.mk"
`ssh #{$G_GIT_HOST} find #{$G_GIT_DIR}/#{$BUILD_TYPE} -type d -depth 1 | grep .git`.split(/\n/).each do |reso|
	`mkdir -p temp/`
	`git clone ssh://#{$G_GIT_HOST}#{reso} temp/`
	`echo #{$MAKEFILE} > temp/Makefile`
	`cd temp && git commit -am "change makefile to a submodule" && git push`
	`rm -rf temp/`
end

