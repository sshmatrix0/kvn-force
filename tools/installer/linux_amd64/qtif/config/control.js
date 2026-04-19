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
    installer.gainAdminRights();
    installer.performOperation("Execute", ["systemctl", "stop", "KVNForce"]);
    installer.performOperation("Execute", ["systemctl", "disable", "KVNForce"]);
    installer.performOperation("Execute", ["killall", installer.value("TargetDir") + "/app/KVNForce"]);
}