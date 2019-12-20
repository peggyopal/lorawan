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
 * Author: Peggy Anderson <peggy.anderson@usask.ca>
 */

#include "ns3/class-c-end-device-lorawan-mac.h"
#include "ns3/log.h"

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("ClassCEndDeviceLorawanMac");

NS_OBJECT_ENSURE_REGISTERED (ClassCEndDeviceLorawanMac);

TypeId
ClassCEndDeviceLorawanMac::GetTypeId (void)
{
static TypeId tid = TypeId ("ns3::ClassCEndDeviceLorawanMac")
  .SetParent<EndDeviceLorawanMac> ()
  .SetGroupName ("lorawan")
  .AddConstructor<ClassCEndDeviceLorawanMac> ();
return tid;
}

ClassCEndDeviceLorawanMac::ClassCEndDeviceLorawanMac () :
  // LoraWAN default
  m_receiveDelay1 (Seconds (1)),
  // LoraWAN default
  m_receiveDelay2 (Seconds (2)),
  m_rx1DrOffset (0)
{
  NS_LOG_FUNCTION (this);

  // Void the two receiveWindow events
  m_openFirstWindow = EventId ();
  m_openFirstWindow.Cancel ();
  m_closeFirstWindow = EventId ();
  m_closeFirstWindow.Cancel ();
  m_closeSecondWindow = EventId ();
  m_closeSecondWindow.Cancel ();
  m_secondReceiveWindow = EventId ();
  m_secondReceiveWindow.Cancel ();
}

ClassCEndDeviceLorawanMac::~ClassCEndDeviceLorawanMac ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

//////////////////////////
//  Receiving methods   //
//////////////////////////

void
ClassCEndDeviceLorawanMac::Receive (Ptr<Packet const> packet)
{
  NS_LOG_FUNCTION (this << packet);

  NS_LOG_DEBUG ("Receive");
}

void
ClassCEndDeviceLorawanMac::FailedReception (Ptr<Packet const> packet)
{
  NS_LOG_FUNCTION (this << packet);

  NS_LOG_DEBUG ("FailedReception");
}

void
ClassCEndDeviceLorawanMac::TxFinished (Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);

  NS_LOG_DEBUG ("TxFinished");
}

void
ClassCEndDeviceLorawanMac::OpenFirstReceiveWindow ()
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_LOG_DEBUG ("OpenFirstReceiveWindow");
}

void
ClassCEndDeviceLorawanMac::CloseFirstReceiveWindow ()
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_LOG_DEBUG ("CloseFirstReceiveWindow");
}

void
ClassCEndDeviceLorawanMac::OpenSecondReceiveWindow ()
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_LOG_DEBUG ("OpenSecondReceiveWindow");
}

void
ClassCEndDeviceLorawanMac::CloseSecondReceiveWindow ()
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_LOG_DEBUG ("CloseSecondReceiveWindow");
}

/////////////////////////
// Getters and Setters //
/////////////////////////

void
ClassCEndDeviceLorawanMac::SetSecondReceiveWindowDataRate (uint8_t dataRate)
{
  m_secondReceiveWindowDataRate = dataRate;
}

void
ClassCEndDeviceLorawanMac::SetSecondReceiveWindowFrequency (double frequencyMHz)
{
  m_secondReceiveWindowFrequency = frequencyMHz;
}

} /* namespace lorawan */
} /* namespace ns3 */
