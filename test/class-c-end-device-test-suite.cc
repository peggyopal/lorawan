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
#include "ns3/class-c-end-device-lorawan-mac.h"
#include "ns3/lora-helper.h"

// An essential include is test.h
#include "ns3/test.h"

using namespace ns3;
using namespace lorawan;

NS_LOG_COMPONENT_DEFINE ("ClassCEndDeviceLorawanMacTestSuite");

/////////////////////////////
// EndDeviceStatus testing //
/////////////////////////////

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
  : TestCase ("Verify that the Lorawan Mac Helper Initalizes Device Type"
              "to ED_C")
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

  // LogComponentEnable ("NetworkServer", LOG_LEVEL_ALL);
  // LogComponentEnable ("NetworkStatus", LOG_LEVEL_ALL);
  // LogComponentEnable ("NetworkScheduler", LOG_LEVEL_ALL);
  // LogComponentEnable ("NetworkController", LOG_LEVEL_ALL);
  // LogComponentEnable ("NetworkControllerComponent", LOG_LEVEL_ALL);
  // LogComponentEnable ("LoraNetDevice", LOG_LEVEL_ALL);
  // LogComponentEnable ("GatewayLorawanMac", LOG_LEVEL_ALL);
  LogComponentEnable ("EndDeviceLorawanMac", LOG_LEVEL_ALL);
  // LogComponentEnable ("EndDeviceLoraPhy", LOG_LEVEL_ALL);
  // LogComponentEnable ("EndDeviceStatus", LOG_LEVEL_ALL);

  // LogComponentEnableAll (LOG_PREFIX_FUNC);
  // LogComponentEnableAll (LOG_PREFIX_NODE);
  // LogComponentEnableAll (LOG_PREFIX_TIME);

  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new InitializeLorawanMacClassCEndDeviceTest, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static ClassCEndDeviceLorawanMacTestSuite lorawanTestSuite;
