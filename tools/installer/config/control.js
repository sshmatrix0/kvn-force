// config/control.js

function Controller() {
    if (installer.isUninstaller()) {
        installer.uninstallationStarted.connect(this, this.onUninstallationStarted);
    } else {
        //installer.setDefaultPageVisible(QInstaller.Introduction, false);
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
        //installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
        installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
    }
}


Controller.prototype.onUninstallationStarted = function () {
    if (systemInfo.productType === "windows") {
        onUninstallationStartedWindows();
    } else {
        onUninstallationStartedLinux();
    }
}

function onUninstallationStartedWindows() {
    installer.gainAdminRights();
    installer.performOperation("Execute", ["taskkill", "/im", "KVNForce.exe"]);
    installer.performOperation("Execute", [installer.value("TargetDir") + "\\service\\KVNForceService.exe", "stop"]);
    installer.performOperation("Execute", [installer.value("TargetDir") + "\\service\\KVNForceService.exe", "uninstall"]);

    var registryPath = "HKEY_CURRENT_USER\\Software\\org.sshmatrix\\KVNForce";
    installer.execute("reg", ["delete", registryPath, "/f"]);
}
function onUninstallationStartedLinux() {
    installer.gainAdminRights();
    installer.performOperation("Execute", ["systemctl", "stop", "KVNForce"]);
    installer.performOperation("Execute", ["systemctl", "disable", "KVNForce"]);
    installer.performOperation("Execute", ["killall", installer.value("TargetDir") + "/app/KVNForce"]);
}
