require_relative '../git.conf'
`ssh #{$G_GIT_HOST} find #{$G_GIT_DIR}/cxx -type d -depth 1 | grep .git`.split(/\n/).each do |reso|
	`mkdir -p temp/`
	`git clone ssh://#{$G_GIT_HOST}#{reso} temp/`
	`cd temp/ && git submodule init && git submodule update`
	`rm -rf temp/`
end

