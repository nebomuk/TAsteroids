

function dp(dpVal) {
var px = Math.round(dpVal * (screenPixelDensity / 160));

if(Qt.platform.os == "windows" || Qt.platform.os == "mac" || Qt.platform.os == "linux")
    return px*2;
else
    return px;
}
