/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This file includes testing for the following components:
 * - ClassCEndDeviceStatus
 *
 * Author:Peggy Anderson <peggy.anderson@usask.ca>
*/

// Include headers of classes to test
#include "ns3/log.h"
#include "utilities.h"
#include "ns3/callback.h"
#include "ns3/class-c-end-device-lorawan-mac.h"
#include "ns3/lora-helper.h"

// An essential include is test.h
#include "ns3/test.h"

using namespace ns3;
using namespace lorawan;

NS_LOG_COMPONENT_DEFINE ("ClassCEndDeviceLorawanMacTestSuite");

///////////////////////////////////////////
// LorawanMacHelperSetDeviceType testing //
///////////////////////////////////////////

class InitializeLorawanMacClassCEndDeviceTest : public TestCase
{
public:
  InitializeLorawanMacClassCEndDeviceTest ();
  virtual ~InitializeLorawanMacClassCEndDeviceTest ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
InitializeLorawanMacClassCEndDeviceTest::InitializeLorawanMacClassCEndDeviceTest ()
  : TestCase ("Verify that LorawanMacHelper Initalizes the Device Type to be "
              "ED_C")
{
}

// Reminder that the test case should clean up after itself
InitializeLorawanMacClassCEndDeviceTest::~InitializeLorawanMacClassCEndDeviceTest ()
{
}
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
void
InitializeLorawanMacClassCEndDeviceTest::DoRun (void)
{
  NS_LOG_DEBUG ("InitializeLorawanMacClassCEndDeviceTest");

  // Create the LorawanMacHelper
  LorawanMacHelper macHelper = LorawanMacHelper ();
  macHelper.SetDeviceType (LorawanMacHelper::ED_C);
  NS_TEST_ASSERT_MSG_EQ(macHelper.GetDeviceType (), LorawanMacHelper::ED_C,
                        "macHelper Device Type Not Set Properly.");
}


///////////////////////////////////////////////////
// CreateNodeContainerOfOneClassCDeviceTest Test //
///////////////////////////////////////////////////
class CreateNodeContainerOfOneClassCDeviceTest : public TestCase
{
public:
  CreateNodeContainerOfOneClassCDeviceTest ();
  virtual ~CreateNodeContainerOfOneClassCDeviceTest ();

private:
  virtual void DoRun (void);
};

CreateNodeContainerOfOneClassCDeviceTest::CreateNodeContainerOfOneClassCDeviceTest ()
  : TestCase ("Verify creating a NodeContainer that contains one "
              "ClassCEndDeviceLorawanMac device")
{
}

CreateNodeContainerOfOneClassCDeviceTest::~CreateNodeContainerOfOneClassCDeviceTest ()
{
}
void
CreateNodeContainerOfOneClassCDeviceTest::DoRun (void)
{
  NS_LOG_DEBUG ("CreateNodeContainerOfOneClassCDeviceTest");

  NetworkComponents components = InitializeNetwork (1, 1, 2);

  NodeContainer endDevices = components.endDevices;
  Ptr<ClassCEndDeviceLorawanMac> edMac = endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetMac ()->GetObject<ClassCEndDeviceLorawanMac>();
  NS_TEST_ASSERT_MSG_NE (edMac, 0, "fail");

  NodeContainer::Iterator it;
  int i = 0;
  for (it = endDevices.Begin (); it != endDevices.End (); it++)
    {
      i++;
    }
  NS_TEST_ASSERT_MSG_EQ(i, 1, "More than one ED was created.");
}


/////////////////////////////////////////////////////
// CreateNodeContainerOfManyClassCDeviceTests Test //
/////////////////////////////////////////////////////
class CreateNodeContainerOfManyClassCDeviceTests : public TestCase
{
public:
  CreateNodeContainerOfManyClassCDeviceTests ();
  virtual ~CreateNodeContainerOfManyClassCDeviceTests ();

private:
  virtual void DoRun (void);
};

CreateNodeContainerOfManyClassCDeviceTests::CreateNodeContainerOfManyClassCDeviceTests ()
  : TestCase ("Verify creating a NodeContainer that contains many "
              "ClassCEndDeviceLorawanMac devices")
{
}

CreateNodeContainerOfManyClassCDeviceTests::~CreateNodeContainerOfManyClassCDeviceTests ()
{
}
void
CreateNodeContainerOfManyClassCDeviceTests::DoRun (void)
{
  NS_LOG_DEBUG ("CreateNodeContainerOfManyClassCDeviceTests");

  NetworkComponents components = InitializeNetwork (10, 1, 2);

  NodeContainer endDevices = components.endDevices;
  Ptr<ClassCEndDeviceLorawanMac> edMac = endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetMac ()->GetObject<ClassCEndDeviceLorawanMac>();
  NS_TEST_ASSERT_MSG_NE (edMac, 0, "fail");

  NodeContainer::Iterator it;
  int i = 0;
  for (it = endDevices.Begin (); it != endDevices.End (); it++)
    {
      i++;
    }
  NS_TEST_ASSERT_MSG_EQ(i, 10, "More than one ED was created.");
}


////////////////////////////////////////
// UplinkPacketClassCDeviceTests Test //
////////////////////////////////////////
class UplinkPacketClassCDeviceTests : public TestCase
{
public:
  UplinkPacketClassCDeviceTests ();
  virtual ~UplinkPacketClassCDeviceTests ();

  void ReceivedPacket (Ptr<Packet const> packet);
  void SendPacket (Ptr<Node> endDevice);

private:
  virtual void DoRun (void);
  bool m_receivedPacket = false;
};

// Add some help text to this case to describe what it is intended to test
UplinkPacketClassCDeviceTests::UplinkPacketClassCDeviceTests ()
  : TestCase ("Verify that the NetworkServer can receive"
              " packets sent in the uplink by devices")
{
}

// Reminder that the test case should clean up after itself
UplinkPacketClassCDeviceTests::~UplinkPacketClassCDeviceTests ()
{
}

void
UplinkPacketClassCDeviceTests::ReceivedPacket (Ptr<Packet const> packet)
{
  NS_LOG_DEBUG ("Received a packet at the NS");
  m_receivedPacket = true;
}

void
UplinkPacketClassCDeviceTests::SendPacket (Ptr<Node> endDevice)
{
  endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
}

// This method is the pure virtual method from class TestCase that every
// TestCase must implement
void
UplinkPacketClassCDeviceTests::DoRun (void)
{
  NS_LOG_DEBUG ("UplinkPacketClassCDeviceTests");

  // Create a bunch of actual devices
  NetworkComponents components = InitializeNetwork (1, 1, 2);

  Ptr<LoraChannel> channel = components.channel;
  NodeContainer endDevices = components.endDevices;
  NodeContainer gateways = components.gateways;
  Ptr<Node> nsNode = components.nsNode;

  // Connect the trace source for received packets
  nsNode->GetApplication (0)->TraceConnectWithoutContext
    ("ReceivedPacket",
    MakeCallback
      (&UplinkPacketClassCDeviceTests::ReceivedPacket,
      this));

  // Send a packet
  Simulator::Schedule (Seconds (1), &UplinkPacketClassCDeviceTests::SendPacket, this,
                       endDevices.Get (0));

  Simulator::Stop (Seconds (5));
  Simulator::Run ();
  Simulator::Destroy ();

  // Check that we received the packet
  NS_ASSERT (m_receivedPacket == true);
}


// NOT READY FOR THESE TESTS
///////////////////////////////////////
// ReceiveDownlinkMessageClassC Test //
///////////////////////////////////////
// class ReceiveDownlinkMessageClassC : public TestCase
// {
// public:
//   ReceiveDownlinkMessageClassC ();
//   virtual ~ReceiveDownlinkMessageClassC ();

//   void ReceivedPacketAtEndDevice (uint8_t requiredTransmissions, bool success,
//                                   Time time, Ptr<Packet> packet);
//   void SendPacket (Ptr<Node> endDevice);

// private:
//   virtual void DoRun (void);
//   bool m_receivedPacketAtEd = false;
// };

// ReceiveDownlinkMessageClassC::ReceiveDownlinkMessageClassC ()
//   : TestCase ("Verify creating a NodeContainer that contains many "
//               "ClassCEndDeviceLorawanMac devices")
// {
// }

// ReceiveDownlinkMessageClassC::~ReceiveDownlinkMessageClassC ()
// {
// }

// void
// ReceiveDownlinkMessageClassC::ReceivedPacketAtEndDevice (uint8_t requiredTransmissions, bool success, Time time, Ptr<Packet> packet)
// {
//   NS_LOG_DEBUG ("Received a packet at the ED");
//   m_receivedPacketAtEd = true;
// }

// void
// ReceiveDownlinkMessageClassC::SendPacket (Ptr<Node> endDevice)
// {
//   endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
// }

// void
// ReceiveDownlinkMessageClassC::DoRun (void)
// {
//   NS_LOG_DEBUG ("ReceiveDownlinkMessage");

//   NetworkComponents components = InitializeNetwork (1, 1, 2);

//   NodeContainer endDevices = components.endDevices;

//   // Connect the ED's trace source for received packets
//   endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetMac ()->GetObject<EndDeviceLorawanMac>()->TraceConnectWithoutContext ("RequiredTransmissions", MakeCallback (&ReceiveDownlinkMessageClassC::ReceivedPacketAtEndDevice, this));

  
//   // Send a packet
//   Simulator::Schedule (Seconds (1), &ReceiveDownlinkMessageClassC::SendPacket, this,
//                        endDevices.Get (0));

//   Simulator::Stop (Seconds (8));
//   Simulator::Run ();
//   Simulator::Destroy ();

//   // Check that we received the packet
//   NS_ASSERT (m_receivedPacketAtEd == true);
// }


/**************
 * Test Suite *
 **************/

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run. Typically, only the constructor for
// this class must be defined

class ClassCEndDeviceLorawanMacTestSuite : public TestSuite
{
public:
  ClassCEndDeviceLorawanMacTestSuite ();
};

ClassCEndDeviceLorawanMacTestSuite::ClassCEndDeviceLorawanMacTestSuite ()
  : TestSuite ("class-c", UNIT)
  // : TestSuite ("class-c-lora-end-device", UNIT)
{
  LogComponentEnable ("ClassCEndDeviceLorawanMacTestSuite", LOG_LEVEL_DEBUG);

  LogComponentEnable ("LoraHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("LorawanMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LoraNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LorawanMacHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("EndDeviceLoraPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("ClassCEndDeviceLorawanMac", LOG_LEVEL_ALL);
  LogComponentEnable ("SimpleEndDeviceLoraPhy", LOG_LEVEL_ALL);
  // LogComponentEnable ("EndDeviceLoraPhy", LOG_LEVEL_ALL);

  // LogComponentEnableAll (LOG_PREFIX_FUNC);
  // LogComponentEnableAll (LOG_PREFIX_NODE);
  // LogComponentEnableAll (LOG_PREFIX_TIME);

  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new InitializeLorawanMacClassCEndDeviceTest, TestCase::QUICK);
  AddTestCase (new CreateNodeContainerOfOneClassCDeviceTest, TestCase::QUICK);
  AddTestCase (new CreateNodeContainerOfManyClassCDeviceTests, TestCase::QUICK);
  AddTestCase (new UplinkPacketClassCDeviceTests, TestCase::QUICK);
  // AddTestCase (new ReceiveDownlinkMessageClassC, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static ClassCEndDeviceLorawanMacTestSuite lorawanTestSuite;
