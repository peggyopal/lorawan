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

#ifndef END_DEVICE_C_LORA_MAC_H
#define END_DEVICE_C_LORA_MAC_H

#include "ns3/lorawan-mac.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/lorawan-mac-header.h"
#include "ns3/lora-frame-header.h"
#include "ns3/random-variable-stream.h"
#include "ns3/lora-device-address.h"
#include "ns3/traced-value.h"

namespace ns3 {
namespace lorawan {

/**
 * Class representing the MAC layer of a LoRaWAN device.
 */
class EndDeviceCLorawanMac : public EndDeviceLorawanMac
{
public:
  static TypeId GetTypeId (void);

  EndDeviceCLorawanMac ();
  virtual ~EndDeviceCLorawanMac ();

  /**
   * Set the message type to send when the Send method is called.
   */
  void SetMType (LorawanMacHeader::MType mType);

  private:
    /**
     * The message type to apply to packets sent with the Send method.
     */
    LorawanMacHeader::MType m_mType;
};

} /* namespace ns3 */

}
#endif /* END_DEVICE_LORA_MAC_H */
