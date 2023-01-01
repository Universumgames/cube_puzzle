ARCHITECTURE=$(uname -m | tr -d '\n')
GIT_HASH=$(git log -1 --format=%h)
OS=$(/bin/bash osname.sh)

source extractXCodeData.sh

extractBuildSettings 2> /dev/null

replaceContent(){
    content=$1
    toReplace=$2
    replaceWith=$3
    #echo $(echo "$content" | sed "s/$toReplace/$replaceWith/g")
    #echo $(sed "s/$toReplace/$replaceWith/g" <<< "$content")
    echo ${content//$toReplace/$replaceWith}
}


CONFIG_IN=$(cat ../src/config.hpp.in | tr '\n' '!')
echo "$CONFIG_IN"
CONFIG_1=$(replaceContent "$CONFIG_IN" "@CMAKE_PROJECT_VERSION@" "${marketingVersion}build${buildVersion}")
echo "$CONFIG_1"
CONFIG_2=$(replaceContent "$CONFIG_1" "@GIT_HASH@" $GIT_HASH)
echo "$CONFIG_2"
CONFIG_3=$(replaceContent "$CONFIG_2" "@ARCHITECTURE@" $ARCHITECTURE)
echo "$CONFIG_3"
CONFIG_4=$(replaceContent "$CONFIG_3" "@CMAKE_SYSTEM_NAME@" $OS)
echo "$CONFIG_4"

CONFIG_OUT=$(echo "$CONFIG_4" | tr '!' '\n')
echo "$CONFIG_OUT"

mkdir -p prepared

echo "$CONFIG_OUT" > prepared/config.hpp

echo "Version: XCode
Commit: ${GIT_HASH}
Architecture: ${ARCHITECTURE}
System: ${OS}
" > appInfo

#EOF
