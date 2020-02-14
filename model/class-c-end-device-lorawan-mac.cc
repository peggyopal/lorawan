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
#include "ns3/end-device-lora-phy.h"
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
  .AddTraceSource ("NumberOfOpenSecondReceiveWindowCalls",
                   "Whether or not the second occurence of the "
                    "second receive window is cancelled.",
                    MakeTraceSourceAccessor
                      (&EndDeviceLorawanMac::m_numCloseSecondReceiveWindowCalls),
                    "ns3::TracedValueCallback::Int")
  .AddConstructor<ClassCEndDeviceLorawanMac> ();
return tid;
}

ClassCEndDeviceLorawanMac::ClassCEndDeviceLorawanMac () 
{
  NS_LOG_FUNCTION (this);

  m_deviceClass = EndDeviceLorawanMac::CLASS_C;

  m_firstReceiveWindow = EventId ();
  m_firstReceiveWindow.Cancel ();
  m_continuousReceiveWindow = EventId ();
  m_continuousReceiveWindow.Cancel ();
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

  // Work on a copy of the packet
  Ptr<Packet> packetCopy = packet->Copy ();

  // Remove the Mac Header to get some information
  LorawanMacHeader mHdr;
  packetCopy->RemoveHeader (mHdr);

  NS_LOG_DEBUG ("Mac Header: " << mHdr);

  // Only keep analyzing the packet if it's downlink
  if (!mHdr.IsUplink ())
    {
      NS_LOG_INFO ("Found a downlink packet.");

      // Remove the Frame Header
      LoraFrameHeader fHdr;
      fHdr.SetAsDownlink ();
      packetCopy->RemoveHeader (fHdr);

      NS_LOG_DEBUG ("Frame Header: " << fHdr);

      // Determine whether this packet is for us
      bool messageForUs = (m_address == fHdr.GetAddress ());

      Time Rx1DelayRemaining = Time(0);
      Time Rx2DelayRemaining = Time(0);

      if (messageForUs)
        {
          NS_LOG_INFO ("The message is for us!");

          // If it exists, check which receive window the packet was
          // received in.  
          // Note: A scheduled event is marked as expired as soon as it starts
          if (m_continuousReceiveWindow.IsExpired () &&
              !m_firstReceiveWindow.IsExpired () &&
              !m_secondReceiveWindow.IsExpired ())
            {
              NS_LOG_DEBUG ("Packet was received in first occurence of RX2");
              Rx1DelayRemaining = Simulator::GetDelayLeft (m_firstReceiveWindow);
              NS_LOG_INFO ("Rx1DelayRemaining " << Rx1DelayRemaining);
              Rx2DelayRemaining = Simulator::GetDelayLeft (m_secondReceiveWindow);
              NS_LOG_INFO ("Rx2DelayRemaining " << Rx2DelayRemaining);
            }
          else if (m_continuousReceiveWindow.IsExpired () && 
                   m_firstReceiveWindow.IsExpired () &&
                   !m_secondReceiveWindow.IsExpired ())
            {
              NS_LOG_DEBUG ("Packet was received in RX1");
            }    
          else 
            {
              NS_LOG_DEBUG ("Packet must have been received in first occurence of RX2");
            }

          // Parse the MAC commands
          ParseCommands (fHdr);

          // TODO Pass the packet up to the NetDevice

          // TODO Check if we have already received this packet?

          // Call the trace source
          m_receivedPacket (packet);
          
          // if (!m_secondReceiveWindow.IsExpired ())
          //   {
          //     Simulator::ScheduleNow (&ClassCEndDeviceLorawanMac::OpenSecondReceiveWindow,
          //                                         this);
          //   }
          // m_secondSecondReceiveWindow = Simulator::Schedule (Seconds (0.001),
          //                                                    &ClassCEndDeviceLorawanMac::OpenSecondReceiveWindow,
          //                                                    this);
        }
    } 
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

  // Schedule the opening of the second receive window
  m_continuousReceiveWindow = Simulator::ScheduleNow (&ClassCEndDeviceLorawanMac::OpenSecondReceiveWindow,
                                                      this);

  // Schedule the opening of the first receive window
  m_firstReceiveWindow = Simulator::Schedule (m_receiveDelay1,
                                              &ClassCEndDeviceLorawanMac::OpenFirstReceiveWindow, this);

  // Schedule the opening of the second receive window
  m_secondReceiveWindow = Simulator::Schedule (m_receiveDelay2,
                                               &ClassCEndDeviceLorawanMac::OpenSecondReceiveWindow,
                                               this);
  
}

} /* namespace lorawan */
} /* namespace ns3 */
