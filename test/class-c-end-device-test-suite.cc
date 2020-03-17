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

/////////////////////////////////////////////////////
// InitializeLorawanMacClassCEndDeviceTest testing //
/////////////////////////////////////////////////////
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


///////////////////////////////////////
// InitializeDeviceClassTest testing //
///////////////////////////////////////
class InitializeDeviceClassTest : public TestCase
{
public:
  InitializeDeviceClassTest ();
  virtual ~InitializeDeviceClassTest ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
InitializeDeviceClassTest::InitializeDeviceClassTest ()
  : TestCase ("Verify that on creation of a ClassCEndDeviceLorawanMac "
              "device, m_deviceClass is set to CLASS_C")
{
}

// Reminder that the test case should clean up after itself
InitializeDeviceClassTest::~InitializeDeviceClassTest ()
{
}
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
void
InitializeDeviceClassTest::DoRun (void)
{
  NS_LOG_DEBUG ("InitializeDeviceClassTest");

  // Create the network components
  NetworkComponents components = InitializeNetwork (1, 1, 2);

  // Get the endDevices
  NodeContainer endDevices = components.endDevices;

  Ptr<ClassCEndDeviceLorawanMac> edMac = endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetMac ()->GetObject<ClassCEndDeviceLorawanMac>();
  enum EndDeviceLorawanMac::DeviceClass dc = edMac->GetDeviceClass ();

  NS_TEST_ASSERT_MSG_EQ(dc, EndDeviceLorawanMac::CLASS_C,
                        "m_deviceClass Not Set Properly.");
}


///////////////////////////////////
// CreateNodeContainerOfOne Test //
///////////////////////////////////
class CreateNodeContainerOfOne : public TestCase
{
public:
  CreateNodeContainerOfOne ();
  virtual ~CreateNodeContainerOfOne ();

private:
  virtual void DoRun (void);
};

CreateNodeContainerOfOne::CreateNodeContainerOfOne ()
  : TestCase ("Verify creating a NodeContainer that contains one "
              "ClassCEndDeviceLorawanMac device")
{
}

CreateNodeContainerOfOne::~CreateNodeContainerOfOne ()
{
}
void
CreateNodeContainerOfOne::DoRun (void)
{
  NS_LOG_DEBUG ("CreateNodeContainerOfOne");

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
// CreateNodeContainerOfMany Test //
/////////////////////////////////////////////////////
class CreateNodeContainerOfMany : public TestCase
{
public:
  CreateNodeContainerOfMany ();
  virtual ~CreateNodeContainerOfMany ();

private:
  virtual void DoRun (void);
};

CreateNodeContainerOfMany::CreateNodeContainerOfMany ()
  : TestCase ("Verify creating a NodeContainer that contains many "
              "ClassCEndDeviceLorawanMac devices")
{
}

CreateNodeContainerOfMany::~CreateNodeContainerOfMany ()
{
}
void
CreateNodeContainerOfMany::DoRun (void)
{
  NS_LOG_DEBUG ("CreateNodeContainerOfMany");

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


/////////////////////////////////////
// PacketReceivedInEDPhyLayer Test //
/////////////////////////////////////
class PacketReceivedInEDPhyLayer : public TestCase
{
public:
  PacketReceivedInEDPhyLayer ();
  virtual ~PacketReceivedInEDPhyLayer ();

  void PacketReceivedInEDPhyLayer (Ptr<Packet const> packet, uint32_t address);
  void SendPacket (Ptr<Node> endDevice);

private:
  virtual void DoRun (void);
  bool m_receivedPacketInEDPhyLayer = false;
};

PacketReceivedInEDPhyLayer::PacketReceivedInEDPhyLayer ()
  : TestCase ("Verify that the PHY layer of a Class C End Device "
              "receives a packet from the MAC layer and is able "
              "to continue the transmission.")
{
}

PacketReceivedInEDPhyLayer::~PacketReceivedInEDPhyLayer ()
{
}

void
PacketReceivedInEDPhyLayer::PacketReceivedInEDPhyLayer (Ptr<Packet const> packet, uint32_t address)
{
  NS_LOG_DEBUG ("Sending a packet from the ED Phy Layer");
  m_receivedPacketInEDPhyLayer = true;
}

void
PacketReceivedInEDPhyLayer::SendPacket (Ptr<Node> endDevice)
{
  endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
}

void
PacketReceivedInEDPhyLayer::DoRun (void)
{
  NS_LOG_DEBUG ("PacketReceivedInEDPhyLayer");

  NetworkComponents components = InitializeNetwork (1, 1, 2);

  NodeContainer endDevices = components.endDevices;

  // Connect the ED's trace source for received packets
  endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetPhy ()->GetObject<LoraPhy>()->TraceConnectWithoutContext 
    ("StartSending", 
    MakeCallback 
      (&PacketReceivedInEDPhyLayer::PacketReceivedInEDPhyLayer, 
      this));

  
  // Send a packet
  Simulator::Schedule (Seconds (1), &PacketReceivedInEDPhyLayer::SendPacket, this,
                       endDevices.Get (0));

  Simulator::Stop (Seconds (8));
  Simulator::Run ();
  Simulator::Destroy ();

  // Check that we received the packet
  NS_ASSERT (m_receivedPacketInEDPhyLayer == true);
}


/////////////////////////////
// UplinkPacketClassC Test //
/////////////////////////////
class UplinkPacketClassC : public TestCase
{
public:
  UplinkPacketClassC ();
  virtual ~UplinkPacketClassC ();

  void ReceivedPacket (Ptr<Packet const> packet);
  void SendPacket (Ptr<Node> endDevice);

private:
  virtual void DoRun (void);
  bool m_receivedPacket = false;
};

// Add some help text to this case to describe what it is intended to test
UplinkPacketClassC::UplinkPacketClassC ()
  : TestCase ("Verify that the NetworkServer can receive"
              " packets sent in the uplink by class C devices")
{
}

// Reminder that the test case should clean up after itself
UplinkPacketClassC::~UplinkPacketClassC ()
{
}

void
UplinkPacketClassC::ReceivedPacket (Ptr<Packet const> packet)
{
  NS_LOG_DEBUG ("Received a packet at the NS");
  m_receivedPacket = true;
}

void
UplinkPacketClassC::SendPacket (Ptr<Node> endDevice)
{
  endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
}

// This method is the pure virtual method from class TestCase that every
// TestCase must implement
void
UplinkPacketClassC::DoRun (void)
{
  NS_LOG_DEBUG ("UplinkPacketClassC");

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


////////////////////////////////////
// ThirdRxcStaysOpenAfterRx2 Test //
////////////////////////////////////
class ThirdRxcStaysOpenAfterRx2 : public TestCase
{
public:
  ThirdRxcStaysOpenAfterRx2 ();
  virtual ~ThirdRxcStaysOpenAfterRx2 ();

  void NumberOfOpenSecondReceiveWindowCalls (int oldValue, int newValue);
  void SendPacket (Ptr<Node> endDevice);

private:
  virtual void DoRun (void);
  int m_numContinuousReceiveWindows = 0;
};

// Add some help text to this case to describe what it is intended to test
ThirdRxcStaysOpenAfterRx2::ThirdRxcStaysOpenAfterRx2 ()
  : TestCase ("Verify that CloseSecondReceiveWindow function is only"
              " called once.")
{
}

// Reminder that the test case should clean up after itself
ThirdRxcStaysOpenAfterRx2::~ThirdRxcStaysOpenAfterRx2 ()
{
}

void
ThirdRxcStaysOpenAfterRx2::NumberOfOpenSecondReceiveWindowCalls (int oldValue, int newValue)
{
  NS_LOG_DEBUG ("Received a packet at the NS");
  m_numContinuousReceiveWindows += 1;
}

void
ThirdRxcStaysOpenAfterRx2::SendPacket (Ptr<Node> endDevice)
{
  endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
}

// This method is the pure virtual method from class TestCase that every
// TestCase must implement
void
ThirdRxcStaysOpenAfterRx2::DoRun (void)
{
  NS_LOG_DEBUG ("ThirdRxcStaysOpenAfterRx2");

  // Create a bunch of actual devices
  NetworkComponents components = InitializeNetwork (1, 1, 2);

  Ptr<LoraChannel> channel = components.channel;
  NodeContainer endDevices = components.endDevices;
  NodeContainer gateways = components.gateways;
  Ptr<Node> nsNode = components.nsNode;

  // Connect the trace source for received packets
  //nsNode->GetApplication (0)->TraceConnectWithoutContext
  endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetMac ()->GetObject<ClassCEndDeviceLorawanMac>()->TraceConnectWithoutContext 
      ("NumberOfContinuousReceiveWindowCalls",
    MakeCallback
      (&ThirdRxcStaysOpenAfterRx2::NumberOfOpenSecondReceiveWindowCalls,
      this));

  // Send a packet
  Simulator::Schedule (Seconds (1), &ThirdRxcStaysOpenAfterRx2::SendPacket, this,
                       endDevices.Get (0));

  Simulator::Stop (Seconds (5));
  Simulator::Run ();
  Simulator::Destroy ();

  // Check that we received the packet
  NS_ASSERT (m_numContinuousReceiveWindows == 3);
}


/////////////////////////////////////
// ReceiveDownlinkMessageRXC1 Test //
/////////////////////////////////////
class ReceiveDownlinkMessageRXC1 : public TestCase
{
public:
  ReceiveDownlinkMessageRXC1 ();
  virtual ~ReceiveDownlinkMessageRXC1 ();

  void ReceivedPacketAtED (Ptr<Packet const> packet);
  void SendPacket (Ptr<Node> endDevice, bool requestAck);

private:
  virtual void DoRun (void);
  bool m_receivedPacketAtEd = false;
};

ReceiveDownlinkMessageRXC1::ReceiveDownlinkMessageRXC1 ()
  : TestCase ("Verify creating a NodeContainer that contains many "
              "ClassCEndDeviceLorawanMac devices")
{
}

ReceiveDownlinkMessageRXC1::~ReceiveDownlinkMessageRXC1 ()
{
}

void
ReceiveDownlinkMessageRXC1::ReceivedPacketAtED (Ptr<Packet const> packet)
{
  NS_LOG_DEBUG ("Received a packet at the ED");
  m_receivedPacketAtEd = true;
}

void
ReceiveDownlinkMessageRXC1::SendPacket (Ptr<Node> endDevice, bool requestAck)
{
  if (requestAck)
    {
      endDevice->GetDevice (0)->GetObject<LoraNetDevice> ()->GetMac
        ()->GetObject<EndDeviceLorawanMac> ()->SetMType
        (LorawanMacHeader::CONFIRMED_DATA_UP);
    }
  endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
}

void
ReceiveDownlinkMessageRXC1::DoRun (void)
{
  NS_LOG_DEBUG ("ReceiveDownlinkMessageRXC1");

  NetworkComponents components = InitializeNetwork (1, 1, 2);

  NodeContainer endDevices = components.endDevices;

  // Connect the ED's trace source for received packets
  endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetMac ()->GetObject<EndDeviceLorawanMac>()->TraceConnectWithoutContext 
    ("ReceivedPacket", 
    MakeCallback 
      (&ReceiveDownlinkMessageRXC1::ReceivedPacketAtED, 
      this));

  
  // Send a packet
  Simulator::Schedule (Seconds (1), &ReceiveDownlinkMessageRXC1::SendPacket, this,
                       endDevices.Get (0), true);

  Simulator::Stop (Seconds (8));
  Simulator::Run ();
  Simulator::Destroy ();

  // Check that we received the packet
  NS_ASSERT (m_receivedPacketAtEd == true);
}


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

  // LogComponentEnable ("LoraHelper", LOG_LEVEL_ALL);
  // LogComponentEnable ("LorawanMac", LOG_LEVEL_ALL);
  // LogComponentEnable ("LoraNetDevice", LOG_LEVEL_ALL);
  // LogComponentEnable ("LorawanMacHelper", LOG_LEVEL_ALL);
  // LogComponentEnable ("EndDeviceLoraPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("ClassCEndDeviceLorawanMac", LOG_LEVEL_ALL);
  // LogComponentEnable ("SimpleEndDeviceLoraPhy", LOG_LEVEL_ALL);
  // LogComponentEnable ("EndDeviceLoraPhy", LOG_LEVEL_ALL);
  // LogComponentEnable ("NetworkScheduler", LOG_LEVEL_ALL);
  // LogComponentEnable ("NetworkStatus", LOG_LEVEL_ALL);

  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnableAll (LOG_PREFIX_NODE);
  LogComponentEnableAll (LOG_PREFIX_TIME);

  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new InitializeLorawanMacClassCEndDeviceTest, TestCase::QUICK);
  AddTestCase (new InitializeDeviceClassTest, TestCase::QUICK);
  AddTestCase (new CreateNodeContainerOfOne, TestCase::QUICK);
  AddTestCase (new CreateNodeContainerOfMany, TestCase::QUICK);
  AddTestCase (new PacketReceivedInEDPhyLayer, TestCase::QUICK);
  AddTestCase (new UplinkPacketClassC, TestCase::QUICK);
  AddTestCase (new ThirdRxcStaysOpenAfterRx2, TestCase::QUICK);
  AddTestCase (new ReceiveDownlinkMessageRXC1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static ClassCEndDeviceLorawanMacTestSuite lorawanTestSuite;
