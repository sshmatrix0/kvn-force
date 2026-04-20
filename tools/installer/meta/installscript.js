function Component() {
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
}

Component.prototype.installationFinishedPageIsShown = function () {
    if (systemInfo.productType === "windows") {
        installationFinishedPageIsShownWindows();
    } else {
        installationFinishedPageIsShownLinux();
    }
};
function installationFinishedPageIsShownWindows() {
    if (installer.isInstaller() && installer.status === QInstaller.Success) {
        var appPath = installer.value("TargetDir") + "\\app\\KVNForce";
        installer.executeDetached(appPath, "");
    }
}
function installationFinishedPageIsShownLinux() {
    if (installer.isInstaller() && installer.status === QInstaller.Success) {
        var appPath = installer.value("TargetDir") + "/app/KVNForce.exe";
        installer.executeDetached(appPath, "");
    }
}

Component.prototype.createOperations = function () {
    component.createOperations();

    if (systemInfo.productType === "windows") {
        createOperationWindows();
    } else {
        createOperationsLinux();
    }

}
function createOperationWindows() {
    var sedCommand = "powershell -Command \"(Get-Content '" + installer.value("TargetDir") + "\\service\\KVNForceService.xml') -replace '" + "%APP_DIR%" + "', '" + installer.value("TargetDir") + "' | Set-Content '" + installer.value("TargetDir") + "\\service\\KVNForceService.xml'\"";
    component.addElevatedOperation("Execute", "powershell", "-Command", sedCommand);

    component.addElevatedOperation("Execute", installer.value("TargetDir") + "\\service\\KVNForceService.exe", "install");
    component.addElevatedOperation("Execute", installer.value("TargetDir") + "\\service\\KVNForceService.exe", "start");
    var target = "@TargetDir@\\app\\KVNForce.exe";
    var shortcut = "@StartMenuDir@\\KVNForce.lnk";
    component.addOperation(
        "CreateShortcut",
        target,
        shortcut,
        "workingDirectory=@TargetDir@\\app\\",
        "iconPath=@TargetDir@\\app\\app_icon.ico",
        "iconId=0",
        "description=KVN Force"
    );

}
function createOperationsLinux() {
    component.addElevatedOperation("Execute", "apt", "install", "-y", "libxcb-cursor0");
    var sedCommand = "sed -i 's|" + "%APP_DIR%" + "|" + installer.value("TargetDir") + "|' " + installer.value("TargetDir") + "/KVNForce.service";
    component.addElevatedOperation("Execute", "sh", "-c", sedCommand);
    component.addElevatedOperation("Copy", installer.value("TargetDir") + "/KVNForce.service", "/etc/systemd/system/KVNForce.service");

    component.addElevatedOperation("Execute", "chmod", "ugo+xr", "/etc/systemd/system/KVNForce.service");
    component.addElevatedOperation("Execute", "systemctl", "start", "KVNForce");
    component.addElevatedOperation("Execute", "systemctl", "enable", "KVNForce");

    try {
        var desktopPath = QDesktopServices.storageLocation(0);
        console.log("Desktop path:", desktopPath);
    } catch (e) {
        var desktopPath = installer.environmentVariable("HOME") + "/Desktop";
        console.log("Fallback desktop path:", desktopPath);
    }
    var desktopLinkPath = desktopPath + "/KVNForce.desktop";
    var desktopFileContent =
        "[Desktop Entry]\n" +
        "Version=1.0\n" +
        "Type=Application\n" +
        "Name=KVNForce\n" +
        "Comment=VPN Client Light\n" +
        "Exec=" + installer.value("TargetDir") + "/app/KVNForce\n" +
        "Icon=" + installer.value("TargetDir") + "/app_icon.png\n" +
        "Terminal=false\n" +
        "Categories=Network;Utility;\n";

    component.addOperation("CreateDesktopEntry", desktopLinkPath, desktopFileContent);

    component.addOperation("Execute", "chmod", "+x", desktopLinkPath);

    component.addOperation("Execute", "cp", desktopLinkPath, installer.environmentVariable("HOME") + "/.config/autostart/");

}