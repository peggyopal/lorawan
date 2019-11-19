/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 University of Padova
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 *         Martina Capuzzo <capuzzom@dei.unipd.it>
 */

#include "ns3/end-device-c-lorawan-mac.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/end-device-lora-phy.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <algorithm>

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("EndDeviceCLorawanMac");

NS_OBJECT_ENSURE_REGISTERED (EndDeviceCLorawanMac);

TypeId
EndDeviceCLorawanMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EndDeviceCLorawanMac")
    .SetParent<EndDeviceLorawanMac> ()
    .SetGroupName ("lorawan")
    .AddConstructor<EndDeviceCLorawanMac> ();
  return tid;
}

EndDeviceCLorawanMac::EndDeviceCLorawanMac ()
{
  NS_LOG_FUNCTION (this);
}

EndDeviceCLorawanMac::~EndDeviceCLorawanMac ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
EndDeviceCLorawanMac::SetMType (LorawanMacHeader::MType mType)
{
  m_mType = mType;
  NS_LOG_DEBUG ("Message type is set to " << mType);
}

}
}
