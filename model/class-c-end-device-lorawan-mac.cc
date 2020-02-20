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
  .AddTraceSource ("NumberOfContinuousReceiveWindowCalls",
                   "Count how many times a continuous receive window "
                   "has been opened.",
                    MakeTraceSourceAccessor
                      (&EndDeviceLorawanMac::m_numContinuousReceiveWindows),
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
  m_continuousReceiveWindow1 = EventId ();
  m_continuousReceiveWindow1.Cancel ();
  m_continuousReceiveWindow2 = EventId ();
  m_continuousReceiveWindow2.Cancel ();
  m_continuousReceiveWindow3 = EventId ();
  m_continuousReceiveWindow3.Cancel ();
  m_closeContinuousWindow1 = EventId ();
  m_closeContinuousWindow1.Cancel ();
  m_closeContinuousWindow2 = EventId ();
  m_closeContinuousWindow2.Cancel ();
  m_closeContinuousWindow3 = EventId ();
  m_closeContinuousWindow3.Cancel ();
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
      Time RxcCloseDelayRemaining = Time(0);


      // Keep track of time remaining before the next events are to execute
      // Remove 0.013 seconds off of time to simulate doing stuff
      Rx1DelayRemaining = Simulator::GetDelayLeft (m_firstReceiveWindow) - Seconds(0.013);
      Rx2DelayRemaining = Simulator::GetDelayLeft (m_secondReceiveWindow) - Seconds(0.013);
      RxcCloseDelayRemaining = Simulator::GetDelayLeft (m_closeContinuousWindow1) - Seconds(0.013);

      if (messageForUs)
        {
          NS_LOG_INFO ("The message is for us!");

          // If it exists, check which receive window the packet was
          // received in.  
          // Note: A scheduled event is marked as expired as soon as it starts
          // if (m_continuousReceiveWindow.IsExpired () &&
          //     !m_firstReceiveWindow.IsExpired () &&
          //     !m_secondReceiveWindow.IsExpired ())
          if (Rx1DelayRemaining == RxcCloseDelayRemaining)
            {
              NS_LOG_DEBUG ("Packet was received before RX1");
              
              Simulator::Cancel (m_firstReceiveWindow);
              Simulator::Cancel (m_secondReceiveWindow);
              Simulator::Cancel (m_continuousReceiveWindow2);
              Simulator::Cancel (m_continuousReceiveWindow3);
              Simulator::Cancel (m_closeContinuousWindow1);
              Simulator::Cancel (m_closeContinuousWindow2);
              Simulator::Cancel (m_closeContinuousWindow3);

              Time Rx1Delay = (Rx1DelayRemaining > Seconds(0)) ? Rx1DelayRemaining : Seconds (0);
              NS_LOG_INFO (m_receiveDelay1.Compare(Rx1Delay));
              NS_LOG_INFO (Rx1Delay.IsPositive ());
              // m_closeContinuousWindow = Simulator::Schedule (Rx1Delay,
              //                                                &ClassCEndDeviceLorawanMac::CloseContinuousReceiveWindow,
              //                                                this);
              // m_firstReceiveWindow = Simulator::Schedule (Rx1Delay,
              //                                            &EndDeviceLorawanMac::OpenFirstReceiveWindow, 
              //                                            this);
              // Time Rx2Delay = (Rx2DelayRemaining > Seconds(0)) ? Rx2DelayRemaining : Seconds (0);
              // NS_LOG_INFO(Rx2Delay);


              // Simulator::Cancel (m_continuousReceiveWindow);
              // m_secondReceiveWindow = Simulator::Schedule ();
              // m_closeContinuousWindow = Simulator::Schedule ();
            }
          else if (!m_continuousReceiveWindow2.IsExpired () && 
                   m_firstReceiveWindow.IsExpired () &&
                   !m_secondReceiveWindow.IsExpired ())
            {
              NS_LOG_DEBUG ("Packet was received in RX1");
            }    
          else if (m_continuousReceiveWindow2.IsExpired () && 
                   m_firstReceiveWindow.IsExpired () &&
                   !m_secondReceiveWindow.IsExpired ())
            {
              NS_LOG_DEBUG ("Packet was received after RX1 but before RX2");
            }    
          else if (!m_continuousReceiveWindow3.IsExpired () && 
                   m_firstReceiveWindow.IsExpired () &&
                   m_secondReceiveWindow.IsExpired ())
            {
              NS_LOG_DEBUG ("Packet was received in RX2");
            }
          else 
            {
              NS_LOG_DEBUG ("Packet was received in the final RXC");
            }

          // Parse the MAC commands
          ParseCommands (fHdr);

          // TODO Pass the packet up to the NetDevice

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
ClassCEndDeviceLorawanMac::OpenContinuousReceiveWindow (void)
{ 
  NS_LOG_FUNCTION_NOARGS ();

  m_numContinuousReceiveWindows += 1;

  Simulator::ScheduleNow (&ClassCEndDeviceLorawanMac::OpenSecondReceiveWindow,
                          this);

  // Time Rx2DelayRemaining = Time(0);

  if (m_numContinuousReceiveWindows == 1)
    {
      // This is the first receive window to open, it occurs before RX1
      // this->CloseContinuousReceiveWindow (m_receiveDelay1); 
      m_closeContinuousWindow1 = Simulator::Schedule (m_receiveDelay1,
                                                      &ClassCEndDeviceLorawanMac::CloseContinuousReceiveWindow, 
                                                      this);
    }
  else if (m_numContinuousReceiveWindows == 2)
    {
      // Close RXC when RX2 opens
      // Rx2DelayRemaining = Simulator::GetDelayLeft (m_secondReceiveWindow);
      // if GetDelayLeft == 0 then... close immediately?
      // this->CloseContinuousReceiveWindow (Rx2DelayRemaining); 
      m_closeContinuousWindow2 = Simulator::Schedule (m_receiveDelay2,
                                                      &ClassCEndDeviceLorawanMac::CloseContinuousReceiveWindow, 
                                                      this);
    }
}

void
ClassCEndDeviceLorawanMac::CloseContinuousReceiveWindow (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Simulator::ScheduleNow (&ClassCEndDeviceLorawanMac::CloseSecondReceiveWindow,
                          this);
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

  double tSymRx1 = pow (2, GetSfFromDataRate (GetFirstReceiveWindowDataRate ())) / GetBandwidthFromDataRate ( GetFirstReceiveWindowDataRate ());
  Time Rxc2Delay = Seconds (m_receiveWindowDurationInSymbols*tSymRx1);
  double tSymRx2 = pow (2, GetSfFromDataRate (GetSecondReceiveWindowDataRate ())) / GetBandwidthFromDataRate ( GetSecondReceiveWindowDataRate ());
  Time Rxc3Delay = Seconds (m_receiveWindowDurationInSymbols*tSymRx2);

  // Schedule the opening of the second receive window
  m_continuousReceiveWindow1 = Simulator::ScheduleNow (&ClassCEndDeviceLorawanMac::OpenContinuousReceiveWindow,
                                                       this);

  // Schedule the opening of the first receive window
  m_firstReceiveWindow = Simulator::Schedule (m_receiveDelay1,
                                              &ClassCEndDeviceLorawanMac::OpenFirstReceiveWindow, this);

  // After the first receive window (RW) closes, a continuous window must open.  
  // If a packet is received and for some  reason the first RW time extends 
  // past the delay to opened the second RW, then no continuous RW needs to 
  // open as an intermediate RW.  This case should be handled in Receive().
  m_continuousReceiveWindow2 = Simulator::Schedule (m_receiveDelay1 + Rxc2Delay,
                                                    &ClassCEndDeviceLorawanMac::OpenContinuousReceiveWindow,
                                                    this);

  // Schedule the opening of the second receive window
  m_secondReceiveWindow = Simulator::Schedule (m_receiveDelay2,
                                               &ClassCEndDeviceLorawanMac::OpenSecondReceiveWindow,
                                               this);

  m_continuousReceiveWindow3 = Simulator::Schedule (m_receiveDelay2+Rxc3Delay,
                                                    &ClassCEndDeviceLorawanMac::OpenContinuousReceiveWindow,
                                                    this);
  
}

} /* namespace lorawan */
} /* namespace ns3 */
