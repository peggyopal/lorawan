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

#ifndef CLASS_C_END_DEVICE_LORAWAN_MAC_H
#define CLASS_C_END_DEVICE_LORAWAN_MAC_H

#include "ns3/lorawan-mac.h"
#include "ns3/end-device-lorawan-mac.h"

namespace ns3 {
namespace lorawan {

/**
 * Class representing the MAC layer of a Class C LoRaWAN device.
 */
class ClassCEndDeviceLorawanMac : public EndDeviceLorawanMac
{
public:
  static TypeId GetTypeId (void);

  ClassCEndDeviceLorawanMac ();
  virtual ~ClassCEndDeviceLorawanMac ();

  //////////////////////////
  //  Receiving methods   //
  //////////////////////////

  /**
   * Receive a packet.
   *
   * This method is typically registered as a callback in the underlying PHY
   * layer so that it's called when a packet is going up the stack.
   *
   * \param packet the received packet.
   */
  virtual void Receive (Ptr<Packet const> packet);

  virtual void FailedReception (Ptr<Packet const> packet);

  /**
   * Perform the actions that are required after a packet send.
   *
   * This function handles opening of the first receive window.
   */
  virtual void TxFinished (Ptr<const Packet> packet);

  /**
   * Perform operations needed to open the first receive window.
   */
  void OpenFirstReceiveWindow (void);

  /**
   * Perform operations needed to close the first receive window.
   */
  void CloseFirstReceiveWindow (void);

  /**
   * Perform operations needed to open the second receive window.
   */
  void OpenSecondReceiveWindow (void);

  /**
   * Perform operations needed to close the second receive window.
   */
  void CloseSecondReceiveWindow (void);

  /////////////////////////
  // Getters and Setters //
  /////////////////////////

  /**
   * Set the Data Rate to be used in the second receive window.
   *
   * \param dataRate The Data Rate.
   */
  void SetSecondReceiveWindowDataRate (uint8_t dataRate);

  /**
   * Set the frequency that will be used for the second receive window.
   *
   * \param frequencyMHz the Frequency.
   */
  void SetSecondReceiveWindowFrequency (double frequencyMHz);

private:
  /**
   * The interval between when a packet is done sending and when the first
   * receive window is opened.
   */
  Time m_receiveDelay1;

  /**
   * The interval between when a packet is done sending and when the second
   * receive window is opened.
   */
  Time m_receiveDelay2;

  /**
   * The RX1DROffset parameter value
   */
  uint8_t m_rx1DrOffset;

  /**
   * The event of the closing the first receive window.
   *
   * This Event will be canceled if there's a successful reception of a packet.
   */
  EventId m_openFirstWindow;

  /**
   * The event of the closing the first receive window.
   *
   * This Event will be canceled if there's a successful reception of a packet.
   */
  EventId m_closeFirstWindow;

  /**
   * The event of the closing the second receive window.
   *
   * This Event will be canceled if there's a successful reception of a packet.
   */
  EventId m_closeSecondWindow;

  /**
   * The event of the second receive window opening.
   *
   * This Event is used to cancel the second window in case the first one is
   * successful.
   */
  EventId m_secondReceiveWindow;

  /**
   * The frequency to listen on for the second receive window.
   */
  double m_secondReceiveWindowFrequency;

  /**
   * The Data Rate to listen for during the second downlink transmission.
   */
  uint8_t m_secondReceiveWindowDataRate;

}; /* ClassCEndDeviceLorawanMac */

} /* namespace lorawan */
} /* namespace ns3 */

#endif /* CLASS_C_END_DEVICE_LORAWAN_MAC_H */
