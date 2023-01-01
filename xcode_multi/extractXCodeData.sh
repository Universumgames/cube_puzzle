
extractBuildSettings(){
    xcodeProjDir=$(find . -maxdepth 1 -type d | grep ".xcodeproj")

    if [[ ! (-n "$xcodeProjDir") ]]; then
        echo "${RED}This is not an XCode project${WHITE}"
        return
    fi

    echo "${GREEN}Found Xcode Project Folder: $xcodeProjDir${WHITE}"

    pbxprojFile="$xcodeProjDir/project.pbxproj"

    if [[ ! (-f "$pbxprojFile") ]]; then
        echo "${RED}Could not find file $pbxprojFile${WHITE}"
        return
    fi

    echo "${GRAY}Extracting Buildsettings from XCode Project${WHITE}"
    
    buildSettings=$(xcodebuild -showBuildSettings 2> /dev/null)

    if [[ ! (-n "$buildSettings")]]; then
        echo "${RED}Could not obtain data from project structure${WHITE}"
        return
    fi
    
    marketingVersion=$(echo "$buildSettings" | grep MARKETING_VERSION | sed 's/MARKETING_VERSION = //g' | sed 's/ //g')
    buildVersion=$(echo "$buildSettings" | grep CURRENT_PROJECT_VERSION | sed 's/CURRENT_PROJECT_VERSION = //g' | sed 's/ //g')
    bundleID=$(echo "$buildSettings" | grep PRODUCT_BUNDLE_IDENTIFIER | sed 's/PRODUCT_BUNDLE_IDENTIFIER = //g' | sed 's/ //g')
    team=$(echo "$buildSettings" | grep DEVELOPMENT_TEAM | sed 's/DEVELOPMENT_TEAM = //g' | sed 's/ //g')
    scheme=$(echo "$buildSettings" | grep TARGETNAME | sed 's/TARGETNAME = //g' | sed 's/ //g')

    return "$buildSettings"
}
