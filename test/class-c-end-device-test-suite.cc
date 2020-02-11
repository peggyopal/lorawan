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


///////////////////////////////////////////
// PacketReceivedInEDPhyLayerClassC Test //
///////////////////////////////////////////
class PacketReceivedInEDPhyLayerClassC : public TestCase
{
public:
  PacketReceivedInEDPhyLayerClassC ();
  virtual ~PacketReceivedInEDPhyLayerClassC ();

  void PacketReceivedInEDPhyLayer (Ptr<Packet const> packet, uint32_t address);
  void SendPacket (Ptr<Node> endDevice);

private:
  virtual void DoRun (void);
  bool m_receivedPacketInEDPhyLayer = false;
};

PacketReceivedInEDPhyLayerClassC::PacketReceivedInEDPhyLayerClassC ()
  : TestCase ("Verify that the PHY layer of a Class C End Device "
              "receives a packet from the MAC layer and is able "
              "to continue the transmission.")
{
}

PacketReceivedInEDPhyLayerClassC::~PacketReceivedInEDPhyLayerClassC ()
{
}

void
PacketReceivedInEDPhyLayerClassC::PacketReceivedInEDPhyLayer (Ptr<Packet const> packet, uint32_t address)
{
  NS_LOG_DEBUG ("Sending a packet from the ED Phy Layer");
  m_receivedPacketInEDPhyLayer = true;
}

void
PacketReceivedInEDPhyLayerClassC::SendPacket (Ptr<Node> endDevice)
{
  endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
}

void
PacketReceivedInEDPhyLayerClassC::DoRun (void)
{
  NS_LOG_DEBUG ("PacketReceivedInEDPhyLayerClassC");

  NetworkComponents components = InitializeNetwork (1, 1, 2);

  NodeContainer endDevices = components.endDevices;

  // Connect the ED's trace source for received packets
  endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetPhy ()->GetObject<LoraPhy>()->TraceConnectWithoutContext 
    ("StartSending", 
    MakeCallback 
      (&PacketReceivedInEDPhyLayerClassC::PacketReceivedInEDPhyLayer, 
      this));

  
  // Send a packet
  Simulator::Schedule (Seconds (1), &PacketReceivedInEDPhyLayerClassC::SendPacket, this,
                       endDevices.Get (0));

  Simulator::Stop (Seconds (8));
  Simulator::Run ();
  Simulator::Destroy ();

  // Check that we received the packet
  NS_ASSERT (m_receivedPacketInEDPhyLayer == true);
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
              " packets sent in the uplink by class C devices")
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


/////////////////////////////////////////////
// SecondReceiveWindowStaysOpenClassC Test //
/////////////////////////////////////////////
class SecondReceiveWindowStaysOpenClassC : public TestCase
{
public:
  SecondReceiveWindowStaysOpenClassC ();
  virtual ~SecondReceiveWindowStaysOpenClassC ();

  void NumberOfOpenSecondReceiveWindowCalls (Ptr<Packet const> packet);
  void SendPacket (Ptr<Node> endDevice);

private:
  virtual void DoRun (void);
  int m_numCloseSecondReceiveWindowCalls = 0;
};

// Add some help text to this case to describe what it is intended to test
SecondReceiveWindowStaysOpenClassC::SecondReceiveWindowStaysOpenClassC ()
  : TestCase ("Verify that CloseSecondReceiveWindow function is only"
              " called once.")
{
}

// Reminder that the test case should clean up after itself
SecondReceiveWindowStaysOpenClassC::~SecondReceiveWindowStaysOpenClassC ()
{
}

void
SecondReceiveWindowStaysOpenClassC::NumberOfOpenSecondReceiveWindowCalls (Ptr<Packet const> packet)
{
  NS_LOG_DEBUG ("Received a packet at the NS");
  m_numCloseSecondReceiveWindowCalls += 1;
}

void
SecondReceiveWindowStaysOpenClassC::SendPacket (Ptr<Node> endDevice)
{
  endDevice->GetDevice (0)->Send (Create<Packet> (20), Address (), 0);
}

// This method is the pure virtual method from class TestCase that every
// TestCase must implement
void
SecondReceiveWindowStaysOpenClassC::DoRun (void)
{
  NS_LOG_DEBUG ("SecondReceiveWindowStaysOpenClassC");

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
      (&SecondReceiveWindowStaysOpenClassC::NumberOfOpenSecondReceiveWindowCalls,
      this));

  // Send a packet
  Simulator::Schedule (Seconds (1), &SecondReceiveWindowStaysOpenClassC::SendPacket, this,
                       endDevices.Get (0));

  Simulator::Stop (Seconds (5));
  Simulator::Run ();
  Simulator::Destroy ();

  // Check that we received the packet
  NS_ASSERT (m_numCloseSecondReceiveWindowCalls == 1);
}


// NOT READY FOR THESE TESTS
///////////////////////////////////////
// ReceiveDownlinkMessageClassC Test //
///////////////////////////////////////
class ReceiveDownlinkMessageClassC : public TestCase
{
public:
  ReceiveDownlinkMessageClassC ();
  virtual ~ReceiveDownlinkMessageClassC ();

  void ReceivedPacketAtED (Ptr<Packet const> packet);
  void SendPacket (Ptr<Node> endDevice, bool requestAck);

private:
  virtual void DoRun (void);
  bool m_receivedPacketAtEd = false;
};

ReceiveDownlinkMessageClassC::ReceiveDownlinkMessageClassC ()
  : TestCase ("Verify creating a NodeContainer that contains many "
              "ClassCEndDeviceLorawanMac devices")
{
}

ReceiveDownlinkMessageClassC::~ReceiveDownlinkMessageClassC ()
{
}

void
ReceiveDownlinkMessageClassC::ReceivedPacketAtED (Ptr<Packet const> packet)
{
  NS_LOG_DEBUG ("Received a packet at the ED");
  m_receivedPacketAtEd = true;
}

void
ReceiveDownlinkMessageClassC::SendPacket (Ptr<Node> endDevice, bool requestAck)
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
ReceiveDownlinkMessageClassC::DoRun (void)
{
  NS_LOG_DEBUG ("ReceiveDownlinkMessage");

  NetworkComponents components = InitializeNetwork (1, 1, 2);

  NodeContainer endDevices = components.endDevices;

  // Connect the ED's trace source for received packets
  endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice>()->GetMac ()->GetObject<EndDeviceLorawanMac>()->TraceConnectWithoutContext 
    ("ReceivedPacket", 
    MakeCallback 
      (&ReceiveDownlinkMessageClassC::ReceivedPacketAtED, 
      this));

  
  // Send a packet
  Simulator::Schedule (Seconds (1), &ReceiveDownlinkMessageClassC::SendPacket, this,
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
  LogComponentEnable ("NetworkScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("NetworkStatus", LOG_LEVEL_ALL);

  // LogComponentEnableAll (LOG_PREFIX_FUNC);
  // LogComponentEnableAll (LOG_PREFIX_NODE);
  // LogComponentEnableAll (LOG_PREFIX_TIME);

  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new InitializeLorawanMacClassCEndDeviceTest, TestCase::QUICK);
  AddTestCase (new InitializeDeviceClassTest, TestCase::QUICK);
  AddTestCase (new CreateNodeContainerOfOneClassCDeviceTest, TestCase::QUICK);
  AddTestCase (new CreateNodeContainerOfManyClassCDeviceTests, TestCase::QUICK);
  AddTestCase (new PacketReceivedInEDPhyLayerClassC, TestCase::QUICK);
  AddTestCase (new UplinkPacketClassCDeviceTests, TestCase::QUICK);
  AddTestCase (new SecondReceiveWindowStaysOpenClassC, TestCase::QUICK);
  AddTestCase (new ReceiveDownlinkMessageClassC, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static ClassCEndDeviceLorawanMacTestSuite lorawanTestSuite;
