require_relative '../git.conf'
`ssh #{$G_GIT_HOST} find #{$G_GIT_DIR}/ruby -type d -depth 1 | grep .git`.split(/\n/).each do |reso|
	`mkdir -p temp/`
	`git clone ssh://#{$G_GIT_HOST}#{reso} temp/`
	`cd temp/ && git submodule add ssh://#{$G_GIT_HOST}#{$G_GIT_DIR}/build/build.git build`
	`cd temp/ && git add .gitmodules && git commit -am "add submoduls" && git push`
end

