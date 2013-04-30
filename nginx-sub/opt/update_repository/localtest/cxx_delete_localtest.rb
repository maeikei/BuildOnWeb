require_relative '../git.conf'
$DELETE_BRANCH = 'guest_from_192_168_0_2'
`ssh #{$G_GIT_HOST} find #{$G_GIT_DIR}/cxx -type d -depth 1 | grep .git`.split(/\n/).each do |reso|
	`mkdir -p temp/`
	`git clone ssh://#{$G_GIT_HOST}#{reso} temp/`
	`cd temp/&& git push origin :#{$DELETE_BRANCH}`
	`rm -rf temp/`
end

