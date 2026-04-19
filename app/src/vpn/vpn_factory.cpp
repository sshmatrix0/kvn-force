//
// Created by ksp on 25.03.2026.
//

#include "vpn_factory.h"

#ifdef Q_OS_WIN
#include "vpn_desktop_service.h"
#elif defined(Q_OS_LINUX)
#include "vpn_desktop_service.h"
#else
#error "Unsupported platform"
#endif

QSharedPointer<AbstractVPN> VPNFactory::createVPN() {
    return QSharedPointer<VPNDesktopService>(new VPNDesktopService());
}
