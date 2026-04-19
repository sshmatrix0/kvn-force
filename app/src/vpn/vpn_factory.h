//
// Created by ksp on 25.03.2026.
//

#pragma once
#include  <QSharedPointer>
#include "abstract_vpn.h"
class VPNFactory {
public:
    static QSharedPointer<AbstractVPN> createVPN();
};
